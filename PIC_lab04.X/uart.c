#include <xc.h>

void DEBUG_UART_INIT()
{
    // Καθορισμός ακροδέκτη RF0 ως ψηφιακή έξοδος και
    // ορισμός του ως σειριακή έξοδος της UART1
    ANSELFbits.ANSELF0 = 0;
    TRISFbits.TRISF0 = 0;
    // U1TX -> RF0
    RF0PPS = 0x20;
    
    // Καθορισμός ακροδέκτη RF1 ως ψηφιακή είσοδος και
    // ορισμός του ως σειριακή είσοδος της UART1
    ANSELFbits.ANSELF1 = 0;
    TRISFbits.TRISF1 = 1;
    // U1RX <- RF1
    U1RXPPS = 0b00101001;
    
    // Καθορισμός baud rate στα 115200
    U1BRGH = 0;  U1BRGL = 139;
    
    // Ρυθμίσεις σειριακής θύρας UART1
    U1CON0 = 0;
    U1CON0bits.BRGS = 1;
    U1CON0bits.TXEN = 1;
    U1CON0bits.RXEN = 1;
    U1CON2 = 0;
    U1CON1 = 0;
    
    // Ενεργοποίηση σειριακής θύρας UART1
    U1CON1bits.ON = 1;
}

// Υλοποίηση της συνάρτησης putch() για τη χρήση εντολών
// printf στη σειριακή θύρα UART1
void putch(unsigned char data)
{
    // Αναμονή για όσo ο transmit buffer είναι γεμάτος
    while(U1FIFObits.TXBF == 1);
    // Φόρτωση δεδομένων από την παράμετρο data στον transmit buffer
    U1TXB = data;
}