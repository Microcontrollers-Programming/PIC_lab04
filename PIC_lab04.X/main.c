/*
 * File:   main.c
 * Author: Advanced Digital Systems Lab
 *
 * Created on September 5, 2020, 5:51 PM
 */

// <editor-fold defaultstate="collapsed" desc="Configuration bits">

// CONFIG1
// External Oscillator Selection (Oscillator not enabled)
#pragma config FEXTOSC = OFF    

// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)
#pragma config RSTOSC = HFINTOSC_64MHZ

// CONFIG2
// Clock out Enable bit (CLKOUT function is disabled)
#pragma config CLKOUTEN = OFF
// PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be cleared and
// set only once)
#pragma config PR1WAY = ON
// Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed
// by user software)
#pragma config CSWEN = OFF
// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config FCMEN = OFF

// CONFIG3
// MCLR Enable bit
// (If LVP = 0, MCLR pin is MCLR;
// If LVP = 1, RE3 pin function is MCLR)
#pragma config MCLRE = EXTMCLR
// Power-up timer selection bits (PWRT is disabled)
#pragma config PWRTS = PWRT_OFF
// Multi-vector enable bit (Multi-vector enabled,
// Vector table used for interrupts)
#pragma config MVECEN = ON
// IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared
// and set only once)
#pragma config IVT1WAY = ON
// Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config LPBOREN = OFF
// Brown-out Reset Enable bits (Brown-out Reset disabled)
#pragma config BOREN = OFF

// CONFIG4
// Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR)
// set to 1.9V)
#pragma config BORV = VBOR_1P9
// ZCD Disable bit (ZCD module is disabled.
// ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config ZCD = OFF
// PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be cleared
// and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config PPS1WAY = ON
// Stack Full/Underflow Reset Enable bit (Stack full/underflow
// will cause Reset)
#pragma config STVREN = ON
// Low Voltage Programming Enable bit (HV on MCLR/VPP must be used
// for programming)
#pragma config LVP = OFF
// Extended Instruction Set Enable bit (Extended Instruction Set and
// Indexed Addressing Mode disabled)
#pragma config XINST = OFF

// CONFIG5
// WDT Period selection bits (Divider ratio 1:65536;
// software control of WDTPS)
#pragma config WDTCPS = WDTCPS_31
// WDT operating mode (WDT Disabled; SWDTEN is ignored)
#pragma config WDTE = OFF

// CONFIG6
// WDT Window Select bits (window always open (100%);
// software control; keyed access not required)
#pragma config WDTCWS = WDTCWS_7
// WDT input clock selector (Software Control)
#pragma config WDTCCS = SC

// CONFIG7
// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBSIZE = BBSIZE_512
// Boot Block enable bit (Boot block disabled)
#pragma config BBEN = OFF
// Storage Area Flash enable bit (SAF disabled)
#pragma config SAFEN = OFF
// Background Debugger (Background Debugger disabled)
#pragma config DEBUG = OFF

// CONFIG8
// Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTB = OFF
// Configuration Register Write Protection bit (Configuration registers
// not Write protected)
#pragma config WRTC = OFF
// Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTD = OFF
// SAF Write protection bit (SAF not Write Protected)
#pragma config WRTSAF = OFF
// Application Block write protection bit (Application Block not write
// protected)
#pragma config WRTAPP = OFF

// CONFIG10
// PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code
// protection disabled)
#pragma config CP = OFF

// </editor-fold>

#include <stdio.h>
#include <stdint.h>
#include <xc.h>
#include "adc.h"
#include "uart.h"

// Καθορισμός παραμέτρου _XTAL_FREQ με τη συχνότητα Fosc
// που απαιτεί η ρουτίνα __delay_ms()
#define _XTAL_FREQ                      64000000

