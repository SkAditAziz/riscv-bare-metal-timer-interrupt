#include "riscv-csr.h"
#include "riscv-interrupts.h"
#include "timer.h"

static void irq_entry(void) __attribute__ ((interrupt ("machine")));

int interrup_cnt = 0;

int main(void) {

    csr_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK); // csrrc mstatus,0x8
    csr_write_mie(0);   //csrw    mie, 0

    mtimer_set_raw_time_cmp(MTIMER_SECONDS_TO_CLOCKS(1)); // MTIMECMP = MTIME + 1

    csr_write_mtvec((uint_xlen_t) irq_entry); // csrw mtvec, irq_handler addr
    
    csr_set_bits_mie(MIE_MTI_BIT_MASK); //enabling timer int setting no 7 bit

    csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK); //csrrs mstatus,0x8

    do {
        __asm__ volatile ("wfi");
        //use nop
    } while (1);

    return 0;

}

static void irq_entry(void)  {
    uint_xlen_t this_cause = csr_read_mcause();
    if (this_cause &  MCAUSE_INTERRUPT_BIT_MASK) {
        this_cause &= 0xFF;
        switch (this_cause) {
        case RISCV_INT_MASK_MTI :
            mtimer_set_raw_time_cmp(MTIMER_SECONDS_TO_CLOCKS(1));
            interrup_cnt++;// a counter to keep track of how many time int has been called
            break;
        }
    }
}   

