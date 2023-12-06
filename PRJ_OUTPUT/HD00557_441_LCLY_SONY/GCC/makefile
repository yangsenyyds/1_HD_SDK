ifeq ($(OS),Windows_NT)
GCC_ARM_PATH = D:\Environment\RISC-V\riscv32-unknown-elf-gcc-8.3.0_rv32imafc_ilp32f_newlib
else
GCC_ARM_PATH = /usr/gcc-arm-none-eabi-7-2018-q2-update
endif
CROSS_COMPILE := $(GCC_ARM_PATH)/bin/riscv32-unknown-elf

######cross compiler###########################
#           C Compiler
CC          :=  $(CROSS_COMPILE)-gcc
#           Preprocessor
CCP         :=  $(CROSS_COMPILE)-g++
#           Assembler
AS          :=  $(CROSS_COMPILE)-as
#           Linker
LD          :=  $(CROSS_COMPILE)-ld
OBJDUMP     :=  $(CROSS_COMPILE)-objdump
OBJCOPY     :=  $(CROSS_COMPILE)-objcopy
SIZE        :=  $(CROSS_COMPILE)-size
##############################################

OUTPUT_DIR := ./output
OBJ_DIR = $(OUTPUT_DIR)/obj

COMMONPAMS := -march=rv32imafc 
COMMONPAMS += -mabi=ilp32f 
COMMONPAMS += -msmall-data-limit=0 
COMMONPAMS += -mstrict-align 
COMMONPAMS += -msave-restore 
COMMONPAMS += -fmessage-length=0 
COMMONPAMS += -fsigned-char 
COMMONPAMS += -ffunction-sections 
COMMONPAMS += -fdata-sections  
COMMONPAMS += -g

LINKFILE := -T gcc.ld

LINKPARAMS := -nostartfiles 
LINKPARAMS += -Xlinker 
LINKPARAMS += --gc-sections 
LINKPARAMS += -u main

LINK_MAP := -Wl,-Map,$(OUTPUT_DIR)

ASMPARAMS := -x assembler-with-cpp
ASMPARAMS += -MMD
ASMPARAMS += -MP

CCPARAMS := -DFIXED_POINT=1 
CCPARAMS += -DDISABLE_FLOAT_API
CCPARAMS += -DOPUS_BULLD
CCPARAMS += -std=gnu11 
CCPARAMS += -fno-toplevel-reorder
CCPARAMS += -MMD 
CCPARAMS += -MP

SDK_ROOT = ./../../../Platform
APP_ROOT = ./../../../APP
ENCODE_ROOT = $(APP_ROOT)/audio/encode/adpcm
MAIN_DIR = ./../../../Vendor/Remote_Control
PRJ_DIR = ./../../../Projects/HD00557


VPATH := $(SDK_ROOT)/core
VPATH += $(SDK_ROOT)/sdk
VPATH += $(SDK_ROOT)/sdk/bt
VPATH += $(SDK_ROOT)/startup
VPATH += $(OUTPUT_DIR)

SRCINCLUDE := -I. 					\
	-I$(GCC_ARM_PATH)/riscv32-unknown-elf/include \
	-I$(GCC_ARM_PATH)/lib/gcc/riscv32-unknown-elf/8.3.0/include \
	-I$(GCC_ARM_PATH)/lib/gcc/riscv32-unknown-elf/8.3.0/include-fixd \
	-I$(SDK_ROOT)/core \
	-I$(SDK_ROOT)/sdk \
	-I$(SDK_ROOT)/sdk/bt \
	-I$(SDK_ROOT)/startup \


VPATH += $(MAIN_DIR)
VPATH += $(PRJ_DIR)
VPATH += $(SDK_ROOT)/devices/ble/att
VPATH += $(SDK_ROOT)/devices/ble/hci
VPATH += $(SDK_ROOT)/devices/bt
VPATH += $(SDK_ROOT)/devices/flash
VPATH += $(SDK_ROOT)/devices/ota
VPATH += $(SDK_ROOT)/devices/dtm
VPATH += $(APP_ROOT)/app_queue
VPATH += $(APP_ROOT)/app_sleep
VPATH += $(APP_ROOT)/software_timer
VPATH += $(APP_ROOT)/malloc
VPATH += $(APP_ROOT)/power
VPATH += $(APP_ROOT)/key
VPATH += $(APP_ROOT)/led
VPATH += $(APP_ROOT)/flash/YC
VPATH += $(APP_ROOT)/ir/ir_send
VPATH += $(APP_ROOT)/ir/ir_lib
VPATH += $(APP_ROOT)/audio
VPATH += $(APP_ROOT)/audio/encode/adpcm
VPATH += $(APP_ROOT)/factory

