#include <stddef.h>
#include <string.h>

#include <common/commands/esb_cmd_def_common.h>
#include <common/commands/esb_commands.h>

#ifndef ESB_COMMANDS_NUM_APP_TABLES
#define ESB_COMMANDS_NUM_APP_TABLES 3 /* max number of additional app specific command tables (default 3)*/
#endif

esb_cmd_table_item_t *g_cmd_tables[ESB_COMMANDS_NUM_APP_TABLES + 1];
static uint32_t g_num_app_tables = 0;

void esb_commands_init(void)
{
    memset(g_cmd_tables, 0, sizeof(g_cmd_tables));

    /* the first command table is always the common commands */
    g_cmd_tables[0] = get_esb_cmd_table_common();
}

esb_protocol_err_t esb_commands_register_app_commands(esb_cmd_table_item_t *app_cmd_table, uint32_t num_entries)
{
    if (app_cmd_table == NULL) {
        return (ESB_PROT_ERR_PARAM);
    }

    if (g_num_app_tables >= ESB_COMMANDS_NUM_APP_TABLES) {
        /* maximum number of tables reached */
        return (ESB_PROT_ERR_MEM);
    }

    if ((num_entries == 0) || (app_cmd_table[num_entries].cmd_fct_pnt != NULL)) {
        return (ESB_PROT_ERR_VALUE);
    }

    g_num_app_tables++;
    g_cmd_tables[g_num_app_tables] = app_cmd_table;

    return (ESB_PROT_ERR_OK);
}

esb_cmd_table_item_t *esb_commands_lookup(uint8_t cmd_id, uint8_t payload_len)
{

    /* iterate through command tables */
    for (uint32_t table_idx = 0; table_idx < ESB_COMMANDS_NUM_APP_TABLES + 1; table_idx++) {

        for (uint32_t index = 0; g_cmd_tables[table_idx][index].cmd_fct_pnt != NULL; index++) {
            if (cmd_id == g_cmd_tables[table_idx][index].cmd_id) {
                /* check if payload size matches */
                if ((g_cmd_tables[table_idx][index].payload_size == payload_len) ||
                    (g_cmd_tables[table_idx][index].payload_size == ESB_CMD_PAYLOAD_LEN_DYN)) {
                    /* return pointer to entry */
                    return &(g_cmd_tables[table_idx][index]);
                } else {
                    /* payload size does not match */
                    return (NULL);
                }
            }
        }
    }

    return (NULL);
}
