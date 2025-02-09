#include <stddef.h>

#include "binary_sensor.h"
#include "binary_sensor_esb_cmd_def.h"

/* Get channel value
 * payload length: 1
 * payload: 0: (uint8_t) channel ID
 * answer payload: (uint8_t) channel value
 * answer error: ESB_PROT_ERR_OK if OK, ESB_PROT_REPLY_ERR_PARAM for invalid channel ID
 */
void binary_sensor_esb_cmd_fct_get_channel(const esb_protocol_message_t *message, esb_protocol_message_t *answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    channel_value_t chan_value;
    esb_protocol_err_t result = binary_sensor_get_channel(message->payload[0], &chan_value);

    if (result != ESB_PROT_ERR_OK) {
        answer->error = ESB_PROT_REPLY_ERR_PARAM;
    } else {
        answer->payload_len = 1;
        answer->payload[0] = chan_value;
    }

    return;
}

/* Set channel value
 * payload length: 2
 * payload: 0: (uint8_t) channel ID,
 *          1: (uint8_t) channel value (only 0 and 1 allowed)
 * answer payload: None
 * answer error: ESB_PROT_ERR_OK if OK, ESB_PROT_REPLY_ERR_PARAM for invalid channel ID or value
 */
void binary_sensor_esb_cmd_fct_set_channel(const esb_protocol_message_t *message, esb_protocol_message_t *answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    esb_protocol_err_t result = binary_sensor_set_channel(message->payload[0], (channel_value_t)message->payload[1]);

    if (result != ESB_PROT_ERR_OK) {
        answer->error = ESB_PROT_REPLY_ERR_PARAM;
    }

    return;
}

#define BINARY_SENSOR_ESB_CMD_NUM 2
/*!
 * \brief Command table
 */
esb_cmd_table_item_t binary_sensor_esb_cmd_table[BINARY_SENSOR_ESB_CMD_NUM + 1] = {
    /* COMMAND_ID                            PAYLOAD_SIZE   FUNCTION_POINTER*/
    {ESB_CMD_BINARY_SENSOR_GET_CHANNEL, 1, binary_sensor_esb_cmd_fct_get_channel},
    {ESB_CMD_BINARY_SENSOR_SET_CHANNEL, 2, binary_sensor_esb_cmd_fct_set_channel},

    /* last entry must be NULL-terminator */
    {0, 0, NULL}};

esb_cmd_table_item_t *binary_sensor_get_esb_cmd_table(uint8_t *num_entries)
{
    *num_entries = (uint8_t)BINARY_SENSOR_ESB_CMD_NUM;
    return (binary_sensor_esb_cmd_table);
}
