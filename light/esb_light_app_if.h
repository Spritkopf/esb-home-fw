
#ifndef ESB_LIGHT_APP_IF_H_
#define ESB_LIGHT_APP_IF_H_

#include <stdint.h>
/******************************************************************************/
/*! \brief Interface definitions for application functions
 * \details These functions must be implemented by the user application. They are
 *  called when the respective incoming ESB command is processed.
 */
/******************************************************************************/

/*! \brief Set light state (Enabled/Disabled)
 * \param[in] enabled   Turn the light ON(1) or OFF(0)
 */
void esb_light_app_set_state(uint8_t enabled);

/*! \brief Set brightness
 * \param[in] brightness   Set brightness (0-255)
 */
void esb_light_app_set_brightness(uint8_t brightness);

/*! \brief Set RGB values
 * \param[in] r   Set RED value (0-255)
 * \param[in] g   Set GREEN value (0-255)
 * \param[in] b   Set BLUE value (0-255)
 */
void esb_light_app_set_rgb(uint8_t r, uint8_t g, uint8_t b);

/*! \brief Set RGBW values
 * \param[in] r   Set RED value (0-255)
 * \param[in] g   Set GREEN value (0-255)
 * \param[in] b   Set BLUE value (0-255)
 * \param[in] w   Set WHITE value (0-255)
 */
void esb_light_app_set_rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/*! \brief Set HSI values
 * \param[in] h   Set HUE value (0-255)
 * \param[in] s   Set SATURATIONvalue (0-255)
 * \param[in] i   Set INTENSITY value (0-255)
 */
void esb_light_app_set_hsi(uint8_t h, uint8_t s, uint8_t i);

/*! \brief Set color temperature
 * \param[in] color_temp   Set Color temperature in Kelvin
 */
void esb_light_app_set_color_temp(uint16_t color_temp);
#endif /* ESB_LIGHT_APP_IF_H_ */