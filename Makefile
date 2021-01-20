# GADGETANGEL to compile the code you must use a Linux machine with  gcc-arm-none-eabi installed
# GADGETANGEL When I ran $make command I got the following message: arm-none-eabi-gcc: not found
# GADGETANGEL my machine is Linux MINT 19.3 which is a Debian derivatives of Linux so I had to install
# GADGETANGEL gcc-arm-none-eabi by excuting the following command: apt install build-essential gcc-arm-none-eabi
# GADGETANGEL once gcc compiler was installed all I needed to do to compile was execute: $make or $make clean or $make all
#
#

#AD: moved to config.h
#APPBAUD  = 115200

PROJECT  = DFU-Bootloader

CONSOLE  = /dev/ttyUSB0

CSRC     = $(wildcard *.c)
CXXSRC   = $(wildcard *.cpp)
ASRC     = $(wildcard *.S)

SUBDIRS  = Drivers Core

INC      = . $(shell find */ -type d)

LIBRARIES =

OUTDIR   = build
OUTBINDIR= bootloader_bin

OSRC     =

NXPSRC   = $(shell find CMSISv2p00_LPC17xx/ LPC17xxLib/ -name '*.c')
NXPO     = $(patsubst %.c,$(OUTDIR)/%.o,$(notdir $(NXPSRC))) $(OUTDIR)/system_LPC17xx.o

FATFSSRC = $(shell find fatfs/ -name '*.c')
FATFSO   = $(patsubst %.c,$(OUTDIR)/%.o,$(notdir $(FATFSSRC)))

CHIP     = lpc1768
MCU      = cortex-m3

ARCH     = arm-none-eabi
PREFIX   = $(ARCH)-

CC       = $(PREFIX)gcc
# CXX      = $(PREFIX)g++
OBJCOPY  = $(PREFIX)objcopy
OBJDUMP  = $(PREFIX)objdump
AR       = $(PREFIX)ar
SIZE     = $(PREFIX)size
READELF  = $(PREFIX)readelf

# You MUST link with G++ if you have even one C++ source file in the project
# If you have no C++, then feel free to link with gcc which gives a significant reduction in included library code
LINK     = $(PREFIX)gcc

MKDIR    = mkdir
RMDIR    = rmdir
RM       = rm -f
CP       = cp -f

OPTIMIZE = s

#DEBUG_MESSAGES
CDEFS    = MAX_URI_LENGTH=512 __LPC17XX__ USB_DEVICE_ONLY

FLAGS    = -O$(OPTIMIZE) -mcpu=$(MCU) -mthumb -mthumb-interwork -mlong-calls -ffunction-sections -fdata-sections -Wall -g -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wno-address-of-packed-member -Wno-unused-but-set-variable
FLAGS   += $(patsubst %,-I%,$(INC))
FLAGS   += $(patsubst %,-D%,$(CDEFS))
CFLAGS   = $(FLAGS) -std=gnu99 -pipe -fno-builtin-printf -fno-builtin-fprintf -fno-builtin-vfprintf -fno-builtin-puts
ASFLAGS  = $(FLAGS)
CXXFLAGS = $(FLAGS) -fno-rtti -fno-exceptions -std=gnu++0x

LDFLAGS  = $(FLAGS) -Wl,--as-needed,--gc-sections,-e,__cs3_reset_cortex_m,-T,$(CHIP).ld
LDFLAGS += $(patsubst %,-L%,$(LIBRARIES)) -lc
LDFLAGS += -Wl,-Map=$(OUTDIR)/$(PROJECT).map

OBJ      = $(patsubst %,$(OUTDIR)/%,$(notdir $(CSRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)))

VPATH    = . $(patsubst %/inc,%/src,$(INC)) $(dir $(NXPSRC)) $(dir $(USBSRC)) $(dir $(UIPSRC)) $(dir $(LWIPSRC))

.PHONY: all clean program upload size functions functionsizes

.PRECIOUS: $(OBJ)

all: $(OUTDIR) $(OUTBINDIR) $(OUTDIR)/nxp.ar $(OUTDIR)/fatfs.ar $(OUTDIR)/$(PROJECT).elf $(OUTBINDIR)/$(PROJECT).bin $(OUTDIR)/$(PROJECT).hex size

clean:
	@echo "  RM    " ".o"
	@$(RM) $(OBJ) $(OBJ:%.o=%.lst)

	@echo "  RM    " "nxp"
	@$(RM) $(NXPO) $(NXPO:%.o=%.lst) $(OUTDIR)/nxp.ar

	@echo "  RM    " "fatfs"
	@$(RM) $(FATFSO) $(FATFSO:%.o=%.lst) $(OUTDIR)/fatfs.ar

	@echo "  RM    " "build/"$(PROJECT)".*"
	@$(RM) $(OUTDIR)/$(PROJECT).bin $(OUTDIR)/$(PROJECT).hex $(OUTDIR)/$(PROJECT).elf $(OUTDIR)/$(PROJECT).map

	@echo "  RM    " "build/"
	@$(RMDIR) $(OUTDIR); true

program: $(OUTDIR)/$(PROJECT).bin
	openocd -f bttskr/lpc17xx-stlink-v2.cfg -c "flash write_image erase $(OUTDIR)/$(PROJECT).bin 0x0" -c "reset run" -c "exit"

