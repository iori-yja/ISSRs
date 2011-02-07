# Hey Emacs, this is a -*- makefile -*-
#
# SOURCERY template makefile 
# by Martin Thomas, Kaiserslautern, Germany 
# <eversmith@heizung-thomas.de>
#
# based on the WinAVR makefile written by Eric B. Weddington, Jorg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean TARGET built project files.
#
# make program = Download the hex file to the device
#
# (TODO: make filename.s = Just compile filename.c into the assembler code only)
#
# To rebuild project do "make clean" then "make all".
#
# Changelog:
# - 17. Feb. 2005  - added thumb-interwork support (mth)
# - 28. Apr. 2005  - added C++ support (mth)
# - 29. Arp. 2005  - changed handling for lst-Filename (mth)
# -  1. Nov. 2005  - exception-vector placement options (mth)
# - 15. Nov. 2005  - added library-search-path (EXTRA_LIB...) (mth)
# -  2. Dec. 2005  - fixed ihex and binary file extensions (mth)
# - 22. Feb. 2006  - added AT91LIBNOWARN setting (mth)
# - 19. Apr. 2006  - option FLASH_TOOL (default lpc21isp); variable IMGEXT (mth)
# - 23. Jun. 2006  - option USE_THUMB_MODE -> THUMB/THUMB_IW
# -  3. Aug. 2006  - added -ffunction-sections -fdata-sections to CFLAGS
#                    and --gc-sections to LDFLAGS. Only available for gcc 4 (mth)
# -  4. Aug. 2006  - pass SUBMDL-define to frontend (mth)
# - 11. Nov. 2006  - FLASH_TOOL-config, TCHAIN-config (mth)
# - 28. Mar. 2007  - remove .dep-Directory with rm -r -f and force "no error"
# - 24. Aprl 2007  - added "both" option for format (.bin and .hex)
# - 15. Aprl 2009  - Changed Nemui's style
# - 27. oct. 2009  - Fixed FreeRTOS consideration.(nemui)
# -  7. Jan. 2010  - Rewrited to able to use on GNU/Linux with arm-none-eabi-gcc(iorivr)
##########################################################################################################
#export PATH = %SYSTEMROOT%;$(TOOLDIR)/arm-gcc/bin;$(TOOLDIR)/bin;$(MAKEDIR);$(OCDIR)

# Toolchain prefix (i.e arm-elf -> arm-elf-gcc.exe)
TCHAIN  = arm-none-eabi
OCD		= openocd

#DEVTOOL = RAISONANCE
#DEVTOOL = SOURCERY
#ifeq ($(DEVTOOL),RAISONANCE)
#TOOLDIR = C:/Devz/ARM/Raisonance/Ride
REMOVAL = rm
#else
#TOOLDIR = C:/Devz/ARM/Sourcery
#REMOVAL = rm
#endif

#MAKEDIR = C:/Devz/AVR/WinAVR/utils/bin
#OCDIR	= C:/Devz/ARM/OCD
#OCD_CMD = -f $(OCDIR)/daemon.cfg \
#          -f $(OCDIR)/tcl/interface/jtagkey.cfg \
#          -f $(OCDIR)/tcl/target/lpc2388_flash.cfg 

# MCU name and submodel
MCU      = arm7tdmi-s
SUBMDL   = LPC2388
USE_THUMB_MODE = YES

# Board definition
#USE_DISPLAY		= USE_ST7735_TFT
#USE_DISPLAY		= USE_SED1339_OLED
# If you are using port B on the LPC2378 uncomment TARGET the next line (Used on the Olimex 2378 Dev Board)
# LPCUSB LIBRARY special setting 
MCU_DEF = LPC23xx
LPC2378_PORT = LPC2378_PORTB


# TARGET definition
TARGET = main

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS =  . 					\
				./inc				\
				$(USBLIB)/inc		\
				$(FATFSLIB)/inc		\
				$(COMMON)/inc		\
				$(RTOS_SOURCE_DIR)/include \
				$(RTOS_SOURCE_DIR)/portable/GCC/ARM7_LPC23xx \
				$(DEMO_INCLUDE_DIR) \
				$(WEBLIB) 			\
				$(UIPLIB) 			\

# List C source files (THUMB INTERWORK) here.
# use file-extension c for "c-only"-files
THUMB_SOURCE = \
		./src/$(TARGET).c		        \

