#ifndef _BINARY_SENSOR_H
#define _BINARY_SENSOR_H

/*!
 * \file binary_sensor.h
 * \brief Application layer for a binary sensor, based on the generic ESB protocol
 * \details The "Binary Sensor" application notifies a central device when a channel
 * has changed it's state. In addition, it offers commands for manually querying or
 * altering the channel's state (see ::binary_sensor_cmd_def.c). The number of
 * available channels is defined by a compiler switch, see ::BINARY_SENSOR_CHAN_NUM
 *
 * The ESB protocol message of the state notification has the following format:
            |----HEADER----|------- PAYLOAD----------------|
 * Bytes:   |  0   |   1   |     2:6     |    7    |   8   |
 * Value:   | CMD  | ERROR | PERIPH_ADDR | CHAN_ID | STATE |
 *
 * - CMD:         Command ID for the notification (always 0x91)
 * - ERROR:       Error byte, not used for notifications (always 0x00)
 * - PERIPH_ADDR: ESB pipeline address of this binary sensor device
 * - CHAN_ID:     ID of the channel,  0 < CHAN_ID < BINARY_SENSOR_CHAN_NUM
 * - STATE:       the binary state of the channel (0 = off, 1 = on)
 * */

#include <common/protocol/esb_protocol.h>
#include <stdint.h>

#define BINARY_SENSOR_NOTIFICATION_ESB_CMD_ID 0x91

typedef enum {
    CHAN_VAL_FALSE = 0x00, /*!< value for binary OFF */
    CHAN_VAL_TRUE = 0x01   /*!< value for binary ON */
} channel_value_t;

/*!
 * \brief Initialize the "Binary Sensor" application layer
 * \param[in] peripheral_address    ESB pipeline address of this binary sensor device
 * \retval ESB_PROT_ERR_OK          No Error
 * \retval ESB_PROT_ERR_PARAM       illegal parameter (NULL-pointer)
 * \retval ESB_PROT_ERR_MEM         No space to register ESB command table, check ::ESB_COMMANDS_NUM_APP_TABLES
 */
esb_protocol_err_t binary_sensor_init(const uint8_t peripheral_address[5]);
/*!
 * \brief Set the target address for the central device
 * \details All channel state change notifications will be sent to the central device.
 * \param[in] central_address    ESB pipeline address of the listening central device
 * \retval ESB_PROT_ERR_OK       No Error
 * \retval ESB_PROT_ERR_PARAM    illegal parameter (NULL-pointer)
 */
esb_protocol_err_t binary_sensor_set_central_address(uint8_t central_address[5]);

/*!
 * \brief Set the value of a channel
 * \param[in] chan_id           ID of the channel (0 < chan_id < BINARY_SENSOR_CHAN_NUM)
 * \param[in] value             New value of the channel (CHAN_VAL_FALSE(0) | CHAN_VAL_TRUE(1))
 * \retval ESB_PROT_ERR_OK      No Error
 * \retval ESB_PROT_ERR_PARAM   Invalid channel ID
 * \retval ESB_PROT_ERR_VALUE   Invalid value
 */
esb_protocol_err_t binary_sensor_set_channel(uint8_t chan_id, channel_value_t value);

/*!
 * \brief Get the value of a channel
 * \param[in] chan_id           ID of the channel (0 < chan_id < BINARY_SENSOR_CHAN_NUM)
 * \param[out] p_value          Pointer to buffer to store the current value of the channel (CHAN_VAL_FALSE (0) |
 * CHAN_VAL_TRUE(1)) \retval ESB_PROT_ERR_OK      No error \retval ESB_PROT_ERR_PARAM   Invalid channel ID or NULL
 * pointer
 */
esb_protocol_err_t binary_sensor_get_channel(uint8_t chan_id, channel_value_t *p_value);

/*!
 * \brief Send notifications for all changed channels
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_INIT    Module is not initialized, call ::binary_sensor_init and
 * ::binary_sensor_set_central_address first
 */
esb_protocol_err_t binary_sensor_publish(void);

#endif
