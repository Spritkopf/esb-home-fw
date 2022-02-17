
#ifndef ESB_CMD_DEF_COMMON_H_
#define ESB_CMD_DEF_COMMON_H_

#include <common/commands/esb_commands.h>

enum
{
    ESB_CMD_VERSION     = 0x10,   /* Get firmware version */
    ESB_CMD_BOND        = 0x11,   /* Bond with device, set address of connected central */
    ESB_CFG_SET_ITEM    = 0x21,   /* Set an configuration item */
    ESB_CFG_GET_ITEM    = 0x22,   /* Get an configuration item */
} esb_cmd_id_common;


/*! \brief get pointer to common command table */
esb_cmd_table_item_t* get_esb_cmd_table_common(void);

#endif /* ESB_CMD_DEF_COMMON_H_ */
