HOST := arm-none-eabi
CC := $(HOST)-gcc
LD := $(HOST)-ld
AS := $(HOST)-as
AR := $(HOST)-ar
OBJCOPY := $(HOST)-objcopy
OBJDUMP := $(HOST)-objdump


CFLAGS := -g -Wall -fstack-usage
CFLAGS := -mcpu=cortex-m4 -mlittle-endian -mthumb $(CFLAGS)
CFLAGS := -Iinclude -DSTM32F303xC $(CFLAGS)

TARGET = proj1

src_c = \
	src/system_stm32f3xx.c \
	src/main.c \
	src/led.c \
	src/btn.c \
	src/sys.c

obj_c = $(foreach f, $(src_c), Debug/$(basename $(f)).o)

src_asm = src/startup_stm32f303xc.s

obj_asm = $(foreach f, $(src_asm), Debug/$(basename $(f)).o)


ldscript = STM32F303XC_FLASH.ld

GIT_REV := $(shell git rev-parse HEAD)

all: git_rev $(TARGET).hex $(TARGET).list

list: $(TARGET).list

check-syntax: $(TARGET).elf

.PHONY: git_rev

git_rev:
	sh -c "if [ X$(GIT_REV) != X$(shell cat GIT_REV) ]; then \
	    echo $(GIT_REV) > GIT_REV; \
	    echo 'char *git_rev = \"'$(GIT_REV)'\";' > src/git_rev.c; \
	fi"

$(TARGET).hex : $(TARGET).elf
	$(OBJCOPY) $< -O ihex $@

$(TARGET).list : %.list : %.elf
	$(OBJDUMP) -D $< > $@

$(TARGET).elf : %.elf : $(obj_c) $(obj_asm) $(ldscript)
	$(CC) $(CFLAGS) -Wl,-T$(ldscript) \
	-Wl,-Map,$(TARGET).map \
	-Wl,--gc-sections \
	$(obj_c) \
	$(obj_asm) \
	-lm -o $@

# NOTE, use abspath to remove trail '/' on windows
$(obj_c) : Debug/%.o : %.c
	sh -c "mkdir -p -- $(abspath $(dir $@))"
	$(CC) -MD $(CFLAGS) -c $< -o $@

$(obj_asm) : Debug/%.o : %.s
	$(CC) $(CFLAGS) -c $< -o $@

-include $(obj_src:%.c=Debug/%.d)

clean:
	rm -rf *.o *.elf *.map *.list *.out *.bin *.su Debug/*
