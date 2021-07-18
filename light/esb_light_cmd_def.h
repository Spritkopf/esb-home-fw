
#ifndef ESB_LIGHT_CMD_DEF_H_
#define ESB_LIGHT_CMD_DEF_H_

#include <common/commands/esb_commands.h>

enum 
{
    ESB_LIGHT_CMD_SET_STATE      = 0xA0,   /* Turn the light ON or OFF*/
    ESB_LIGHT_CMD_SET_BRIGHTNESS = 0xA1,   /* Set brightness */
    ESB_LIGHT_CMD_SET_RGB        = 0xA2,   /* Set RGB */
    ESB_LIGHT_CMD_SET_RGBW       = 0xA3,   /* Set RGBW */
    ESB_LIGHT_CMD_SET_HSI        = 0xA4,   /* Set HSI */
    ESB_LIGHT_CMD_SET_TEMP       = 0xA5,   /* Set color temperature */
} esb_light_cmd_id_t;




/*! 
 * \brief get pointer to ESB Light command table 
 * \param[out] num_entries number of entries in the command table
 */
esb_cmd_table_item_t* esb_light_get_cmd_table(uint8_t *num_entries);

#endif /* ESB_LIGHT_CMD_DEF_H_ */