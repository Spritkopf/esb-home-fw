
#include <common/commands/esb_commands.h>
#include <common/protocol/esb_protocol.h>
#include <stdint.h>
#include <string.h>

#include "nrf_queue.h"

#define ESB_PIPE_SEND ESB_PIPE_0
#define ESB_PIPE_LISTENING ESB_PIPE_1

#define ESB_FRAME_IDX_CMD 0
#define ESB_FRAME_IDX_ERR 1
#define ESB_FRAME_IDX_PIPE 2
#define ESB_FRAME_IDX_PAYLOAD ESB_PROTOCOL_HEADER_SIZE

#define ESB_MESSAGE_QUEUE_SIZE 5 /* up to 5 messages can be stored before processing */

/* define message queues in NO_OVERFLOW mode, throws error when full (don't overwrite old items)*/
NRF_QUEUE_DEF(esb_protocol_message_t, g_queue_tx, ESB_MESSAGE_QUEUE_SIZE, NRF_QUEUE_MODE_NO_OVERFLOW);
NRF_QUEUE_DEF(esb_protocol_message_t, g_queue_rx, ESB_MESSAGE_QUEUE_SIZE, NRF_QUEUE_MODE_NO_OVERFLOW);

static uint8_t g_initialized = 0;
static uint8_t g_pipeline_address[ESB_PIPE_ADDR_LENGTH] = {0};

static void esb_listener_callback(uint8_t *payload, uint8_t payload_length)
{
    if (payload == NULL) {
        return;
    }
    esb_protocol_message_t rx_msg;
    rx_msg.cmd = payload[ESB_FRAME_IDX_CMD];
    rx_msg.error = payload[ESB_FRAME_IDX_ERR];
    rx_msg.payload_len = payload_length - ESB_PROTOCOL_HEADER_SIZE;

    if (rx_msg.payload_len > 0) {
        memcpy(rx_msg.payload, &(payload[ESB_FRAME_IDX_PAYLOAD]), rx_msg.payload_len);
    }
    nrf_queue_push(&g_queue_rx, &rx_msg);
}

static void esb_protocol_send(const esb_pipeline_t pipeline, esb_protocol_message_t *message)
{
    uint8_t tx_buffer[ESB_FRAME_SIZE];
    uint8_t tx_size = message->payload_len + ESB_PROTOCOL_HEADER_SIZE;

    tx_buffer[ESB_FRAME_IDX_CMD] = message->cmd;
    tx_buffer[ESB_FRAME_IDX_ERR] = message->error;
    memcpy(&(tx_buffer[ESB_FRAME_IDX_PIPE]), g_pipeline_address, sizeof(g_pipeline_address));
    memcpy(&(tx_buffer[ESB_FRAME_IDX_PAYLOAD]), message->payload, message->payload_len);

    /* set TX adress if not an answer */
    if (pipeline == ESB_PIPE_SEND) {
        esb_set_pipeline_address(pipeline, message->address);
    }

    esb_send_packet(pipeline, tx_buffer, tx_size);
}

esb_protocol_err_t esb_protocol_init(const uint8_t pipeline_address[5])
{
    if (pipeline_address == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }

    int8_t result = esb_init();
    if (result != ESB_ERR_OK) {
        return (ESB_PROT_ERR_HAL);
    }

    result = esb_set_pipeline_address(ESB_PIPE_LISTENING, pipeline_address);
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

    memcpy(g_pipeline_address, pipeline_address, sizeof(g_pipeline_address));
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

    if (message->payload_len > ESB_PROTOCOL_MAX_PAYLOAD_LEN) {
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
