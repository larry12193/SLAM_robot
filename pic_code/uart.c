#include "uart.h"
#include <pic18f2550.h>

// serial com initialization
void uart_init(void) {
    SPBRG = 0x0C;               // Set baud rate to 9600b/s (12)
    TXSTAbits.SYNC = 0x00;      // Set asynchronous mode
    RCSTAbits.SPEN = 0x01;      // Enable EUSART, configures pins
    TRISCbits.RC7  = 0x01;      // Set as stated in data sheet
    TRISCbits.RC6  = 0x01;      // Set as stated in data sheet
    
    TXSTAbits.BRGH = 0x00;      // Disable high baud rate mode
    BAUDCONbits.BRG16 = 0x00;   // Enable 8-bit baud rate generator mode
    
    RCSTAbits.CREN = 0x01;      // Enable continuous recieve
    TXSTAbits.TXEN = 0x01;      // Enable transmit
}              

// write single character to uart
void uart_write(char data) {
    while(!TRMT);
    TXREG = data;
}         

// write array of text to uart
void uart_write_line(char *line) {
    int i;
    for( i = 0; line[i] != '\0'; i++ ) {
        uart_write(line[i]);
    }
}   

// read single character from uart
char uart_read(void) {
    //while(!RCIF);
    return RCREG;
}  

// reads array of text
void uart_read_line(char *out, unsigned int len) {
    for( unsigned int i = 0; i < len; i++ ) {
        out[i] = uart_read();
    }
}   
