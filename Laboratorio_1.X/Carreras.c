/*
 * File:   Carreras.c
 * Author: Jorge
 *
 * Created on 22 de enero de 2020, 02:21 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000 //Frecuencia a la cual vamos a trabajar

char bandera = 0; //Declaracion de variables
char player1 = 0, player2 = 0;
char anti1 = 0, anti2 = 0;
unsigned char const SEGMENT_MAP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; //Valores para el display en hexadecimal
unsigned char const COUNTER_MAP[9] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //Valores para el contador de decada

void main(void) {
    TRISA = 0; //Configuracion de puertos
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;

    TRISDbits.TRISD5 = 1;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;

    PORTA = 0; //Valores iniciales de los puertos
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;

    while (1) { //Loop principal
        if (PORTDbits.RD5 == 1) { //Boton de inicio

            bandera = 0; //Reinicio de puertos y variables
            player1 = 0;
            player2 = 0;
            anti1 = 0;
            anti2 = 0;
            PORTA = 0;
            PORTB = 0;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 0;

            PORTDbits.RD0 = 1; //Secuencia de semaforo
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 0;
            PORTC = SEGMENT_MAP [3]; //Conteo en el display
            __delay_ms(1000); //Delay de 1 segundo para la frecuencia __XTAL_FREQ

            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 1;
            PORTDbits.RD2 = 0;
            PORTC = SEGMENT_MAP [2];
            __delay_ms(1000);

            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 1;
            PORTC = SEGMENT_MAP [1];
            __delay_ms(1000);

            PORTDbits.RD0 = 1;
            PORTDbits.RD1 = 1;
            PORTDbits.RD2 = 1;
            PORTC = SEGMENT_MAP [0];
            bandera = 1;
        }
        while (bandera == 1) { //Loop de la carrera

            if (PORTDbits.RD6 == 1 && anti1 == 0) { //incremento del contador de decadas con antirrebote
                player1++;
                anti1 = 1;
            }
            if (PORTDbits.RD7 == 1 && anti2 == 0) {
                player2++;
                anti2 = 1;
            }

            PORTA = COUNTER_MAP[player1]; //Imprimir en los leds el contador de decadas
            PORTB = COUNTER_MAP[player2];

            if (player1 == 8) { //Verificar ganador y fin de la carrera
                PORTC = SEGMENT_MAP[1];
                PORTDbits.RD3 = 1;
                bandera = 0;
            }
            if (player2 == 8) {
                PORTC = SEGMENT_MAP[2];
                PORTDbits.RD4 = 1;
                bandera = 0;
            }

            if (PORTDbits.RD6 == 0) { //Reiniciar el antirebote de los botones
                anti1 = 0;
            }
            if (PORTDbits.RD7 == 0) {
                anti2 = 0;
            }
        }
    }
}
