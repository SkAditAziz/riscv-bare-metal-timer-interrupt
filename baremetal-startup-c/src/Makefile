GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
MARCH=rv64ima
MABI=lp64
SPIKE=/tools/experimental/riscv-isa-sim-commitlog/bin/spike
SPIKEFLAGE=--log-commits -l -m0x80000000

CFLAGS= -march=$(MARCH) -mabi=$(MABI) -mcmodel=medany -std=c99 -Og -Wall -ffunction-sections
LDFLAGS= $(CFLAGS) -nostartfiles -T linker.lds -Wl,-Map=main.map
    
all:main.c.o startup.c.o timer.c.o #main.o.dump startup.o.dump main.elf
dump:main.o.dump startup.o.dump timer.o.dump main.dump
elf:main.elf
asm:main.s
log:main.log

main.c.o:main.c
        $(GCC) $(CFLAGS) -o $@ -c $^

startup.c.o:startup.c
        $(GCC) $(CFLAGS) -o $@ -c $^

timer.c.o:timer.c
        $(GCC) $(CFLAGS) -o $@ -c $^

main.elf:main.c.o timer.c.o startup.c.o
        $(GCC) $(LDFLAGS) $^ -o $@

main.o.dump:main.c.o
        $(OBJDUMP) -D $^ > $@

startup.o.dump:startup.c.o
        $(OBJDUMP) -D $^ > $@

timer.o.dump:timer.c.o
        $(OBJDUMP) -D $^ > $@

main.dump:main.elf
        $(OBJDUMP) -D $^ > $@

main.s:main.elf
        $(OBJDUMP) -S $^ > $@

main.log:main.elf
        $(SPIKE) --isa=rv64gv $(SPIKEFLAGE) $^ &> $@

clean:
        rm -rf *.o *.elf *.dump *.map *.i *.s *.log
        find . -perm /100 -type f -delete

