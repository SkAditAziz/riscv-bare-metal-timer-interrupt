#include "timer.h"

void mtimer_set_raw_time_cmp(uint64_t clock_offset) {
    uint64_t new_mtimecmp = mtimer_get_raw_time() + clock_offset;
    
    volatile uint64_t *mtimecmp = (volatile uint64_t*)(RISCV_MTIMECMP_ADDR);
    *mtimecmp = new_mtimecmp;
}
 
uint64_t mtimer_get_raw_time(void) {
    volatile uint64_t *mtime = (volatile uint64_t *)(RISCV_MTIME_ADDR);
    return *mtime;
}

