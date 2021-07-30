#include <stddef.h>

#include "esb_light.h"
#include "esb_light_cmd_def.h"
#include "esb_light_app_if.h"

/* Turn light on or off
 * payload length: 1
 *  payload: 0: (uint8_t) state (0->OFF, !=0 -> ON)
 * answer payload: None
 * answer error: ESB_PROT_ERR_OK if successful, ESB_PROT_REPLY_ERR_API if not
 */
void esb_light_cmd_fct_set_state(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    answer->payload_len = 0;
    
    esb_protocol_err_t result = esb_light_set_property(ESB_LIGHT_PROP_STATE, message->payload, 1);

    if(result == ESB_PROT_ERR_CMD){
        answer->error = ESB_PROT_REPLY_ERR_NOT_SUPPORTED;
    }else if(result != ESB_PROT_ERR_OK){
        answer->error = ESB_PROT_REPLY_ERR_API;
    }
    
    return;
}

/* Set brightness
 * payload length: 1
 *  payload: 0: (uint8_t) brightness (0->0% ... 255->100%)
 * answer payload: None
 * answer error: 
 *      ESB_PROT_ERR_OK if successful
 *      ESB_PROT_REPLY_ERR_API if not successfull
 *      ESB_PROT_REPLY_ERR_NOT_SUPPORTED if this device does not support setting brightness
 */
void esb_light_cmd_fct_set_brightness(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    answer->payload_len = 0;
    
    esb_protocol_err_t result = esb_light_set_property(ESB_LIGHT_PROP_BRIGHTNESS, message->payload, 1);

    if(result == ESB_PROT_ERR_CMD){
        answer->error = ESB_PROT_REPLY_ERR_NOT_SUPPORTED;
    }else if(result != ESB_PROT_ERR_OK){
        answer->error = ESB_PROT_REPLY_ERR_API;
    }
    
    return;
}

/* Set RGB values
 * payload length: 3
 *  payload: 0: (uint8_t) RED value
 *  payload: 1: (uint8_t) GREEN value
 *  payload: 2: (uint8_t) BLUE value
 * answer payload: None
 * answer error: 
 *      ESB_PROT_ERR_OK if successful
 *      ESB_PROT_REPLY_ERR_API if not successfull
 *      ESB_PROT_REPLY_ERR_NOT_SUPPORTED if this device does not support RGB
 */
void esb_light_cmd_fct_set_rgb(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    answer->payload_len = 0;
    
    esb_protocol_err_t result = esb_light_set_property(ESB_LIGHT_PROP_RGB, message->payload, 3);

    if(result == ESB_PROT_ERR_CMD){
        answer->error = ESB_PROT_REPLY_ERR_NOT_SUPPORTED;
    }else if(result != ESB_PROT_ERR_OK){
        answer->error = ESB_PROT_REPLY_ERR_API;
    }
    
    return;
}

/* Set RGBW values
 * payload length: 4
 *  payload: 0: (uint8_t) RED value
 *  payload: 1: (uint8_t) GREEN value
 *  payload: 2: (uint8_t) BLUE value
 *  payload: 3: (uint8_t) WHITE value
 * answer payload: None
 * answer error: 
 *      ESB_PROT_ERR_OK if successful
 *      ESB_PROT_REPLY_ERR_API if not successfull
 *      ESB_PROT_REPLY_ERR_NOT_SUPPORTED if this device does not support RGBW
 */
void esb_light_cmd_fct_set_rgbw(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    answer->payload_len = 0;
    
    esb_protocol_err_t result = esb_light_set_property(ESB_LIGHT_PROP_RGBW, message->payload, 4);

    if(result == ESB_PROT_ERR_CMD){
        answer->error = ESB_PROT_REPLY_ERR_NOT_SUPPORTED;
    }else if(result != ESB_PROT_ERR_OK){
        answer->error = ESB_PROT_REPLY_ERR_API;
    }
    
    return;
}

/* Set HSI values
 * payload length: 3
 *  payload: 0: (uint8_t) Hue value
 *  payload: 1: (uint8_t) Satuarion value
 *  payload: 2: (uint8_t) Intensity value
 * answer payload: None
 * answer error: 
 *      ESB_PROT_ERR_OK if successful
 *      ESB_PROT_REPLY_ERR_API if not successfull
 *      ESB_PROT_REPLY_ERR_NOT_SUPPORTED if this device does not support HSI
 */
void esb_light_cmd_fct_set_hsi(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    answer->payload_len = 0;
    
    esb_protocol_err_t result = esb_light_set_property(ESB_LIGHT_PROP_HSI, message->payload, 3);

    if(result == ESB_PROT_ERR_CMD){
        answer->error = ESB_PROT_REPLY_ERR_NOT_SUPPORTED;
    }else if(result != ESB_PROT_ERR_OK){
        answer->error = ESB_PROT_REPLY_ERR_API;
    }
    
    return;
}

/* Set color temperature
 * payload length: 1
 *  payload: 0: (uint8_t) Color temperature (Kelvin)
 * answer payload: None
 * answer error: 
 *      ESB_PROT_ERR_OK if successful
 *      ESB_PROT_REPLY_ERR_API if not successfull
 *      ESB_PROT_REPLY_ERR_NOT_SUPPORTED if this device does not support color temperature
 */
void esb_light_cmd_fct_set_temp(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;
    answer->payload_len = 0;
    
    esb_protocol_err_t result = esb_light_set_property(ESB_LIGHT_PROP_TEMP, message->payload, 1);

    if(result == ESB_PROT_ERR_CMD){
        answer->error = ESB_PROT_REPLY_ERR_NOT_SUPPORTED;
    }else if(result != ESB_PROT_ERR_OK){
        answer->error = ESB_PROT_REPLY_ERR_API;
    }
    
    return;
}



#define ESB_LIGHT_CMD_NUM   6
/*!
 * \brief Command table
 */
esb_cmd_table_item_t esb_light_cmd_table[ESB_LIGHT_CMD_NUM+1] = 
{
    /* COMMAND_ID                            PAYLOAD_SIZE   FUNCTION_POINTER*/
    {ESB_LIGHT_CMD_SET_STATE                 ,1              ,esb_light_cmd_fct_set_state},
    {ESB_LIGHT_CMD_SET_BRIGHTNESS            ,1              ,esb_light_cmd_fct_set_brightness},
    {ESB_LIGHT_CMD_SET_RGB                   ,3              ,esb_light_cmd_fct_set_rgb},
    {ESB_LIGHT_CMD_SET_RGBW                  ,4              ,esb_light_cmd_fct_set_rgbw},
    {ESB_LIGHT_CMD_SET_HSI                   ,3              ,esb_light_cmd_fct_set_hsi},
    {ESB_LIGHT_CMD_SET_TEMP                  ,1              ,esb_light_cmd_fct_set_temp},
    
    /* last entry must be NULL-terminator */
    {0,                                     0,             NULL}
};


esb_cmd_table_item_t* esb_light_get_cmd_table(uint8_t *num_entries)
{
    *num_entries = (uint8_t)ESB_LIGHT_CMD_NUM;
    return (esb_light_cmd_table);
}