void main(void)
{
    // Μεταβλητή για την αποθήκευση της μέτρησης της τάσης
    int16_t DVpre;
    
    // Καθορισμός ακροδέκτη RD0 ως ψηφιακή έξοδο και
    // οδήγηση στο λογικό '0'.
    ANSELDbits.ANSELD0 = 0;
    TRISDbits.TRISD0 = 0;
    LATDbits.LATD0 = 0;
    
    // Αρχικοποίηση της UART1 για χρήση ως debug UART
    DEBUG_UART_INIT();
    // Εκτύπωση ενημερωτικού μηνύματος στη UART
    puts("Debug UART is up and running\r");
    
    // Αρχικοποίηση μετατροπέα αναλογικού σήματος σε ψηφιακό
    ADC_INIT();
    
    // Ενεργοποίηση διακοπών υψηλής προτεραιότητας
    INTCON0bits.GIEH = 1;
    
    while(1)
    {
        // Δημιουργία καθυστέρησης 50 ms
        __delay_ms(50);
        
        // Αρχή διαδικασίας μιας απλής μετατροπής αναλογικού σήματος σε ψηφιακό
        
        // Ενεργοποίηση διαδικασίας μίας μετατροπής
        ADCON1bits.DSEN = 0;
        
        // Καθορισμός τρόπος υπολογισμού του καταχωρητή ADERR
        // ADERR = ADRES - ADSTPT
        ADCON3bits.CALC = 1;
        
        // Καθορισμός τιμής καταχωρητή ADSTPT
        ADSTPT = 0;
        
        // Καθορισμός άνω κατωφλίου
        ADUTH = 2047;
        
        // Καθορισμός κατάστασης που θα σηματοδοτήσει τη διακοπή του bit ADTIF
        // Διακοπή όταν ADERR > ADUTH
        ADCON3bits.TMD = 6;
        
        // Καθορισμός ακροδέκτη RA0 ως η αναλογική είδοσος που θα
        // εφαρμοστεί στον μετατροπέα αναλογικού σήματος σε ψηφιακό 
        ADPCH = 0;
        
        // Εκκίνηση μετατροπής αναλογικού σήματος σε ψηφιακό
        ADCON0bits.GO = 1;
        
        // Αναμονή μέχρι την ολοκλήρωση της μετατροπής
        // αναλογικού σήματος σε ψηφιακό
        while(ADCON0bits.GO == 1);
        
        // Εκτύπωση καταχωρητών μετατροπέα
        printf("ADC operation\n\r");
        printf("ADRES  = %d\n\r", ADRES);
        printf("ADPREV = %d\n\r", ADPREV);
        printf("ADERR = %d\n\r", ADERR);
        // Εκτύπωση κατάστασης ακροδέκτη RD0
        printf("RD0 = %d\n\n\r", PORTDbits.RD0);
        
        // Τέλος διαδικασίας απλής μετατροπής αναλογικού σήματος σε ψηφιακό
        
        //---------------------------------------------------------------//
        
        // Αρχή διαδικασίας χρήσης της διαφορικής μέτρησης για ανίχνευση
        // μεταβολής χωρητικότητας (CVD)
        
        // Ενεργοποίηση δύο μετατροπών
        ADCON1bits.DSEN = 1;
        
        // Καθορισμός τρόπος υπολογισμού του καταχωρητή ADERR
        // ADERR = ADRES - ADPREV
        ADCON3bits.CALC = 0;
        
        // Καθορισμός κατάστασης που θα σηματοδοτήσει τη διακοπή του bit ADTIF
        // Καμία διακοπή
        ADCON3bits.TMD = 0;
        
        // Καθορισμός ακροδέκτη RA1 ως η αναλογική είδοσος που θα
        // εφαρμοστεί στον μετατροπέα αναλογικού σήματος σε ψηφιακό 
        ADPCH = 1;
        
        // Εκκίνηση 1ης μετατροπής αναλογικού σήματος σε ψηφιακό
        ADCON0bits.GO = 1;
        // Αναμονή μέχρι την ολοκλήρωση της μετατροπής
        // του αναλογικού σήματος σε ψηφιακό
        while(ADCON0bits.GO == 1);
        
        // Εκκίνηση 2ης μετατροπής αναλογικού σήματος σε ψηφιακό
        ADCON0bits.GO = 1;
        // Αναμονή μέχρι την ολοκλήρωση της μετατροπής
        // του αναλογικού σήματος σε ψηφιακό
        while(ADCON0bits.GO == 1);
        
        // Καθυστέρηση μέχρι να υπολογιστεί η τιμή του καταχωρητή ADERR
        __delay_us(5);
        // Αν η διαφορά είναι πάνω από ένα κατώφλι αυτό σημαίνει ότι έχει
        // πατηθεί το πλήκτρο αφής, οπότε να μηδενιστεί ο ακροδέκτης RD0
        if (((int16_t)ADERR - DVpre) > 20) LATDbits.LATD0 = 0;
        DVpre = (int16_t)ADERR;
        
        // Εκτύπωση καταχωρητών μετατροπέα
        printf("CVD operation\n\r");
        printf("ADRES = %d\n\r", ADRES);
        printf("ADPREV = %d\n\r", ADPREV);
        printf("ADERR = %d\n\r", ADERR);
        // Εκτύπωση κατάστασης ακροδέκτη RD0
        printf("RD0 = %d\n\n\r", PORTDbits.RD0);
        
        // Τέλος διαδικασίας χρήσης της διαφορικής μέτρησης για ανίχνευση
        // μεταβολής χωρητικότητας (CVD)
    }
    return;
}