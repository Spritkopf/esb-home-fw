#ifndef _ESB_LIGHT_H
#define _ESB_LIGHT_H

/*!
 * \file esb_light.h 
 * \brief Application layer for a ESB light device, based on the generic ESB protocol
 * \details The "ESB Light" application offers commands to control a light source. It provides
 * interfaces for several different color options such as RGB, RGBW, HSI, (Warm-)White. The user application
 * is responsible for implementing these, otherwise they will be reported as "not supported".
 * The user application can set and get properties via the api, as well as send a notification for a property.
 * The ESB protocol message of the property notification has the following format:
            |----HEADER----|------- PAYLOAD----------------|
 * Bytes:   |  0   |   1   |     2:6     |    7    |   8 ...   |
 * Value:   | CMD  | ERROR | PERIPH_ADDR | PROP_ID | DATA  ...   |
 * 
 * - CMD:         Command ID for the notification (always 0xAF)
 * - ERROR:       Error byte, not used for notifications (always 0x00)
 * - PERIPH_ADDR: ESB pipeline address of this ESB Light device
 * - PROP_ID:     ID of the Property,  0 < PROP_ID < ESB_LIGHT_PROP_NUM
 * - DATA:        Property data, Length depends on the size of the property, can range between 1 and 4, 
 *                see description of ::esb_light_property_id_t
 * */


#include <stdint.h>
#include <common/protocol/esb_protocol.h>

typedef enum {
    CAP_RGB        = 0x01,    /*!< Device has a RGB light source, Supports setting RGB values */
    CAP_RGBW       = 0x02,    /*!< Device has a RGBW light source, Supports setting RGBW values */
    CAP_TEMP       = 0x04,    /*!< Device has a different White source (warm and cold), Supports setting color temperatures (Kelvin)*/
    CAP_HSI        = 0x08,    /*!< Supports setting HSI values */
    CAP_BRIGHTNESS = 0x10,    /*!< Device can dim the light source(s), support setting brightness value*/
} esb_light_capability_flags_t;

typedef enum {
    ESB_LIGHT_PROP_STATE      = 0x00,  /*!< Describes if the device is ON (1) or OFF (0), data: [state] */
    ESB_LIGHT_PROP_BRIGHTNESS = 0x01,  /*!< Describes the brightness (0-255), data: [brightness] */
    ESB_LIGHT_PROP_RGB        = 0x02,  /*!< Describes the RGB value (0-255), data: [r, g, b] */
    ESB_LIGHT_PROP_RGBW       = 0x03,  /*!< Describes the RGBW value (0-255), data: [r, g, b, w] */
    ESB_LIGHT_PROP_TEMP       = 0x04,  /*!< Describes color temperature in Kelvin, data: [kelvin] */
    ESB_LIGHT_PROP_HSI        = 0x05,  /*!< Describes the HSI value (0-255), data: [h, s, i] */

    ESB_LIGHT_PROP_NUM  = 0x06,        

} esb_light_property_id_t;

/*!
 * \brief Initialize the "ESB Light" application layer
 * \details Upon initialization the user application can specify which light properties actually are supported
 * by setting the capabilities flags accordingly. The capability value can be any combination of flags 
 * defined in ::esb_light_capability_flags_t 
 * \param[in] peripheral_address    ESB pipeline address of this light device
 * \param[in] capabilities          Supported capabilities, see ::esb_light_capability_flags_t
 * \retval ESB_PROT_ERR_OK          No Error
 * \retval ESB_PROT_ERR_PARAM       illegal parameter (NULL-pointer)
 * \retval ESB_PROT_ERR_MEM         No space to register ESb command table, check ::ESB_COMMANDS_NUM_APP_TABLES
 */
esb_protocol_err_t esb_light_init(const uint8_t peripheral_address[5], uint8_t capabilities);

/*!
 * \brief Set the target address for the central device
 * \details All property notifications will be sent to the central device. 
 * \param[in] central_address    ESB pipeline address of the listening central device
 * \retval ESB_PROT_ERR_OK       No Error
 * \retval ESB_PROT_ERR_PARAM    illegal parameter (NULL-pointer)
 */
esb_protocol_err_t esb_light_set_central_address(uint8_t central_address[5]);

/*!
 * \brief Set a property
 * \param[in] prop_id           ID of the property, see ::esb_light_property_id_t
 * \param[in] p_prop_data       Pointer to data of the property see description of ::esb_light_property_id_t for data format
 * \param[in] len               data size of the property, see description of ::esb_light_property_id_t for data size
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_VALUE   unknown property ID
 * \retval ESB_PROT_ERR_PARAM   illegal parameter (NULL-pointer)
 * \retval ESB_PROT_ERR_CMD     Property not supported (Capability)
 * \retval ESB_PROT_ERR_SIZE    Invalid size of property data
 * 
 * \note This function is called from the command handler on incoming ESB commands.
 * It is not supposed to be called from the user application, because it calls the application defined function (see esb_light_app_if.h)
 */
esb_protocol_err_t esb_light_set_property(esb_light_property_id_t prop_id, const uint8_t* p_prop_data, uint8_t len);

/*!
 * \brief Get a property
 * \param[in] prop_id           ID of the property, see ::esb_light_property_id_t
 * \param[out] p_prop_data      Pointer to data of the property see description of ::esb_light_property_id_t for data format
 * \param[in] len               data size of the buffer to hold the property data, see description of ::esb_light_property_id_t for data size
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_VALUE   unknown property ID
 * \retval ESB_PROT_ERR_PARAM   illegal parameter (NULL-pointer)
 * \retval ESB_PROT_ERR_CMD     Property not supported (Capability)
 * \retval ESB_PROT_ERR_SIZE    Invalid size of property data
 */
esb_protocol_err_t esb_light_get_property(esb_light_property_id_t prop_id, uint8_t* p_prop_data, uint8_t len);

/*!
 * \brief Send notifications to report a certain property
 * \param[in] prop_id           ID of the property to report, see ::esb_light_property_id_t
 * \param[in] p_prop_data       Pointer to data of the property see ::esb_light_property_t
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_VALUE   unknown property ID
 * \retval ESB_PROT_ERR_INIT    Module is not initialized, call ::light_init and ::light_set_central_address first
 * \retval ESB_PROT_ERR_CMD     Property not supported (Capability)
 */
esb_protocol_err_t esb_light_notify_property(esb_light_property_id_t prop_id);

#endif