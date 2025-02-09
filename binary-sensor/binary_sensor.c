#include "binary_sensor.h"
#include "binary_sensor_esb_cmd_def.h"
#include <stddef.h>
#include <string.h>

#ifndef BINARY_SENSOR_CHAN_NUM
#define BINARY_SENSOR_CHAN_NUM 1
#endif

#define BINARY_SENSOR_NOTIFICATION_ESB_PL_LEN 7

typedef struct {
    channel_value_t value; /*!< Binary value of this channel */
    uint8_t value_changed; /*!< Indicator if this value has changed since last publishing */
} channel_t;

static channel_t g_channels[BINARY_SENSOR_CHAN_NUM];

static uint8_t g_module_initialized = 0;
static uint8_t g_peripheral_address[5] = {0}; /*!< ESB pipeline address of this binary sensor device */
static uint8_t g_central_address[5] = {0};    /*!< the central device which shall receive notifications */

esb_protocol_err_t binary_sensor_init(const uint8_t peripheral_address[5])
{
    if (peripheral_address == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }
    memcpy(g_peripheral_address, peripheral_address, sizeof(g_peripheral_address));

    /* register config commands in ESB command table */
    uint8_t num_entries = 0;
    esb_cmd_table_item_t *p_cmd_table = binary_sensor_get_esb_cmd_table(&num_entries);
    esb_protocol_err_t esb_result = esb_commands_register_app_commands(p_cmd_table, num_entries);

    if (esb_result != ESB_PROT_ERR_OK) {
        return (ESB_PROT_ERR_MEM);
    }
    g_module_initialized = 1;

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t binary_sensor_set_central_address(uint8_t central_address[5])
{
    if (central_address == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }
    memcpy(g_central_address, central_address, sizeof(g_central_address));

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t binary_sensor_set_channel(uint8_t chan_id, channel_value_t value)
{
    if (chan_id > BINARY_SENSOR_CHAN_NUM) {
        return (ESB_PROT_ERR_PARAM);
    }

    if (value > 1) {
        return (ESB_PROT_ERR_VALUE);
    }

    if (g_channels[chan_id].value != value) {
        g_channels[chan_id].value = value;
        g_channels[chan_id].value_changed = 1;
    }
    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t binary_sensor_get_channel(uint8_t chan_id, channel_value_t *p_value)
{
    if (chan_id > BINARY_SENSOR_CHAN_NUM) {
        return (ESB_PROT_ERR_PARAM);
    }

    if (p_value == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }

    *p_value = g_channels[chan_id].value;

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t binary_sensor_publish(void)
{
    /* check that adresses are set */
    if ((memcmp(g_central_address, 0, sizeof(g_central_address)) == 0) ||
        (memcmp(g_peripheral_address, 0, sizeof(g_peripheral_address)) == 0)) {
        return (ESB_PROT_ERR_INIT);
    }

    esb_protocol_message_t esb_message = {
        .cmd = BINARY_SENSOR_NOTIFICATION_ESB_CMD_ID,
        .error = 0,
        .payload_len = BINARY_SENSOR_NOTIFICATION_ESB_PL_LEN,
    };
    memcpy(esb_message.address, g_central_address, sizeof(g_central_address));
    memcpy(esb_message.payload, g_peripheral_address, sizeof(g_peripheral_address));
    for (uint8_t i = 0; i < BINARY_SENSOR_CHAN_NUM; i++) {
        if (g_channels[i].value_changed == 1) {
            g_channels[i].value_changed = 0;
            esb_message.payload[5] = i;
            esb_message.payload[6] = g_channels[i].value;

            esb_protocol_transmit(&esb_message);
        }
    }
    return (ESB_PROT_ERR_OK);
}
