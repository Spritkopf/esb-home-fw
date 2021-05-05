
#ifndef BINARY_SENSOR_ESB_CMD_DEF_H_
#define BINARY_SENSOR_ESB_CMD_DEF_H_

#include "esb_commands.h"

enum
{
    ESB_CMD_BINARY_SENSOR_GET_CHANNEL     = 0x92,   /* Get channel value */
    ESB_CMD_BINARY_SENSOR_SET_CHANNEL     = 0x93,   /* Set channel value */
} esb_cmd_id_binary_sensor;

/*! 
 * \brief get pointer to binary sensor command table 
 * \param[out] num_entries number of entries in the command table
 */
esb_cmd_table_item_t* binary_sensor_get_esb_cmd_table(uint8_t *num_entries);

#endif /* BINARY_SENSOR_ESB_CMD_DEF_H_ */
