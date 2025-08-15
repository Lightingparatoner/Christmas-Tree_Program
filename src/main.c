/*
 * File: main.c
 * Title: Christmas tree program
 * Author: LightingParatoner
 * Description: Program for Christmas tree circuit.
 * 
 */
// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*
 GPIO Bits:
 * Bit 0: Red
 * Bit 1: Blue
 * Bit 4: Green
 * Bit 5: Yellow
 */

//Delay times, optimized for internal 4MHz oscillator
#define switching_delay 1000000
#define flashing_delay 800000
//

//GPIO Values for each LED driving MOSFET
#define red 0x01
#define blue 0x02
#define green 0x10
#define yellow 0x20
//

#define mode_amount 0x11

#include <xc.h>

void Blink(uint8_t LED1, uint8_t LED2, uint8_t flash);

uint8_t mode = 0x00;
uint8_t x = 0;  // For loops
uint8_t button_used = 0x00; // Disable all loops

void interrupt Change_Mode() {   // Change mode interrupt
    if(mode < mode_amount) {
        mode++;
    }
    else {
        mode = 0x0;
    }
    button_used = 0x04;
    INTCON = 0x90;
}

void main(void) {
    OSCCON = 0x71;
    ANSEL = 0x00;
    TRISIO = 0x0C;
    INTCON = 0x90;
    OPTION_REG = 0x40 | OPTION_REG;
    while(1) {
        switch(mode) {
            case 0x00:   // Blink between red, green and blue, yellow LEDs
                Blink(red | green, blue | yellow, 0x00);
            break;
            case 0x01:   // Blink between red, blue and green, yellow LEDs
                Blink(red | blue, green | yellow, 0x00);
            break;
            case 0x02:   // Blink between red, yellow and green, blue LEDs
                Blink(red | yellow, green | blue, 0x00);
            break;
            case 0x03:   // Static red
                GPIO = red;
            break;
            case 0x04:   // Static green
                GPIO = green;
            break;
            case 0x05:   // Static blue
                GPIO = blue;
            break;
            case 0x06:   // Static yellow
                GPIO = yellow;
            break;
            case 0x07:   // Flashing red, green then repeat case 0x00 four times
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(red | green, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | green, blue | yellow, 0x00);
                }       
            break;
            case 0x08:   // Flashing red, blue then repeat case 0x01 four times
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(red | blue, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | blue, green | yellow, 0x00);
                }                
            break;
            case 0x09:   // Flashing red, yellow then repeat case 0x02 four times
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(red | yellow, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | yellow, green | blue, 0x00);
                }                
            break;
            case 0x0A:   // Flashing blue, yellow then repeat case 0x00 four times
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(blue | yellow, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | green, blue | yellow, 0x00);
                }  
            break;
            case 0x0B:   // Flashing green, yellow then repeat case 0x01 four times
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(green | yellow, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | blue, green | yellow, 0x00);
                }  
            break;
            case 0x0C:   // Flashing green, blue then repeat case 0x02 four times
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(green | blue, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | yellow, green | blue, 0x00);
                }
            break;
            case 0x0D:   // Blink all LEDs
                Blink(red | green | blue | yellow, 0x00, 0x00);
            break;
            case 0x0E:   // Flash all LEDs
                Blink(red | green | blue | yellow, 0x00, 0x01);
            break;
            case 0x0F:   // Flash then blink all LEDs
                for(x = 0; x < (8 - (button_used + 4)); x++) {
                    Blink(red | green | blue | yellow, 0x00, 0x01);
                }
                for(x = 0; x < (4 - button_used); x++) {
                    Blink(red | green | blue | yellow, 0x00, 0x00);
                }
            break;
            case 0x10:   // Static all LEDs
                GPIO = red | green| blue | yellow;
            break;
            case 0x11:  // Lights off
                GPIO = 0x00;
            break;
        }
        button_used = 0x00; // Re-enable mode changing button
    }  
    return;
}

void Blink(uint8_t LED1, uint8_t LED2, uint8_t flash) {
    switch(flash) {
        case 0x00:
            GPIO = LED1; 
            _delay(switching_delay);
            GPIO = LED2;
            _delay(switching_delay);
        break;
        case 0x01:
            GPIO = LED1 | LED2; 
            _delay(flashing_delay);
            GPIO = 0x00;
            _delay(flashing_delay);
        break;
    }
}