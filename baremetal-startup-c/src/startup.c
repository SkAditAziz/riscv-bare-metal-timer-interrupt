#include <stdint.h>
#include <string.h>

extern void *bss_target_start;
extern void *bss_target_end;
extern const void *data_source_start;
extern void *data_target_start;
extern void *data_target_end;

extern void _enter(void)  __attribute__ ((naked, section(".text.metal.init.enter")));

extern void _start(void);

extern int main(void);

void _enter(void) {
    __asm__ volatile  (
        ".option push;"
         ".option norelax;"
        "la    gp, __global_pointer$;"
        ".option pop;"
        "la    sp, _sp;"
        "jal   zero, _start;"
        :  /* output: none %0 */
        : /* input: none */
        : /* clobbers: none */);
}

void _start(void){

    //setting .bss to 0
    uint64_t bss_size = &bss_target_end - &bss_target_start;

    uint64_t *byteTrg = (uint64_t*) &bss_target_start;

    for (uint64_t i=0; i<bss_size; i++){
          *byteTrg++ = 0;
    }

    //copying .data from rom 
    uint64_t data_size = &data_target_end - &data_target_start;
    uint64_t *dataTrg = (uint64_t*) &data_target_start;
    uint64_t *dataSrc = (uint64_t*) &data_source_start;

    for (uint64_t i=0; i<data_size; i++){
          *dataTrg++ = *dataSrc++;
    }

    main();

}

