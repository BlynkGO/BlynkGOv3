
#include "esp32_utils.h"


long esp32_mod(long a, long b)
{ return (a%b+b)%b; }

float esp32_mod_f( float a, long b) {
  long mod_i =  esp32_mod( floor(a), b);
  return mod_i + ( a - floor(a));
}


// Fill screen with a rainbow pattern
// uint16_t rainbow_map(int val, int val_min, int val_max)
// {
//   val = constrain(val, val_min, val_max);
//   int value = map_f(val, val_min, val_max, 0, 127);
//   // Value is expected to be in range 0-127
//   // The value is converted to a spectrum colour from 0 = blue through to red = blue
//   //int value = random (128);
//   byte red = 0; // Red is the top 5 bits of a 16 bit colour value
//   byte green = 0;// Green is the middle 6 bits
//   byte blue = 0; // Blue is the bottom 5 bits

//   byte quadrant = value / 32;

//   if (quadrant == 0) {
//     blue = 31;
//     green = 2 * (value % 32);
//     red = 0;
//   }
//   if (quadrant == 1) {
//     blue = 31 - (value % 32);
//     green = 63;
//     red = 0;
//   }
//   if (quadrant == 2) {
//     blue = 0;
//     green = 63;
//     red = value % 32;
//   }
//   if (quadrant == 3) {
//     blue = 0;
//     green = 63 - 2 * (value % 32);
//     red = 31;
//   }
//   return (red << 11) + (green << 5) + blue;
// }