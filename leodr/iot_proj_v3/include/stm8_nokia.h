/* --------------------- stm8_nokia.h -------------------------

   Headerdatei fuer die Anbindung eines s/w Nokia LCD an
   einen STM8. Abgespeckte LCD-Bibliothek, keine Grafik-
   funktionen

   20.05.2016 R. Seelig
   ------------------------------------------------------------ */

/*
------------------------------------------------------------------
                      STM8S103F3P6 Pinout
------------------------------------------------------------------

                            ------------
UART1_CK / TIM2_CH1 / PD4  |  1     20  |  PD3 / AIN4 / TIM2_CH2 / ADC_ETR
    UART1_TX / AIN5 / PD5  |  2     19  |  PD2 / AIN3
    UART1_RX / AIN6 / PD6  |  3     18  |  PD1 / SWIM
                     NRST  |  4     17  |  PC7 / SPI_MISO
              OSCIN / PA1  |  5     16  |  PC6 / SPI_MOSI
             OSCOUT / PA2  |  6     15  |  PC5 / SPI_CLK
                Vss (GND)  |  7     14  |  PC4 / TIM1_CH4 / CLK_CCO / AIN2
                VCAP (*1)  |  8     13  |  PC3 / TIM1_CH3 /
                Vdd (+Ub)  |  9     12  |  PB4 / I2C_SCL
           TIM2_CH3 / PA3  | 10     11  |  PB5 / I2C_SDA
                            -----------

*1 :  Ist mit min. 1uF gegen GND zu verschalten


       ________________________________
      /                                \
      |   _________________________    |
      |  /                          \  |
      | |                            | |
      | |         N5510 GLCD         | |
      | |                            | |
      | |    (Nokia comp. Display)   | |
      | |                            | |
      |  \__________________________/  |
      \________________________________/
        |   |   |   |   |   |   |   |
        o   o   o   o   o   o   o   o
        1   2   3   4   5   6   7   8
       RST CE  DC  DIN CLK Vcc Lig GND


      Anschlussbelegung N5510 LCD
      Display                STM8S103F3P6
      ---------------------------------------------


      1  RST     .......     PC3  (      Pin.No.: 13)
      2  CE        GND
      3  DC      .......     PC4  (      Pin.No.: 14)
      4  DIN     .......     PC6  (MOSI, Pin.No.: 16)
      5  CLK     .......     PC5  (CLK,  Pin.No.: 15)
      6  Vcc      +3.3V
      7  LIGHT     GND
      8  GND       GND
*/

#ifndef in_stm8_nokia
  #define in_stm8_nokia

  #include "stm8s.h"
  #include "stm8_init.h"
  #include "spi.h"


//  #define n3410

  #define fullascii                             // ASCII Zeichensatz wird auch mit
                                                // Kleinbuchstaben implementiert

  #ifdef fullascii
    #define lastascii 126                       // letztes angegebenes Asciizeichen
  #else
    #define lastascii 96
  #endif


  // LCD Pins

  #define LCD_PORT              PC_ODR
  #define LCD_CR1               PC_CR1
  #define LCD_DDR               PC_DDR
  #define LCD_RST_PIN           PC3
  #define LCD_DC_PIN            PC4

  // <<<<< CE Pin Display hart auf Ground verdrahtet

  #ifdef n3410
    // Nokia 3410

    #define LCD_VISIBLE_X_RES     96
    #define LCD_VISIBLE_Y_RES     64
    #define LCD_REAL_X_RES        102
    #define LCD_REAL_Y_RES        72
    #define OK                    0
    #define OUT_OF_BORDER         1

  #else
    // Nokia 5110 / 3310

    #define LCD_VISIBLE_X_RES     84
    #define LCD_VISIBLE_Y_RES     48
    #define LCD_REAL_X_RES        84
    #define LCD_REAL_Y_RES        48
    #define OK                    0
    #define OUT_OF_BORDER         1
  #endif

  /* ----------------------------------------------------
                          Prototypen
     ---------------------------------------------------- */

  void wrcmd(uint8_t command);                            // sende ein Kommando
  void lcd_init();                                        // initialisiere das Display
  void wrdata(uint8_t dat);                               // sende ein Datum
  void clrscr();                                          // loesche das Display
  void gotoxy(char x,char y);                             // positioniere Ausgabeposition
  void lcd_putchar_d(char ch);                            // setze Zeichen auf das Display
  void lcd_putstring(char *c);                            // schreibe String aus dem RAM


  /* ----------------------------------------------------
     Variable der Displayroutinen
     ---------------------------------------------------- */

  extern char wherex;
  extern char wherey;
  extern char invchar;               // = 1 fuer inversive Textausgabe


  // --------------------------------------------------------------
  // Zeichensatz
  // --------------------------------------------------------------


  /* Bitmaps des Ascii-Zeichensatzes
     ein Smily wuerde so aussehen:
        { 0x36, 0x46, 0x40, 0x46, 0x36 }  // Smiley

     ein grosses A ist folgendermassen definiert:

     { 0x7E, 0x11, 0x11, 0x11, 0x7E }

     . x x x x x x .
     . . . x . . . x
     . . . x . . . x
     . . . x . . . x
     . x x x x x x .

  */

  static const unsigned char fonttab [][5] = {
      { 0x00, 0x00, 0x00, 0x00, 0x00 },   // space
      { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
      { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
      { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
      { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
      { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
      { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
      { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
      { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
      { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
      { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
      { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
      { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
      { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
      { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
      { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
      { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
      { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
      { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
      { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
      { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
      { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
      { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
      { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
      { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
      { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
      { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
      { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
      { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
      { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
      { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
      { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
      { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
      { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
      { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
      { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
      { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
      { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
      { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
      { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
      { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
      { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
      { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
      { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
      { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
      { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
      { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
      { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
      { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
      { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
      { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
      { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
      { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
      { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
      { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
      { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
      { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
      { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
      { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
      { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
      { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // "Yen"
      { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
      { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
      { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
      { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '

  #ifdef fullascii
      { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
      { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
      { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
      { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
      { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
      { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
      { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
      { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
      { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
      { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
      { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
      { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
      { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
      { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
      { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
      { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
      { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
      { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
      { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
      { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
      { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
      { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
      { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
      { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
      { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
      { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
      // Zeichen vom Ascii-Satz abweichend
      { 0x3E, 0x7F, 0x7F, 0x3E, 0x00 },   // Zeichen 123 : ausgefuelltes Oval
      { 0x06, 0x09, 0x09, 0x06, 0x00 },   // Zeichen 124 : hochgestelltes kleines o (fuer Gradzeichen);
      { 0x01, 0x01, 0x01, 0x01, 0x01 },   // Zeichen 125 : Strich in der obersten Reihe
      { 0x00, 0x1D, 0x15, 0x17, 0x00 }    // Zeichen 126 : "hoch 2"
  #endif
  };

#endif

