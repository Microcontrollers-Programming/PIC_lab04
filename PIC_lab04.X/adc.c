#include <xc.h>

void ADC_INIT()
{
    // Καθορισμός ακροδεκτών RA0 και RA1 ως αναλογικές είσοδοι
    ANSELAbits.ANSELA0 = 1;
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSELA1 = 1;
    TRISAbits.TRISA1 = 1;
    
    // Επιλογή ρολογιού ADC από τον RC ταλαντωτή χρονισμού
    ADCON0bits.CS = 1;
    
    // Στοίχιση του αποτελέσματος δεξιά
    ADCON0bits.FM = 1;
    
    // Επιλογή θετικής και αρνητικής τάσης αναφοράς
    // Vref+ = VDD
    ADREFbits.PREF = 0;
    // Vref- = VSS
    ADREFbits.NREF = 0;
    
    // Επιλογή χρονικής καθυστέρησης εκφόρτισης πυκνωτή
    // δειγματοληψίας και συγκράτησης (Sample and Hold capacitor)
    // 10 ADCRC clocks
    ADPRE = 10;
    
    // Επιλογή χρονικής καθυστέρησης φόρτισης πυκνωτή
    // δειγματοληψίας και συγκράτησης (Sample and Hold capacitor)
    // 10 ADCRC clocks (10μs-60μs)
    ADACQ = 10;
    
    // Καθορισμός mode λειτουργίας του ADC
    ADCON2bits.MD = 0;
    
    // Ενεργοποίηση μετατροπέα ADC
    ADCON0bits.ON = 1;
    
    // Ενεργοποίηση διακοπής κατωφλίου
    PIR2bits.ADTIF = 0;
    IPR2bits.ADTIP = 1;
    PIE2bits.ADTIE = 1;
}