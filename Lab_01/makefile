## makefile
## ---------------------------------------------------------------------------
## Build script – STM32 Nucleo-F411RE, bare-metal C con CMSIS.
##
##   make build   → compila y enlaza   → build/app.elf
##   make flash   → programa vía OpenOCD
##   make clean   → elimina objetos y ELF
## ---------------------------------------------------------------------------

include sources.mk

## ── Salida ──────────────────────────────────────────────────────────────────
EXEC = build/app.elf
OBJS = $(SRC:.c=.o)

## ── Linker script ───────────────────────────────────────────────────────────
LINKER_FILE = linker/stm32411re.ld

## ── Target ──────────────────────────────────────────────────────────────────
CPU   = cortex-m4
ARCH  = armv7e-m
FPU   = fpv4-sp-d16
SPECS = nosys.specs

ARCH_FLAGS = -mcpu=$(CPU) -mthumb -march=$(ARCH) -mfloat-abi=hard -mfpu=$(FPU) --specs=$(SPECS)

## ── Flags ───────────────────────────────────────────────────────────────────
CFLAGS  = -g -O0 -std=c99 -Werror -Wall -DSTM32F411xE $(ARCH_FLAGS)
LDFLAGS = -nostdlib -T $(LINKER_FILE)

CC = arm-none-eabi-gcc

## ── Reglas ──────────────────────────────────────────────────────────────────
%.o : %.c
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@

.PHONY: build
build: $(EXEC)
	rm -f $(OBJS)

$(EXEC): $(OBJS)
	@mkdir -p build
	$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@

.PHONY: flash
flash:
	openocd -f board/st_nucleo_f4.cfg \
	        -c "program $(EXEC) verify reset" \
	        -c shutdown

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
