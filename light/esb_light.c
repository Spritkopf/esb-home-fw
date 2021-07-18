#include <string.h>
#include <stddef.h>
#include "esb_light.h"
#include "esb_light_cmd_def.h"


static uint8_t g_module_initialized = 0;
static uint8_t g_peripheral_address[5] = {0};   /*!< ESB pipeline address of this ESB light device */
static uint8_t g_central_address[5] = {0};      /*!< the central device which shall receive notifications */

static light_cmd_callback_t g_cmd_callback = NULL;

esb_protocol_err_t esb_light_init(const uint8_t peripheral_address[5], light_capability_flags_t capabilities, light_cmd_callback_t cmd_callback)
{   
    if(peripheral_address == NULL){
        return (ESB_PROT_ERR_PARAM);
    }
    memcpy(g_peripheral_address, peripheral_address, sizeof(g_peripheral_address));

    /* register config commands in ESB command table */
    uint8_t num_entries = 0;
    esb_cmd_table_item_t *p_cmd_table = esb_light_get_cmd_table(&num_entries);
    esb_protocol_err_t esb_result = esb_commands_register_app_commands(p_cmd_table, num_entries);

    if(esb_result != ESB_PROT_ERR_OK){
        return (ESB_PROT_ERR_MEM);
    }

    /* register ESB command callback */
    if(cmd_callback != NULL){
        g_cmd_callback = cmd_callback;
    }
    g_module_initialized = 1;

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t esb_light_set_central_address(uint8_t central_address[5])
{
    if(central_address == NULL){
        return (ESB_PROT_ERR_PARAM);
    }
    memcpy(g_central_address, central_address, sizeof(g_central_address));

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t esb_light_set_property(light_property_id_t prop_id, light_property_t* p_prop_data)
{

}


esb_protocol_err_t esb_light_get_property(light_property_id_t prop_id, light_property_t* p_prop_data)
{

}

esb_protocol_err_t esb_light_notify_property(light_property_id_t prop_id)
{

}