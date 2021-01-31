```
/** Using the Sparkfun SEN-10250 BoB
 *
 * Example:
 * @code
 *  #include "mbed.h"
 *  #include "MPR121.h"
 *  
 *  int main()  
 *  {       
 *      touch_pad.init();
 *      touch_pad.enable();
 *      
 *      while(1)
 *      {
 *          if(touch_pad.isPressed())
 *          {
 *              uint16_t button_val = touch_pad.buttonPressed();
 *              printf("button = 0x%04x\n", button_val);
 *              myled = (button_val>0) ? 1 : 0;
 *          }            
 *      }
 *  }
 * @endcode
 */
 ```