#/*----- library PATH -----*/
# define COMMON LIBRARY PATH
LIBDIR = ./lib
COMMON = $(LIBDIR)/Common
THUMB_SOURCE += \
		$(COMMON)/src/irq.c				\
		$(COMMON)/src/rtc.c				\
		$(COMMON)/src/uart.c			\
		$(COMMON)/src/fio.c				\
		$(COMMON)/src/misc.c			\
		$(COMMON)/src/target.c			\
		$(COMMON)/src/syscalls.c		\
#		$(COMMON)/src/camerareset.c		\
		$(COMMON)/src/adc.c		
#		$(COMMON)/src/Italia.c
		
# define USB LIBRARY PATH
#USBLIB = $(LIBDIR)/UsbLib
#THUMB_SOURCE += \
#		$(USBLIB)/src/usbcontrol.c 		\
#		$(USBLIB)/src/usbhw_lpc.c 		\
#		$(USBLIB)/src/usbinit.c 		\
#		$(USBLIB)/src/usbstdreq.c

# define uIP Application PATH
#WEBLIB = $(LIBDIR)/webserver
#THUMB_SOURCE += \
#		$(WEBLIB)/uIP_Task.c			\
#		$(WEBLIB)/emac.c				\
#		$(WEBLIB)/httpd.c				\
#		$(WEBLIB)/httpd-cgi.c			\
#		$(WEBLIB)/httpd-fs.c			\
#		$(WEBLIB)/http-strings.c
		
# define uIP Basis PATH
#UIPLIB =../Common/ethernet/uIP/uip-1.0/uip
#THUMB_SOURCE += \
#		$(UIPLIB)/uip_arp.c				\
#		$(UIPLIB)/psock.c				\
#		$(UIPLIB)/timer.c				\
#		$(UIPLIB)/uip.c

# define FreeRTOS Function PATH
DEMO_COMMON_DIR  =../Common/Minimal
DEMO_INCLUDE_DIR =../Common/include
THUMB_SOURCE += \
		$(DEMO_COMMON_DIR)/BlockQ.c 	\
		$(DEMO_COMMON_DIR)/blocktim.c 	\
		$(DEMO_COMMON_DIR)/integer.c 	\
		$(DEMO_COMMON_DIR)/GenQTest.c 	\
		$(DEMO_COMMON_DIR)/QPeek.c 		\
		$(DEMO_COMMON_DIR)/dynamic.c

# define FreeRTOS Basis PATH
RTOS_SOURCE_DIR  =../../Source
THUMB_SOURCE += \
		$(RTOS_SOURCE_DIR)/list.c		\
		$(RTOS_SOURCE_DIR)/queue.c		\
		$(RTOS_SOURCE_DIR)/tasks.c		\
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM7_LPC23xx/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_3.c

# define FatFs LIBRARY PATH
#FATFSLIB = $(LIBDIR)/FatFs
#THUMB_SOURCE += \
#		$(FATFSLIB)/src/ff.c 			\
#		$(FATFSLIB)/src/option/cc932.c

LIBPATHS =	$(COMMON) $(USBLIB) $(FATFSLIB) $(UIPLIB) $(WEBLIB) \
			$(RTOS_SOURCE_DIR) $(DEMO_COMMON_DIR)
LIBRARY_DIRS = $(addprefix -L,$(LIBPATHS))

# List C source files here which must be compiled in ARM-Mode.
# use file-extension c for "c-only"-files
ARM_SOURCE= \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM7_LPC23xx/portISR.c \
		$(COMMON)/src/i2c.c			\
#		$(WEBLIB)/EMAC_ISR.c 			\
#		./src/vcom.c					\

THUMB_OBJS = $(THUMB_SOURCE:.c=.o)
ARM_OBJS = $(ARM_SOURCE:.c=.o)

# TOOLCHAIN SETTINGS
CC 			= $(TCHAIN)-gcc
CPP 		= $(TCHAIN)-g++
OBJCOPY 	= $(TCHAIN)-objcopy
OBJDUMP 	= $(TCHAIN)-objdump
SIZE 		= $(TCHAIN)-size
AR 			= $(TCHAIN)-ar
LD 			= $(TCHAIN)-gcc
NM 			= $(TCHAIN)-nm
REMOVE		= $(REMOVAL) -f
REMOVEDIR 	= $(REMOVAL) -rf


# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
OPT = 2

# Debugging format.
# For ARM Insight.
DEBUG = dwarf-2

# Synsesis makefile Defines FreeRTOS special setting
DEFZ =  $(LPC2378_PORT) $(MCU_DEF) $(DEBUG_PRINTF) $(USE_DISPLAY) \
		ROWLEY_LPC23xx \
		THUMB_INTERWORK \
		USE_FreeRTOS_SUPPORT \
		PACK_STRUCT_END=__attribute\(\(packed\)\) \
		ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\)
SYNTHESIS_DEFS	= $(addprefix -D,$(DEFZ))


CDEFS += -D__$(DEVTOOL)__ -D__$(DEVTOOL)SUBMDL_$(SUBMDL)__ $(SYNTHESIS_DEFS) 
ADEFS += -D__$(DEVTOOL)__ -D__$(DEVTOOL)SUBMDL_$(SUBMDL)__ $(SYNTHESIS_DEFS) 

# Compiler flags.

ifeq ($(USE_THUMB_MODE),YES)
THUMB    = -mthumb
THUMB_IW = -mthumb-interwork
else 
THUMB    = 
THUMB_IW = 
endif

#  -g*:          generate debugging information
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#
# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS  = -mcpu=$(MCU) $(THUMB_IW)
CFLAGS += -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fno-common -fomit-frame-pointer
#CFLAGS += -fno-dwarf2-cfi-asm #仮
CFLAGS += -T$(LDSCRIPT) \
#CFLAGS += -Wall -Wcast-align -Wimplicit 
#CFLAGS += -Wpointer-arith -Wswitch
#CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<) 
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))


LDSCRIPT=LPC2388-ROM.ld
LINKER_FLAGS= $(THUMB) -nostartfiles -Xlinker -o$(TARGET).elf -Xlinker -M -Xlinker -Map=$(TARGET).map


all: gccversion sizebefore build sizeafter
build: bin lss sym

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A -x $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo "Size Before"; $(ELFSIZE); echo; fi
sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo "Size After"; $(ELFSIZE); echo; fi

gccversion : 
	@$(CC) --version
	
elf: $(TARGET).elf
lss: $(TARGET).lss 
sym: $(TARGET).sym
bin: $(TARGET).bin

$(TARGET).bin : $(TARGET).hex
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).bin
	 
$(TARGET).hex : $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf -O ihex $(TARGET).hex

$(TARGET).lss : $(TARGET).elf
	$(OBJDUMP) -h -S -C $< > $@

$(TARGET).sym : $(TARGET).elf
	$(NM) -n $< > $@

$(TARGET).elf : $(THUMB_OBJS) $(ARM_OBJS) boot.s Makefile
	$(CC) $(CFLAGS) $(ARM_OBJS) $(THUMB_OBJS) $(LIBS) boot.s $(LINKER_FLAGS)

$(THUMB_OBJS) : %.o : %.c Makefile FreeRTOSConfig.h
	$(CC) -c $(THUMB) $(CFLAGS)  $< -o $@

$(ARM_OBJS) : %.o : %.c Makefile FreeRTOSConfig.h
	$(CC) -c $(CFLAGS) $< -o $@

# Flash and Debug Program
debug :
	$(OCD) $(OCD_CMD)
program :
	$(OCD) $(OCD_CMD) -c "mt_flash $(TARGET).elf"  -c "continue"  -c "resume" -c "shutdown"
#	$(OCD) $(OCD_CMD) -c "eraser"  -c "resume" -c "shutdown"


clean :
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(THUMB_OBJS)
	$(REMOVE) $(ARM_OBJS)
	$(REMOVE) $(COBJ)
	$(REMOVE) $(CPPOBJ)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(COBJARM)
	$(REMOVE) $(CPPOBJARM)
	$(REMOVE) $(AOBJARM)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRCARM:.c=.s)
	$(REMOVE) $(SRCARM:.c=.d)
	$(REMOVE) $(CPPSRC:.cpp=.s) 
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) $(CPPSRCARM:.cpp=.s) 
	$(REMOVE) $(CPPSRCARM:.cpp=.d)
	$(REMOVEDIR) .dep | exit 0

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex bin lss sym clean clean_list program