hex: $(OUTDIR)/$(PROJECT).bin $(OUTBINDIR)
	objcopy -I binary -O ihex $(OUTDIR)/$(PROJECT).bin $(OUTBINDIR)/$(PROJECT).hex

	
upload: program

flash:	program

console:
	@stty raw ignbrk -echo $(APPBAUD) < $(CONSOLE)
	@echo "Press ctrl+D to exit"
	@( cat <&3 & cat >&3 ; kill %% ) 3<>$(CONSOLE)


# size: $(OUTDIR)/$(PROJECT).elf
# 	@$(SIZE) $<
size: $(OUTDIR)/$(PROJECT).elf
	@echo
	@echo $$'           \033[1;4m  SIZE        LPC1769         (bootloader)\033[0m'
	@$(OBJDUMP) -h $^ | perl -MPOSIX -ne '/.(text|rodata)\s+([0-9a-f]+)/ && do { $$a += eval "0x$$2" }; END { printf "  FLASH    %6d bytes  %2d%% of %3dkb    %2d%% of %3dkb\n", $$a, ceil($$a * 100 / (512 * 1024)), 512, ceil($$a * 100 / (16 * 1024)), 16 }'
	@$(OBJDUMP) -h $^ | perl -MPOSIX -ne '/.(data|bss)\s+([0-9a-f]+)/    && do { $$a += eval "0x$$2" }; END { printf "  RAM      %6d bytes  %2d%% of %3dkb\n", $$a, ceil($$a * 100 / ( 16 * 1024)),  16 }'

functions: $(OUTDIR)/$(PROJECT).elf
	@$(READELF) -s $^ | perl -e 'for (<>) { /^\s+(\d+):\s*([0-9A-F]+)\s+(\d+)/i && do { s/^\s+//; push @symbols, [ split /\s+/, $$_ ]; }; }; for (sort { hex($$a->[1]) <=> hex($$b->[1]); } @symbols) { printf "0x%08s: [%4d] %7s %s\n", $$_->[1], $$_->[2], $$_->[3], $$_->[7] if ($$_->[2]) && (hex($$_->[1]) < 0x10000000); }'

functionsizes: $(OUTDIR)/$(PROJECT).elf
	@$(READELF) -s $^ | perl -e 'for (<>) { /^\s+(\d+):\s*([0-9A-F]+)\s+(\d+)/i && do { s/^\s+//; push @symbols, [ split /\s+/, $$_ ]; }; }; for (sort { $$a->[2] <=> $$b->[2]; } @symbols) { printf "0x%08s: [%4d] %7s %s\n", $$_->[1], $$_->[2], $$_->[3], $$_->[7] if ($$_->[2]) && (hex($$_->[1]) < 0x10000000); }'

$(OUTDIR):
	@$(MKDIR) $(OUTDIR)

$(OUTDIR)/%.bin: $(OUTDIR)/%.elf
	@echo "  COPY  " $@
	@$(OBJCOPY) -O binary $< $@

$(OUTBINDIR)/%.bin: $(OUTDIR)/%.bin
	@echo "  COPY  " $@
	@$(CP)  $< $@

$(OUTDIR)/%.hex: $(OUTDIR)/%.elf
	@echo "  COPY  " $@
	@$(OBJCOPY) -O ihex $< $@

$(OUTDIR)/%.sym: $(OUTDIR)/%.elf
	@echo "  SYM   " $@
	@$(OBJDUMP) -t $< | perl -ne 'BEGIN { printf "%6s  %-40s %s\n", "ADDR","NAME","SIZE"; } /([0-9a-f]+)\s+(\w+)\s+O\s+\.(bss|data)\s+([0-9a-f]+)\s+(\w+)/ && printf "0x%04x  %-40s +%d\n", eval("0x$$1") & 0xFFFF, $$5, eval("0x$$4")' | sort -k1 > $@

$(OUTDIR)/%.elf: $(OBJ) $(OUTDIR)/nxp.ar $(OUTDIR)/fatfs.ar
	@echo "  LINK  " $@
	@$(LINK) $(OSRC) -Wl,-Map=$(@:.elf=.map) -o $@ $^ $(LDFLAGS)

$(OUTDIR)/%.o: %.c Makefile macros.h config.h
	@echo "  CC    " $@
	@$(CC) $(CFLAGS) -Wa,-adhlns=$(@:.o=.lst) -c -o $@ $<

# $(OUTDIR)/%.o: %.cpp
# 	@echo "  CXX   " $@
# 	@$(CXX) $(CXXFLAGS) -Wa,-adhlns=$(@:.o=.lst) -c -o $@ $<

$(OUTDIR)/%.o: %.S Makefile
	@echo "  AS    " $@
	@$(CC) $(ASFLAGS) -Wa,-adhlns=$(@:.o=.lst) -c -o $@ $<

$(OUTDIR)/nxp.ar: $(NXPO)
	@echo "  AR    " "  nxp/"$@
	@$(AR) cru $@ $^

$(OUTDIR)/fatfs.ar: $(FATFSO)
	@echo "  AR    " "fatfs/"$@
	@$(AR) cru $@ $^
