
#ifndef ESB_COMMANDS_H_
#define ESB_COMMANDS_H_

#include "esb_protocol.h"

#define ESB_CMD_PAYLOAD_LEN_DYN 255  /* all payloads are allowed */

typedef uint8_t esb_cmd_t;

typedef struct
{
    esb_cmd_t cmd_id;         /* command id */
    uint8_t payload_size;     /*  expected payload size */
    void (*cmd_fct_pnt)(const esb_protocol_message_t* message, esb_protocol_message_t* answer);      /* pointer to command function */
} esb_cmd_table_item_t;

/*!
 * \brief Initialize ESB commands module
 */
void esb_commands_init(void);

/*! \brief Register an application specific commands
 *  \details The command table consists of table items (see ::esb_cmd_table_item_t) and musst be
 *           terminated with an NULL entry (see example of common command table ::esb_cmd_table
 *           in esb_protocol.c). The application specic command tables will be loked up in order of
 *           registration, but after the common commands. Duplicate command IDs will be ignored, 
 *           first come first served
 *  \param app_cmd_table            - table with application specific command definitions
 *  \param num_entries              - number of entries (excluding NULL-terminator)
 *  \retval ESB_PROT_ERR_OK         - OK
 *  \retval ESB_PROT_ERR_PARAM      - Parameter Error (NULL Pointer)
 *  \retval ESB_PROT_ERR_MEM        - Maximum number of custom command tables reached (see ESB_COMMANDS_MAX_APP_TABLES)
 *  \retval ESB_PROT_ERR_VALUE      - passed command table is missing the NULL terminator as the last entry
*/
esb_protocol_err_t esb_commands_register_app_commands(esb_cmd_table_item_t* app_cmd_table, uint32_t num_entries);

/*!
 * \brief lookup command ID in table(s)
 * \returns pointer to command entry, NULL if none is found for this command ID
 */
esb_cmd_table_item_t* esb_commands_lookup(uint8_t cmd_id, uint8_t payload_len);


#endif /* ESB_COMMANDS_H_ */
