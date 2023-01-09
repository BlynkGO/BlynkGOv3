#ifndef LITTLEVGL_FONT_CONFIG_H
#define LITTLEVGL_FONT_CONFIG_H

/*==================
 *    FONT USAGE
 *===================*/

/* The built-in fonts contains the ASCII range and some Symbols with  4 bit-per-pixel.
 * The symbols are available via `LV_SYMBOL_...` defines
 * More info about fonts: https://docs.littlevgl.com/#Fonts
 * To create a new font go to: https://littlevgl.com/ttf-font-to-c-array
 */

/* Robot fonts with bpp = 4
 * https://fonts.google.com/specimen/Roboto  */
#define LV_FONT_ROBOTO_12    0
#define LV_FONT_ROBOTO_16    0
#define LV_FONT_ROBOTO_22    0
#define LV_FONT_ROBOTO_28    0

/* Demonstrate special features */
#define LV_FONT_ROBOTO_12_SUBPX 1
#define LV_FONT_ROBOTO_28_COMPRESSED 1  /*bpp = 3*/

/*Pixel perfect monospace font
 * http://pelulamu.net/unscii/ */
#define LV_FONT_UNSCII_8     0

/* Optionally declare your custom fonts here.
 * You can use these fonts as default font too
 * and they will be available globally. E.g.
 * #define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(my_font_1) \
 *                                LV_FONT_DECLARE(my_font_2)
 */
#define LV_FONT_CUSTOM_DECLARE


// Thai LV_FONT
#define PRASANMIT_20  1
#define PRASANMIT_25  1
#define PRASANMIT_30  1
#define PRASANMIT_35  1
#define PRASANMIT_40  1
// #define PRASANMIT_60  0

// Lao LV_FONT
// #define SAYSETTHA_15  0
// #define SAYSETTHA_20  1
// #define SAYSETTHA_25  0
// #define SAYSETTHA_30  0
// #define SAYSETTHA_35  0
// #define SAYSETTHA_40  0


/*Always set a default font from the built-in fonts*/
#define LV_FONT_DEFAULT        &prasanmit_30

/* Enable it if you have fonts with a lot of characters.
 * The limit depends on the font size, font face and bpp
 * but with > 10,000 characters if you see issues probably you need to enable it.*/
#define LV_FONT_FMT_TXT_LARGE   1

/* Set the pixel order of the display.
 * Important only if "subpx fonts" are used.
 * With "normal" font it doesn't matter.
 */
#define LV_FONT_SUBPX_BGR    0



/*Declare the type of the user data of fonts (can be e.g. `void *`, `int`, `struct`)*/
typedef void * lv_font_user_data_t;




#endif //LITTLEVGL_FONT_CONFIG_H