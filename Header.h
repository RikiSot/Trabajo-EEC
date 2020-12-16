

#fuses NOJTAG      // JTAG port is disabled
#fuses NOPROTECT   // Code protection is disabled
#fuses NOWRT       // Writes to program memory are allowed
#fuses ICSP1       // Emulator functions are shared with PGEC1/PGED1
#fuses NOWDT       // WDT is disabled; SWDTEN bit is disabled
#fuses WPOSTS16    // Watchdog Timer Postscaler Select bits-> 1:32,768 (NOT USED)
#fuses NOIESO      // Internal External Switchover bit: IESO mode (Two-Speed Start-up) is disabled
#fuses FRC_PS      // Initial Oscillator Select bits: Fast RC Oscillator with Postscaler (FRCDIV)
#fuses CKSNOFSM    // Clock switching is enabled, Fail-Safe Clock Monitor is disabled
#fuses OSCIO       // OSCO Pin Configuration bit: OSCO/CLKO/RC15 functions as port I/O (RC15)
#fuses NOPR        // Primary Oscillator Configuration bits: Primary Oscillator mode is disabled
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#use delay(clock=7370000)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#define ON( pin)   output_high(pin)
#define OFF(pin)   output_low( pin)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//#define SDA1         PIN_B9  //                    ( 1)
#define BUZZER         PIN_C6  //                    ( 2)
#define LED1           PIN_C7  //                    ( 3)
#define LED2           PIN_C8  //                    ( 4)
#define LED3           PIN_C9  //                    ( 5)
//                     VSS     //                    ( 6)
//                     VCAP    //                    ( 7)
#define H3             PIN_B10 //                    ( 8)
#define L3             PIN_B11 //                    ( 9)
#define H2             PIN_B12 //                    (10)
#define L2             PIN_B13 //                    (11)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _I             PIN_A10 //                    (12)
#define BTOK           PIN_A7  //                    (13)
#define H1             PIN_B14 //                    (14)
#define L1             PIN_B15 //                    (15)
//                     AVSS    //                    (16)
//                     AVDD    //                    (17)
//                     ~MCLR   //                    (18)
#define BTIN           PIN_A0  //                    (19)
#define POT  		   PIN_A1  //                    (20)
//                     PIN_B0  //                    (21)
//                     PIN_B1  //                    (22)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define ANL4           PIN_B2  //                    (23)
#define ANL3           PIN_B3  //                    (24)
#define ANL2           PIN_C0  //                    (25)
#define SD             PIN_C1  //                    (26)
#define ANL1           PIN_C2  //                    (27)
//                     VDD     //                    (28)
//                     VSS     //                    (29)
//                     PIN_A2  //                    (30)
//                     PIN_A3  //                    (31)
#define BTCM           PIN_A8  //                    (32)
//                     PIN_B4  //                    (33)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//                     PIN_A4  //                    (34)
#define COMM           PIN_A9  //                    (35) 
#pin_select U1TX =     PIN_C3  //                    (36)
#pin_select U1RX =     PIN_C4  //                    (37)
#define SS             PIN_C5  //                    (38)
//                     VSS     //                    (39)
//                     VDD     //                    (40)
#pin_select SDI1 =     PIN_B5  //                    (41)
#pin_select SCK1OUT=   PIN_B6  //                    (42)
#pin_select SDO1 =     PIN_B7  //                    (43)
//#define SCL1         PIN_B8  //                    (44)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -