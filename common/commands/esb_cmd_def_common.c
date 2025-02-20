#include <stddef.h>

#include <common/commands/esb_cmd_def_common.h>

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif
#ifndef VERSION_MINOR
#define VERSION_MINOR 0
#endif
#ifndef VERSION_PATCH
#define VERSION_PATCH 0
#endif


/* Get FW Version
 * payload length must be 0
 * answer payload: answer payload
 * answer error: E_OK if OK, otherwise E_ESB
 */
void esb_cmd_fct_get_version(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    answer->error = ESB_PROT_REPLY_ERR_OK;

    answer->payload_len = 3;
    answer->payload[0] = VERSION_MAJOR;
    answer->payload[1] = VERSION_MINOR;
    answer->payload[2] = VERSION_PATCH;
    
    return;
}

void esb_cmd_fct_cfg_set_item(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    /* NOT USED FOR NOW, maybe remove later */
    answer->error = ESB_PROT_REPLY_ERR_CMD;
   
    return;
}

void esb_cmd_fct_cfg_get_item(const esb_protocol_message_t* message, esb_protocol_message_t* answer)
{
    /* NOT USED FOR NOW, maybe remove later */
    answer->error = ESB_PROT_REPLY_ERR_CMD;
   
    return;
}

/*!
 * \brief Command table
 */
esb_cmd_table_item_t esb_cmd_table_common[] = 
{
    /* COMMAND_ID           PAYLOAD_SIZE                FUNCTION_POINTER*/
    {ESB_CMD_VERSION,       0,                          esb_cmd_fct_get_version},
    {ESB_CFG_SET_ITEM,      ESB_CMD_PAYLOAD_LEN_DYN,    esb_cmd_fct_cfg_set_item},
    {ESB_CFG_GET_ITEM,      1,                          esb_cmd_fct_cfg_get_item},
    
    /* last entry must be NULL-terminator */
    {0,                 0,                    NULL}
};



esb_cmd_table_item_t* get_esb_cmd_table_common(void)
{
    return (esb_cmd_table_common);
}
