/* -------------------------------------------------------
                         math3.c

     Demoprogramm fuer Sinusberechnungen

     MCU   :  STM8S103F3
     Takt  :  interner Takt 16 MHz

     19.05.2016  R. Seelig
   ------------------------------------------------------ */

#include <stdint.h>
#include <math.h>
#include "stm8s.h"
#include "stm8_init.h"
#include "stm8_gpio.h"
#include "my_printf.h"
#include "stm8_nokia.h"

#define countspeed   350

#define printf       my_printf


#define bled_output_init()   PB5_output_init()
#define bled_set()           PB5_set()
#define bled_clr()           PB5_clr()

#define exled_output_init()  PD4_output_init()
#define exled_set()          PD4_set()
#define exled_clr()          PD4_clr()

#define button_input_init()  PD3_input_init()
#define is_button()          is_PD3()


/* --------------------------------------------------------
   putchar

   wird von my-printf / printf aufgerufen und hier muss
   eine Zeichenausgabefunktion angegeben sein, auf das
   printf dann schreibt !
   -------------------------------------------------------- */
void putchar(char ch)
{
  lcd_putchar_d(ch);
}


int main(void)
{
  int   cnt;
  int   i;
  float r;

  uint32_t z;

  sysclock_init(0);

  bled_output_init();
  exled_output_init();
  button_input_init();

  printfkomma= 2;

  lcd_init();
  clrscr();
  gotoxy(0,0);
  printf(" STM8S103F3P6\n\r");
  printf("    math.h\n\r");
  printf("--------------\n\r");

  z= 876121;
  z= z / 4;

  i= z;
  gotoxy(0,3);
//  printf("z= %d",i);
  delay_ms(1000);

  cnt= 0;
  while(1)
  {

    bled_set();
    delay_ms(countspeed);
    bled_clr();
    delay_ms(countspeed);

    if (cnt & 1) { exled_set(); } else { exled_clr(); }

    cnt++;
    cnt= cnt % 360;
    r= sinf((float)cnt * (PI / 180.0f));
    i= (r * 100.0);
    gotoxy(0,3); printf("              ");
    gotoxy(0,3); printf("SIN(%d)=%k",cnt,i);
    r= cosf((float)cnt * (PI / 180.0f));
    i= (r * 100.0);
    gotoxy(0,4); printf("              ");
    gotoxy(0,4); printf("COS(%d)=%k",cnt,i);
  }
}
