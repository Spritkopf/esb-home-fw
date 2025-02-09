
#include <common/commands/esb_commands.h>
#include <common/protocol/esb_protocol.h>
#include <stdint.h>
#include <string.h>

#include "nrf_queue.h"

#define ESB_PIPE_SEND ESB_PIPE_0
#define ESB_PIPE_LISTENING ESB_PIPE_1

#define ESB_MESSAGE_QUEUE_SIZE 5 /* up to 5 messages can be stored before processing */

/* define message queues in NO_OVERFLOW mode, throws error when full (don't overwrite old items)*/
NRF_QUEUE_DEF(esb_protocol_message_t, g_queue_tx, ESB_MESSAGE_QUEUE_SIZE, NRF_QUEUE_MODE_NO_OVERFLOW);
NRF_QUEUE_DEF(esb_protocol_message_t, g_queue_rx, ESB_MESSAGE_QUEUE_SIZE, NRF_QUEUE_MODE_NO_OVERFLOW);

static uint8_t g_initialized = 0;

static void esb_listener_callback(uint8_t *payload, uint8_t payload_length)
{
    if (payload == NULL) {
        return;
    }
    // debug_swo_printf("received data: length: %d, cmd=%02X\n", payload_length, payload[0]);
    esb_protocol_message_t rx_msg = {.cmd = payload[0], .error = 0x00, .payload_len = payload_length - 1};

    if (payload_length > 1) {
        memcpy(rx_msg.payload, &(payload[1]), rx_msg.payload_len);
    }
    nrf_queue_push(&g_queue_rx, &rx_msg);
}

static void esb_protocol_send(const esb_pipeline_t pipeline, esb_protocol_message_t *message)
{
    uint8_t tx_buffer[32];
    uint32_t tx_size = (uint32_t)message->payload_len + ESB_PROTOCOL_HEADER_SIZE;

    tx_buffer[0] = message->cmd;
    tx_buffer[1] = message->error;
    memcpy(&(tx_buffer[2]), message->payload, message->payload_len);

    /* set TX adress if not an answer */
    if (pipeline == ESB_PIPE_SEND) {
        esb_set_pipeline_address(pipeline, message->address);
    }

    esb_send_packet(pipeline, tx_buffer, tx_size);
}

esb_protocol_err_t esb_protocol_init(const uint8_t listening_address[5])
{
    if (listening_address == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }

    int8_t result = esb_init();
    if (result != ESB_ERR_OK) {
        return (ESB_PROT_ERR_HAL);
    }

    result = esb_set_pipeline_address(ESB_PIPE_LISTENING, listening_address);
    if (result != ESB_ERR_OK) {
        return (ESB_PROT_ERR_HAL);
    }

    result = esb_start_listening(ESB_PIPE_LISTENING, esb_listener_callback);
    if (result != ESB_ERR_OK) {
        return (ESB_PROT_ERR_HAL);
    }

    nrf_queue_reset(&g_queue_tx);
    nrf_queue_reset(&g_queue_rx);

    esb_commands_init();

    g_initialized = 1;
    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t esb_protocol_transmit(const esb_protocol_message_t *message)
{
    if (g_initialized == 0) {
        return (ESB_PROT_ERR_INIT);
    }

    if (message == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }

    nrf_queue_push(&g_queue_tx, message);

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t esb_protocol_process(void)
{
    if (g_initialized == 0) {
        return (ESB_PROT_ERR_INIT);
    }

    /* process incoming messages */
    while (!nrf_queue_is_empty(&g_queue_rx)) {
        esb_protocol_message_t message;
        esb_protocol_message_t answer = {0};
        nrf_queue_pop(&g_queue_rx, &message);

        /* lookup command */
        esb_cmd_table_item_t *cmd = esb_commands_lookup(message.cmd, message.payload_len);

        if (cmd != NULL) {
            cmd->cmd_fct_pnt(&message, &answer);
        } else {
            answer.error = ESB_PROT_REPLY_ERR_CMD;
        }
        /* send reply here if applicable */
        if (answer.error != ESB_PROT_REPLY_NONE) {
            answer.cmd = message.cmd;
            esb_protocol_send(ESB_PIPE_LISTENING, &answer);
        }
    }

    /* process outgoing messages */
    while (!nrf_queue_is_empty(&g_queue_tx)) {
        esb_protocol_message_t message;
        nrf_queue_pop(&g_queue_tx, &message);

        esb_protocol_send(ESB_PIPE_SEND, &message);
    }
    return (ESB_PROT_ERR_OK);
}
