#ifndef ESB_H_
#define ESB_H_

#include <stdint.h>

#define ESB_ERR_OK 0       /* No Error */
#define ESB_ERR_INIT -1    /* ESB module not initialized */
#define ESB_ERR_HAL -2     /* Error accessing ESB function */
#define ESB_ERR_SIZE -3    /* Invalid payload length */
#define ESB_ERR_PARAM -4   /* Function parameter error */
#define ESB_ERR_TIMEOUT -5 /* Timeout waiting for an answer */

typedef void (*esb_listener_callback_t)(uint8_t *payload, uint8_t payload_length);

typedef enum {
    ESB_PIPE_0 = 0x00,
    ESB_PIPE_1 = 0x01,
    ESB_PIPE_NUM = 0x02,
} esb_pipeline_t;

/* Initialize Enhanced Shockburst (ESB) communication
 * \retval ESB_ERR_OK       - OK
 * \retval ESB_ERR_HAL      - ESB HAL Error
 */
int8_t esb_init(void);

/* \brief Set pipeline address
 * \param[in] pipeline          Pipeline number to listen on
 * \param[in] addr              Pipeline address
 */
int8_t esb_set_pipeline_address(const esb_pipeline_t pipeline, const uint8_t addr[5]);

/* \brief Start listening on pipeline
 * \param pipeline[in]              Pipeline number to listen on
 * \param listener_callback[in]     gets called on incoming package
 * \retval ESB_ERR_OK           - OK
 * \retval ESB_ERR_PARAM        - Parameter Error (NULL Pointer, Illegal pipeline number)
 */
int8_t esb_start_listening(const esb_pipeline_t pipeline, esb_listener_callback_t listener_callback);

/* \brief Stop listening on pipeline
 * \param pipeline[in]          Pipeline number to listen on
 * \retval ESB_ERR_OK           - OK
 * \retval ESB_ERR_PARAM        - Illegal pipeline number
 */
int8_t esb_stop_listening(const esb_pipeline_t pipeline);

/* \brief Set RF Channel
 * \param pipeline[in]  Pipeline number to listen on
 * \retval ESB_ERR_OK         - OK
 * \retval ESB_ERR_HAL        - Error setting channel
 */
int8_t esb_set_rf_channel(const uint8_t channel);

/* \brief Send data
 * \param pipeline          Target Pipeline address
 * \param payload           Pointer to buffer for payload data
 * \param payload_length    Length of payload buffer
 * \retval ESB_ERR_OK       OK
 * \retval ESB_ERR_INIT     Module not initialized
 * \retval ESB_ERR_HAL      ESB HAL Error
 * \retval ESB_ERR_SIZE     Invalid Payload length
 * \retval ESB_ERR_PARAM    Parameter Error (NULL Pointer)
 * \retval ESB_ERR_TIMEOUT  ESB Timeout waiting for TX success
 */
int8_t esb_send_packet(const esb_pipeline_t pipeline, const uint8_t *payload, uint8_t payload_length);

#endif
