#ifndef _ESB_LIGHT_H
#define _ESB_LIGHT_H

/*!
 * \file esb_light.h 
 * \brief Application layer for a ESB light device, based on the generic ESB protocol
 * \details The "ESB Light" application offers commands to control a light source. It provides
 * interfaces for several different color options such as RGB, RGBW, HSI, (Warm-)White. The user application
 * is responsible for implementing these, otherwise they will be reported as "not supported".
 * */


#include <stdint.h>
#include <common/protocol/esb_protocol.h>

typedef enum {
    RGB        = 0x01,    /*!< Device has a RGB light source, Supports setting RGB values */
    RGBW       = 0x02,    /*!< Device has a RGBW light source, Supports setting RGBW values */
    TEMP       = 0x04,    /*!< Device has a different White source (warm and cold), Supports setting color temperatures (Kelvin)*/
    HSI        = 0x08,    /*!< Supports setting HSI values */
    BRIGHTNESS = 0x10,    /*!< Device can dim the light source(s), support setting brightness value*/
} light_capability_flags_t;

typedef enum {
    ESB_LIGHT_PROP_STATE      = 0x00,  /*!< Describes if the device is ON or OFF, data: [state] */
    ESB_LIGHT_PROP_BRIGHTNESS = 0x01,  /*!< Describes the brightness, data: [brightness] */
    ESB_LIGHT_PROP_RGB        = 0x02,  /*!< Describes the RGB value, data: [r, g, b] */
    ESB_LIGHT_PROP_RGBW       = 0x03,  /*!< Describes the RGBW value, data: [r, g, b, w] */
    ESB_LIGHT_PROP_TEMP       = 0x04,  /*!< Describes color temperature, data: [kelvin] */
    ESB_LIGHT_PROP_HSI        = 0x05,  /*!< Describes the HSI value, data: [h, s, i] */

    ESB_LIGHT_PROP_NUM  = 0x06,        

} esb_light_property_id_t;

typedef struct {
    uint8_t enabled;
    uint8_t brightness;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t color_temp;
    uint8_t h;
    uint8_t s;
    uint8_t i;
} esb_light_property_t;

/*!
 * \brief Callback for incoming commands
 * \details This callback is called when a ESB command changes a property. While the "esb_light" application layer
 * keeps track of all property values, the user application is responsible for actually handling the light sources 
 * themselves
 */
typedef esb_protocol_msg_err_t (*light_cmd_callback_t)(esb_light_property_id_t prop_id, esb_light_property_t* p_prop_data);

/*!
 * \brief Initialize the "ESB Light" application layer
 * \details Upon initialization the user application can specify which light properties actually are supported
 * by setting the capabilities flags accordingly. The capability value can be any combination of flags 
 * defined in ::light_capability_flags_t 
 * \param[in] peripheral_address    ESB pipeline address of this light device
 * \param[in] capabilities          Supported capabilities, see ::light_capability_flags_t
 * \param[in] cmd_callback          Callback which is called when receiving ESB commands / requests, see ::light_cmd_callback_t
 * \retval ESB_PROT_ERR_OK          No Error
 * \retval ESB_PROT_ERR_PARAM       illegal parameter (NULL-pointer)
 * \retval ESB_PROT_ERR_MEM         No space to register ESb command table, check ::ESB_COMMANDS_NUM_APP_TABLES
 */
esb_protocol_err_t esb_light_init(const uint8_t peripheral_address[5], light_capability_flags_t capabilities, light_cmd_callback_t cmd_callback);

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
 * \param[in] p_prop_data       Pointer to data of the property see ::esb_light_property_t
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_VALUE   unknown property ID
 * \retval ESB_PROT_ERR_PARAM   illegal parameter (NULL-pointer)
 * 
 * \note This function is called from the command handler on incoming ESB commands.
 * It is not supposed to be called from the user application, because it calls the user callback,
 * which should only be called if a message is actually incoming
 */
esb_protocol_err_t esb_light_set_property(esb_light_property_id_t prop_id, esb_light_property_t* p_prop_data);

/*!
 * \brief Get a property
 * \param[in] prop_id           ID of the property, see ::esb_light_property_id_t
 * \param[out] p_prop_data      Pointer to buffer to store property data, see ::esb_light_property_t
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_VALUE   unknown property ID
 * \retval ESB_PROT_ERR_PARAM  illegal parameter (NULL-pointer)
 */
esb_protocol_err_t esb_light_get_property(esb_light_property_id_t prop_id, esb_light_property_t* p_prop_data);

/*!
 * \brief Send notifications to report a certain property
 * \param[in] prop_id           ID of the property to report, see ::esb_light_property_id_t
 * \param[in] p_prop_data       Pointer to data of the property see ::esb_light_property_t
 * \retval ESB_PROT_ERR_OK      OK
 * \retval ESB_PROT_ERR_VALUE   unknown property ID
 * \retval ESB_PROT_ERR_INIT    Module is not initialized, call ::light_init and ::light_set_central_address first
 */
esb_protocol_err_t esb_light_notify_property(esb_light_property_id_t prop_id);

#endif