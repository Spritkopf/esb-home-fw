#include <string.h>
#include <stddef.h>
#include "esb_light.h"
#include "esb_light_cmd_def.h"

#define ESB_LIGHT_NOTIFICATION_CMD_ID    0xAF

typedef struct {
    uint8_t data_len;             /* length of propery data */
    uint8_t* p_data;              /* pointer to data buffer */
    uint8_t supported;            /* wheter this property is supported (1) or not (0)   */
} esb_light_property_table_t;

static uint8_t g_module_initialized = 0;
static uint8_t g_peripheral_address[5] = {0};   /*!< ESB pipeline address of this ESB light device */
static uint8_t g_central_address[5] = {0};      /*!< the central device which shall receive notifications */

static uint8_t g_capabilities = 0;

static uint8_t g_property_enabled = 0;
static uint8_t g_property_brightness = 0;
static uint8_t g_property_rgb[3] = {0};
static uint8_t g_property_rgbw[3] = {0};
static uint8_t g_property_color_temp = 0;
static uint8_t g_property_hsi[3] = {0};

esb_light_property_table_t g_property_table[ESB_LIGHT_PROP_NUM] = {
    /*LEN  DATA                        SUPPORTED */
    {1,    &g_property_enabled,        1},       /* ESB_LIGHT_PROP_STATE */
    {1,    &g_property_brightness,     0},       /*ESB_LIGHT_PROP_BRIGHTNESS */
    {3,    g_property_rgb,             0},       /*ESB_LIGHT_PROP_RGB */
    {4,    g_property_rgbw,            0},       /*ESB_LIGHT_PROP_RGBW */
    {1,    &g_property_color_temp,     0},       /*ESB_LIGHT_PROP_TEMP */
    {3,    g_property_hsi,             0}        /*ESB_LIGHT_PROP_HSI */
};

static void call_app_func(esb_light_property_id_t prop_id, uint8_t* p_data);

esb_protocol_err_t esb_light_init(const uint8_t peripheral_address[5], uint8_t capabilities)
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

    /* enable properties according to selected capabilities */
    if(capabilities&CAP_BRIGHTNESS) g_property_table[ESB_LIGHT_PROP_BRIGHTNESS].supported = 1;
    if(capabilities&CAP_RGB) g_property_table[ESB_LIGHT_PROP_RGB].supported = 1;
    if(capabilities&CAP_RGBW) g_property_table[ESB_LIGHT_PROP_RGBW].supported = 1;
    if(capabilities&CAP_TEMP) g_property_table[ESB_LIGHT_PROP_TEMP].supported = 1;
    if(capabilities&CAP_HSI) g_property_table[ESB_LIGHT_PROP_HSI].supported = 1;

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

esb_protocol_err_t esb_light_set_property(esb_light_property_id_t prop_id, uint8_t* p_prop_data, uint8_t len)
{
    if(prop_id > ESB_LIGHT_PROP_NUM){
        return (ESB_PROT_ERR_VALUE);
    }
    
    if(p_prop_data == NULL){
        return (ESB_PROT_ERR_PARAM);
    }

    if(g_property_table[prop_id].supported == 0){
        return (ESB_PROT_ERR_CMD);
    }

    if(g_property_table[prop_id].data_len != len){
        return (ESB_PROT_ERR_SIZE);
    }

    memcpy(p_prop_data, g_property_table[prop_id].p_data, len);

    call_app_func(prop_id, p_prop_data);
    
    return (ESB_PROT_ERR_OK);
}


esb_protocol_err_t esb_light_get_property(esb_light_property_id_t prop_id, uint8_t* p_prop_data, uint8_t len)
{
    if(prop_id > ESB_LIGHT_PROP_NUM){
        return (ESB_PROT_ERR_VALUE);
    }

    if(p_prop_data == NULL){
        return (ESB_PROT_ERR_PARAM);
    }

    if(g_property_table[prop_id].supported == 0){
        return (ESB_PROT_ERR_CMD);
    }

    if(g_property_table[prop_id].data_len != len){
        return (ESB_PROT_ERR_SIZE);
    }

    memcpy(p_prop_data, g_property_table[prop_id].p_data, len);

    return (ESB_PROT_ERR_OK);
}

esb_protocol_err_t esb_light_notify_property(esb_light_property_id_t prop_id)
{
    if(prop_id > ESB_LIGHT_PROP_NUM){
        return (ESB_PROT_ERR_VALUE);
    }

    /* check that adresses are set */
    if((memcmp(g_central_address, 0, sizeof(g_central_address)) == 0) ||
       (memcmp(g_peripheral_address, 0, sizeof(g_peripheral_address)) == 0)){
        return (ESB_PROT_ERR_INIT);
    }

    esb_protocol_message_t esb_message = {
        .cmd = ESB_LIGHT_NOTIFICATION_CMD_ID,
        .error = 0,
        .payload_len = 5 + g_property_table[prop_id].data_len,
    };
    memcpy(esb_message.address, g_central_address, sizeof(g_central_address));
    memcpy(esb_message.payload, g_peripheral_address, sizeof(g_peripheral_address));
    esb_message.payload[5] = prop_id;
    esb_light_get_property(prop_id, &(esb_message.payload[6]), g_property_table[prop_id].data_len);

    esb_protocol_transmit(&esb_message);
    return (ESB_PROT_ERR_OK);
}


/* static functions */
void call_app_func(esb_light_property_id_t prop_id, uint8_t* p_data)
{
    switch (prop_id)
    {
    case ESB_LIGHT_PROP_STATE:
        esb_light_app_set_state(p_data[0]);
        break;
    case ESB_LIGHT_PROP_BRIGHTNESS:
        esb_light_app_set_brightness(p_data[0]);
        break;
    case ESB_LIGHT_PROP_RGB:
        esb_light_app_set_rgb(p_data[0], p_data[1], p_data[2]);
        break;
    case ESB_LIGHT_PROP_RGBW:
        esb_light_app_set_rgbw(p_data[0], p_data[1], p_data[2], p_data[3]);
        break;
    case ESB_LIGHT_PROP_TEMP:
        esb_light_app_set_color_temp(p_data[0]);
        break;
    case ESB_LIGHT_PROP_HSI:
        esb_light_app_set_hsi(p_data[0], p_data[1], p_data[2]);
        break;
    default:
        break;
    }

    return (ESB_PROT_ERR_OK);

}
/* Weak definitions of the application specific functions. 
They are meant to be overwritten by the user application */
__attribute__((weak)) void esb_light_app_set_state(uint8_t enabled) {}
__attribute__((weak)) void esb_light_app_set_brightness(uint8_t brightness){}
__attribute__((weak)) void esb_light_app_set_rgb(uint8_t r, uint8_t g, uint8_t b){}
__attribute__((weak)) void esb_light_app_set_rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w){}
__attribute__((weak)) void esb_light_app_set_hsi(uint8_t h, uint8_t s, uint8_t i){}
__attribute__((weak)) void esb_light_app_set_color_temp(uint16_t color_temp){}