#ifndef ESB_PROTOCOL_H_
#define ESB_PROTOCOL_H_

#include <stdint.h>
#include <common/driver/esb.h>

#define ESB_PROTOCOL_HEADER_SIZE        2  /* command and error byte */
#define ESB_PROTOCOL_MAX_PAYLOAD_LEN    32 - ESB_PROTOCOL_HEADER_SIZE

/*! \brief module error codes */
typedef enum {
    ESB_PROT_ERR_OK   = 0x00,           /* No Error */
    ESB_PROT_ERR_HAL  = 0x01,           /* ESB Hal error */
    ESB_PROT_ERR_CMD  = 0x02,           /* unknown command */
    ESB_PROT_ERR_QUEUE_FULL  = 0x03,    /* queue full */
    ESB_PROT_ERR_QUEUE_EMPTY  = 0x04,   /* queue empty */
    ESB_PROT_ERR_PARAM  = 0x05,         /* parameter error */
    ESB_PROT_ERR_INIT  = 0x06,          /* module not initialized */
    ESB_PROT_ERR_MEM  = 0x07,           /* Not enough memory for operation */
    ESB_PROT_ERR_VALUE  = 0x08          /* Value error, unexpected value */
} esb_protocol_err_t;

/*! \brief error code sent in ESB replies */
typedef enum {
    ESB_PROT_REPLY_ERR_OK    = 0x00,  /* No Error */
    ESB_PROT_REPLY_ERR_SIZE  = 0x01,  /* invalid payload length */
    ESB_PROT_REPLY_ERR_CMD   = 0x02,  /* unknown command */
    ESB_PROT_REPLY_ERR_API   = 0x03,  /* Call of an API function returned an Error */
    ESB_PROT_REPLY_ERR_PARAM = 0x04,  /* Invalid Parameter */
    ESB_PROT_REPLY_NONE      = 0xFF   /* special reply code. when set on a reply, the protocol layer will not send a reply for the current command*/
} esb_protocol_msg_err_t;

/*! \brief structure representing ESB messages */
typedef struct {
    uint8_t address[5];          /* pipeline address on which the message shall be sent (tx-only) */
    uint8_t cmd;                 /* command byte */
    esb_protocol_msg_err_t error;   /* message error code, (tx-only)*/
    uint8_t payload[ESB_PROTOCOL_MAX_PAYLOAD_LEN];
    uint8_t payload_len;        /* payload length */
} esb_protocol_message_t;
    

/*! \brief initialize Enhanced Shockburst (ESB) communication protocol
 *  \param listening_address        ESP Pipeline address for listening (only 5-byte address supported)
 *  \retval ESB_PROT_ERR_OK         - OK
 *  \retval ESB_PROT_ERR_HAL        - ESB HAL Error
 *  \retval ESB_PROT_ERR_PARAM      - Parameter Error (NULL Pointer)
*/
esb_protocol_err_t esb_protocol_init(const uint8_t listening_address[5]);

/*! \brief Queue message for transmission
 *  \details Messages don't get sent right away, they will be put in the queue for outgoing
 *           messages and will be sent on the next call of esb_protocol_process()
 *  \param message           Message to queue for transmission
 *  \retval ESB_PROT_ERR_OK         - OK
 *  \retval ESB_PROT_ERR_INIT       - module not initialized
 *  \retval ESB_PROT_ERR_HAL        - ESB HAL Error
 *  \retval ESB_PROT_ERR_PARAM      - Parameter Error (NULL Pointer)
 */
esb_protocol_err_t esb_protocol_transmit(const esb_protocol_message_t* message);

/*! \brief Process incoming and outgoing message queue
 *  \details check for pending incoming messages, execute associated function and
 *           send reply if applicable
 *  \retval ESB_PROT_ERR_OK          - OK
 *  \retval ESB_PROT_ERR_INIT        - module not initialized
 *  \retval ESB_PROT_ERR_HAL         - ESB HAL Error
 */
esb_protocol_err_t esb_protocol_process(void);

#endif  // ESB_PROTOCOL_H_