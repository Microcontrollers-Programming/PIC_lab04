#include <xc.h>

// Καθορισμός διεύθυνσης πίνακα ανυσμάτων διακοπών
#define IVT                             0x000008

// Ρουτίνα διαχείρισης διακοπής σύγκρισης με κατώφλι ADTIF
void __interrupt(irq(IRQ_ADT), base(IVT), high_priority) ADTIF_ISR()
{
    // Εκκαθάριση του bit ADTIF
    PIR2bits.ADTIF = 0;
    // Οδήγηση στο λογικό '1' του ακροδέκτη RD0
    LATDbits.LATD0 = 1;
}

// Ρουτίνα διαχείρισης οποιασδήποτε άλλης διακοπής
void __interrupt(irq(default), base(IVT), high_priority) DEFAULT_ISR()
{
    asm("nop");
}