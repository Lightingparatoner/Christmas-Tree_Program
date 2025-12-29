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

#define mode_amount 17
#define loop_amount 4

#include <xc.h>

void Blink(uint8_t LED1, uint8_t LED2, uint8_t flash);

// Enumerations
enum LEDs {
    OFF = 0x00,
    RED = 0x01,
    BLUE = 0x02,
    GREEN = 0x10,
    YELLOW = 0x20
};

enum LED_Modes {
    RG_BLINK_BY,
    RB_BLINK_GY,
    RY_BLINK_GB,
    R_STATIC,
    G_STATIC,
    B_STATIC,
    Y_STATIC,
    RG_FLASH_RG_BLINK_BY,
    RB_FLASH_RB_BLINK_GY,
    RY_FLASH_RY_BLINK_GB,
    BY_FLASH_RG_BLINK_BY,
    GY_FLASH_RB_BLINK_GY,
    GB_FLASH_RY_BLINK_GB,
    ALL_BLINK,
    ALL_FLASH,
    ALL_FLASH_ALL_BLINK,
    ALL_STATIC,
    ALL_OFF
};

enum Blink_Mode {
    BLINK,
    FLASH
};
//

uint8_t mode = 0x00;        // Mode number
uint8_t x = 0;              // For loops
uint8_t button_used = 0x00;

void __interrupt() Change_Mode(void) // Change mode interrupt
{   
    if(mode < mode_amount) {
        mode++;
    }
    else {
        mode = 0x00;
    }
    button_used = loop_amount;  // Stop all ongoing loops instantly to change mode
    INTCON = 0x90;
}

void main(void) 
{
    OSCCON = 0x71;
    ANSEL = 0x00;
    TRISIO = 0x0C;
    INTCON = 0x90;
    OPTION_REG = 0x40 | OPTION_REG;
    while(1) {
        switch(mode) {
            case RG_BLINK_BY:   // Blink between red, green and blue, yellow LEDs
                Blink(RED | GREEN, BLUE | YELLOW, BLINK);
            break;
            case RB_BLINK_GY:   // Blink between red, blue and green, yellow LEDs
                Blink(RED | BLUE, GREEN | YELLOW, BLINK);
            break;
            case RY_BLINK_GB:   // Blink between red, yellow and green, blue LEDs
                Blink(RED | YELLOW, GREEN | BLUE, BLINK);
            break;
            case R_STATIC:   // Static red
                GPIO = RED;
            break;
            case G_STATIC:   // Static green
                GPIO = GREEN;
            break;
            case B_STATIC:   // Static blue
                GPIO = BLUE;
            break;
            case Y_STATIC:   // Static yellow
                GPIO = YELLOW;
            break;
            case RG_FLASH_RG_BLINK_BY:   // Flashing red, green then repeat case RG_BLINK_BY four times
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | GREEN, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | GREEN, BLUE | YELLOW, BLINK);
                }       
            break;
            case RB_FLASH_RB_BLINK_GY:   // Flashing red, blue then repeat case RB_BLINK_GY four times
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | BLUE, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | BLUE, GREEN | YELLOW, BLINK);
                }                
            break;
            case RY_FLASH_RY_BLINK_GB:   // Flashing red, yellow then repeat case RY_BLINK_GB four times
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | YELLOW, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | YELLOW, GREEN | BLUE, BLINK);
                }                
            break;
            case BY_FLASH_RG_BLINK_BY:   // Flashing blue, yellow then repeat case RG_BLINK_BY four times
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(BLUE | YELLOW, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | GREEN, BLUE | YELLOW, BLINK);
                }  
            break;
            case GY_FLASH_RB_BLINK_GY:   // Flashing green, yellow then repeat case RB_BLINK_GY four times
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(GREEN | YELLOW, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | BLUE, GREEN | YELLOW, BLINK);
                }  
            break;
            case GB_FLASH_RY_BLINK_GB:   // Flashing green, blue then repeat case RY_BLINK_GB four times
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(GREEN | BLUE, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | YELLOW, GREEN | BLUE, BLINK);
                }
            break;
            case ALL_BLINK:   // Blink all LEDs
                Blink(RED | GREEN | BLUE | YELLOW, OFF, BLINK);
            break;
            case ALL_FLASH:   // Flash all LEDs
                Blink(RED | GREEN | BLUE | YELLOW, OFF, FLASH);
            break;
            case ALL_FLASH_ALL_BLINK:   // Flash then blink all LEDs
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | GREEN | BLUE | YELLOW, OFF, FLASH);
                }
                for(x = 0; x < (loop_amount - button_used); x++) {
                    Blink(RED | GREEN | BLUE | YELLOW, OFF, BLINK);
                }
            break;
            case ALL_STATIC:   // Static all LEDs
                GPIO = RED | GREEN| BLUE | YELLOW;
            break;
            case ALL_OFF:  // Lights off
                GPIO = OFF;
            break;
        }
        button_used = 0x00; // Re-enable flashing and blinking loops
    }  
    return;
}

void Blink(uint8_t LED1, uint8_t LED2, uint8_t flash) 
{
    switch(flash) {
        case BLINK:
            GPIO = LED1; 
            _delay(switching_delay);
            GPIO = LED2;
            _delay(switching_delay);
        break;
        case FLASH:
            GPIO = LED1 | LED2; 
            _delay(flashing_delay);
            GPIO = OFF;
            _delay(flashing_delay);
        break;
    }
}