SRCINCLUDE += -I$(MAIN_DIR)
SRCINCLUDE += -I$(PRJ_DIR)
SRCINCLUDE += -I$(SDK_ROOT)/devices/ble/hci
SRCINCLUDE += -I$(SDK_ROOT)/devices/ble/att
SRCINCLUDE += -I$(SDK_ROOT)/devices/bt
SRCINCLUDE += -I$(SDK_ROOT)/devices/flash
SRCINCLUDE += -I$(SDK_ROOT)/devices/ota
SRCINCLUDE += -I$(SDK_ROOT)/devices/dtm
SRCINCLUDE += -I$(APP_ROOT)/SECRETKEY
SRCINCLUDE += -I$(APP_ROOT)/app_queue
SRCINCLUDE += -I$(APP_ROOT)/app_sleep
SRCINCLUDE += -I$(APP_ROOT)/software_timer
SRCINCLUDE += -I$(APP_ROOT)/malloc
SRCINCLUDE += -I$(APP_ROOT)/power
SRCINCLUDE += -I$(APP_ROOT)/key
SRCINCLUDE += -I$(APP_ROOT)/led
SRCINCLUDE += -I$(APP_ROOT)/flash/YC
SRCINCLUDE += -I$(APP_ROOT)/ir/ir_send
SRCINCLUDE += -I$(APP_ROOT)/ir/ir_lib
SRCINCLUDE += -I$(APP_ROOT)/audio
SRCINCLUDE += -I$(APP_ROOT)/audio/encode/adpcm
SRCINCLUDE += -I$(APP_ROOT)/factory

CORE := \
	main.c \
	yc_debug.c \
	yc_nvic.c \
	yc11xx.c \
	malloc.c \
	software_timer.c \

SDK := \
	yc11xx_bt_interface.c \
	yc11xx_ipc.c \
	yc11xx_audio_adc.c \
	yc11xx_gpio.c \
	yc11xx_iwdg.c \
	yc11xx_qspi.c \
	yc11xx_qspi0.c \
	yc11xx_spi.c \
	yc11xx_systick.c \
	yc11xx_timer.c \
	yc11xx_uart.c \
	yc11xx_lpm.c \
	yc11xx_pwm.c \
	# yc11xx_iic.c \

APP := \
	yc11xx_dev_bt_aap.c \
	yc11xx_dev_bt_data_trans.c \
	yc11xx_dev_qspi.c \
	yc_dev_bt.c \
	yc_ota.c \
	hci_dtm_test.c \
	att.c \
	flash.c \
	app_queue.c \
	app_sleep.c \
	vbat.c \
	keyscan.c \
	led.c \
	ir_send.c \
	ir_load.c \
	ir_lib.c \
	audio.c \
	factory_test_off_line.c \
	$(wildcard $(APP_ROOT)/audio/encode/adpcm/*.c) \

APP_DIR := $(notdir $(APP))

RMC := remote_control.c
RMC += att_list.c

# .c to .o
STARTUP_OBJ := $(OBJ_DIR)/startup_rv32.o

ALL_C_OBJ := $(patsubst %.c, $(OBJ_DIR)/%.o, $(CORE))
ALL_C_OBJ += $(patsubst %.c, $(OBJ_DIR)/%.o, $(SDK))
ALL_C_OBJ += $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_DIR))
ALL_C_OBJ += $(patsubst %.c, $(OBJ_DIR)/%.o, $(RMC))

$(STARTUP_OBJ) : startup_rv32.s
	$(CC) $(COMMONPAMS) $(INCLUDES) $(ASMPARAMS) -Os -c -o $@ $^

$(ALL_C_OBJ) : $(OBJ_DIR)/%.o : %.c
	$(CC) $(COMMONPAMS) $(SRCINCLUDE) $(CCPARAMS) -Os -c -o $@ $^


out : $(ALL_C_OBJ) $(STARTUP_OBJ)
	@echo Start Build Image.
	@$(CC)  $(COMMONPAMS) $(LINKFILE) $(LINKPARAMS) $(LINK_MAP)/$@.map -Os -o $(OUTPUT_DIR)/$@.elf $^ -L $(APP_ROOT)/SECRETKEY -lSecretKey
	$(OBJCOPY) -O ihex $(OUTPUT_DIR)/$@.elf $(OUTPUT_DIR)/$@.hex
	$(OBJDUMP) --source --all-headers --demangle --line-numbers --wide $(OUTPUT_DIR)/$@.elf > $(OUTPUT_DIR)/$@.lst
	@echo GNU RISC-V Cross Print Size
	@$(SIZE) --format=berkeley $(OUTPUT_DIR)/$@.elf


MAKE_TARGET := out

all: $(MAKE_TARGET)


clean :
ifeq ($(OS),Windows_NT)
	del /Q /S output\*
else
	rm -rf output/*
endif
