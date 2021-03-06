#############################################################
# Root Level Makefile
# Version 2.0
# (c) by CHERTS <sleuthhound@gmail.com>
#
# Modified By Ankit bhatnagar
# ankit.bhatnagarindia@gmail.com
#		- uses rboot for bootloader (instead of espressif boot)
#		- flash (irom0) offset modified to 0x2000 instead of 0x1000
#			as rboot using 2 sectors as compared to 1 by espressif
#			supplied
#		- Note : Need to also change the irom0 base address in the
#				 ld files from 0x40201010 to 0x40202010
#
#		USAGE:
#				make BOOT=<none|new|old> APP=<1|2> SPI_SIZE_MAP=<1..6>
#               make flashboot SPI_SIZE_MAP=|1..6| SPI_SPEED=40 SPI_MODE=QIO
#               make flashall SPI_SIZE_MAP=|1..6| SPI_SPEED=40 SPI_MODE=QIO
#               make flasherase
#       
#       TO MAKE VERSION FILE:
#               make version MAJ=|x| MIN=|y|
#               the version file generated is app.ver
#
#		TO BURN:
#				esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash -fs 32m-c1 -fm qio 0x0000 rboot.bin 0x02000 user1.4096.new.6.bin 0x102000 user2.4096.new.6.bin 0x3fc000 esp_init_data_default.bin 0x3fe000 blank.bin
#############################################################

##################################
# PROJECT SPECIFIC OPTIONS
##################################
# Use this area to do custom defines used in my code using #ifdef
PROJECT_DEFINES = ESP8266

##################################
# OUTPUT FOLDERS
##################################
BUILD_BASE	= build
FW_BASE		= firmware

##################################
# COMPILER PATHS
##################################
# Base directory for the compiler
XTENSA_TOOLS_ROOT ?= /media/ankit/mydev/Toolchains/Expressif/esp-open-sdk/xtensa-lx106-elf/bin
# base directory of the ESP8266 SDK package, absolute
SDK_BASE	?= /media/ankit/mydev/Toolchains/Expressif/esp-open-sdk/sdk
SDK_TOOLS	?= $(SDK_BASE)/tools
# esptool path and port
ESPTOOL ?= /media/ankit/mydev/Toolchains/Expressif/esp-open-sdk/esptool/esptool.py

##################################
# FLASH OPTIONS
##################################
ESPPORT ?= /dev/ttyUSB0
BAUD ?= 115200

# BOOT = none
# BOOT = old - boot_v1.1
# BOOT = new - boot_v1.2+
#BOOT?=new
# APP = 0 - eagle.flash.bin + eagle.irom0text.bin
# APP = 1 - user1.bin
# APP = 2 - user2.bin
#APP?=1
# SPI_SPEED = 40, 26, 20, 80
SPI_SPEED ?= 40
# SPI_MODE: QIO, QOUT, DIO, DOUT
SPI_MODE ?= DIO
# SPI_SIZE_MAP
# 0 : 512 KB (256 KB + 256 KB)
# 1 : 256 KB
# 2 : 1024 KB (512 KB + 512 KB)
# 3 : 2048 KB (512 KB + 512 KB)
# 4 : 4096 KB (512 KB + 512 KB)
# 5 : 2048 KB (1024 KB + 1024 KB)
# 6 : 4096 KB (1024 KB + 1024 KB)
#SPI_SIZE_MAP ?= 6

ifeq ($(BOOT), new)
    boot = new
else
    ifeq ($(BOOT), old)
        boot = old
    else
        boot = none
    endif
endif

ifeq ($(APP), 1)
    app = 1
else
    ifeq ($(APP), 2)
        app = 2
    else
        app = 0
    endif
endif

ifeq ($(SPI_SPEED), 26.7)
    freqdiv = 1
    flashimageoptions = -ff 26m
else
    ifeq ($(SPI_SPEED), 20)
        freqdiv = 2
        flashimageoptions = -ff 20m
    else
        ifeq ($(SPI_SPEED), 80)
            freqdiv = 15
            flashimageoptions = -ff 80m
        else
            freqdiv = 0
            flashimageoptions = -ff 40m
        endif
    endif
endif

ifeq ($(SPI_MODE), QOUT)
    mode = 1
    flashimageoptions += -fm qout
else
    ifeq ($(SPI_MODE), DIO)
	mode = 2
	flashimageoptions += -fm dio
    else
        ifeq ($(SPI_MODE), DOUT)
            mode = 3
            flashimageoptions += -fm dout
        else
            mode = 0
            flashimageoptions += -fm qio
        endif
    endif
endif

addr = 0x02000

ifeq ($(SPI_SIZE_MAP), 1)
  size_map = 1
  flash = 256
  flashimageoptions += -fs 2m
else
  ifeq ($(SPI_SIZE_MAP), 2)
    size_map = 2
    flash = 1024
    flashimageoptions += -fs 8m
    ifeq ($(app), 2)
      addr = 0x81000
    endif
  else
    ifeq ($(SPI_SIZE_MAP), 3)
      size_map = 3
      flash = 2048
      flashimageoptions += -fs 16m
      ifeq ($(app), 2)
        addr = 0x81000
      endif
    else
      ifeq ($(SPI_SIZE_MAP), 4)
	size_map = 4
	flash = 4096
	flashimageoptions += -fs 32m
        ifeq ($(app), 2)
          addr = 0x81000
        endif
      else
        ifeq ($(SPI_SIZE_MAP), 5)
          size_map = 5
          flash = 2048
          flashimageoptions += -fs 16m
          ifeq ($(app), 2)
            addr = 0x101000
          endif
        else
          ifeq ($(SPI_SIZE_MAP), 6)
            size_map = 6
            flash = 4096
            flashimageoptions += -fs 32m-c1
            ifeq ($(app), 2)
							# for espressif supplied boot.bin address = 0x101000
							# rboot needs 2 sectors,so address = 0x102000
							addr = 0x102000
            endif
          else
            size_map = 0
            flash = 512
            flashimageoptions += -fs 4m
            ifeq ($(app), 2)
              addr = 0x41000
            endif
          endif
        endif
      endif
    endif
  endif
endif

# name for the target project
TARGET = app

# which modules (subdirectories) of the project to include in compiling
MODULES		= user user/driver user/libs/
MODULES		:= $(MODULES) user/libs/ESP8266_MDNS
MODULES		:= $(MODULES) user/libs/ESP8266_GPIO
MODULES     := $(MODULES) user/libs/ESP8266_SMARTCONFIG
MODULES		:= $(MODULES) user/libs/ESP8266_SYSINFO
MODULES		:= $(MODULES) user/libs/ESP8266_TCP_SERVER
MODULES		:= $(MODULES) user/libs/ESP8266_SSID_FRAMEWORK
MODULES		:= $(MODULES) user/libs/ESP8266_FLASH
MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot
MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot/appcode
MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot/build
MODULES		:= $(MODULES) user/libs/ESP8266_OTA
#MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot-sample
#MODULES		:= $(MODULES) user/libs/ESP8266_SNTP
#MODULES		:= $(MODULES) user/libs/ESP8266_I2C
#MODULES		:= $(MODULES) user/libs/RTC_DS1307
#MODULES		:= $(MODULES) user/libs/ESP8266_TCP_POST
#MODULES		:= $(MODULES) user/libs/EEPROM_AT24CXX

EXTRA_INCDIR = include $(SDK_BASE)/../extra/include

# libraries used in this project, mainly provided by the SDK
LIBS		= c gcc hal pp phy net80211 lwip wpa crypto main smartconfig ssl

# compiler flags using during compilation of source files
CFLAGS = -Os -g -O2 -std=gnu90 -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -mno-serialize-volatile -D__ets__ -DICACHE_FLASH
CFLAGS := $(CFLAGS) -D$(PROJECT_DEFINES)

# linker flags used to generate the main object file
LDFLAGS	= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static

# linker script used for the above linkier step
LD_SCRIPT = eagle.app.v6.ld

ifneq ($(boot), none)
ifneq ($(app),0)
    ifeq ($(size_map), 6)
      LD_SCRIPT = eagle.app.v6.$(boot).1024.app$(app).ld
    else
      ifeq ($(size_map), 5)
        #LD_SCRIPT = eagle.app.v6.$(boot).2048.ld
      else
        ifeq ($(size_map), 4)
          #LD_SCRIPT = eagle.app.v6.$(boot).1024.app$(app).ld
        else
          ifeq ($(size_map), 3)
            #LD_SCRIPT = eagle.app.v6.$(boot).1024.app$(app).ld
          else
            ifeq ($(size_map), 2)
              #LD_SCRIPT = eagle.app.v6.$(boot).1024.app$(app).ld
            else
              ifeq ($(size_map), 0)
                #LD_SCRIPT = eagle.app.v6.$(boot).512.app$(app).ld
              endif
            endif
	      endif
	    endif
	  endif
	endif
	BIN_NAME = user$(app).$(flash).$(boot).$(size_map)
	CFLAGS += -DAT_UPGRADE_SUPPORT
endif
else
    app = 0
endif

# various paths from the SDK used in this project
SDK_LIBDIR	= lib
SDK_LDDIR	= ld
SDK_INCDIR	= include include/json

# select which tools to use as compiler, librarian and linker
CC	:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
AR	:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD	:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
OBJCOPY := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objcopy
OBJDUMP := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objdump

# no user configurable options below here
SRC_DIR		:= $(MODULES)
BUILD_DIR	:= $(addprefix $(BUILD_BASE)/,$(MODULES))
SDK_LIBDIR	:= $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR	:= $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))
SRC		    := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ		    := $(patsubst %.c,$(BUILD_BASE)/%.o,$(SRC))
LIBS		:= $(addprefix -l,$(LIBS))
APP_AR		:= $(addprefix $(BUILD_BASE)/,$(TARGET)_app.a)
TARGET_OUT	:= $(addprefix $(BUILD_BASE)/,$(TARGET).out)

LD_SCRIPT	:= $(addprefix -T$(SDK_BASE)/$(SDK_LDDIR)/,$(LD_SCRIPT))

INCDIR		:= $(addprefix -I,$(SRC_DIR))
EXTRA_INCDIR	:= $(addprefix -I,$(EXTRA_INCDIR))
MODULE_INCDIR	:= $(addsuffix /include,$(INCDIR))

V ?= $(VERBOSE)
ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif

vpath %.c $(SRC_DIR)

define compile-objects
$1/%.o: %.c
	$(vecho) "CC $$<"
	$(Q) $(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS)  -c $$< -o $$@
endef

.PHONY: all checkdirs clean flash flashboot flashinit rebuild

all: checkdirs $(TARGET_OUT)

$(TARGET_OUT): $(APP_AR)
	$(vecho) $(LD_SCRIPT)
	$(vecho) "LD $@"
	$(Q) $(LD) -L$(SDK_LIBDIR) $(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $(APP_AR) -Wl,--end-group -o $@
	$(vecho) "Run objcopy, please wait..."
	$(Q) $(OBJCOPY) --only-section .text -O binary $@ eagle.app.v6.text.bin
	$(Q) $(OBJCOPY) --only-section .data -O binary $@ eagle.app.v6.data.bin
	$(Q) $(OBJCOPY) --only-section .rodata -O binary $@ eagle.app.v6.rodata.bin
	$(Q) $(OBJCOPY) --only-section .irom0.text -O binary $@ eagle.app.v6.irom0text.bin
	$(vecho) "objcopy done"
	$(vecho) "Run gen_appbin.py"
ifeq ($(app), 0)
	$(Q) $(SDK_TOOLS)/gen_appbin.py $@ 0 $(mode) $(freqdiv) $(size_map) $(app)
	$(Q) mv eagle.app.flash.bin $(FW_BASE)/eagle.flash.bin
	$(Q) mv eagle.app.v6.irom0text.bin $(FW_BASE)/eagle.irom0text.bin
	$(Q) rm eagle.app.v6.*
	$(vecho) "No boot needed."
	$(vecho) "Generate eagle.flash.bin and eagle.irom0text.bin successully in folder $(FW_BASE)"
	$(vecho) "eagle.flash.bin-------->0x00000"
	$(vecho) "eagle.irom0text.bin---->0x10000"
else
    ifneq ($(boot), new)
	$(Q) $(SDK_TOOLS)/gen_appbin.py $@ 1 $(mode) $(freqdiv) $(size_map) $(app)
	$(vecho) "Support boot_v1.1 and +"
    else
	$(Q) $(SDK_TOOLS)/gen_appbin.py $@ 2 $(mode) $(freqdiv) $(size_map) $(app)
    	ifeq ($(size_map), 6)
		$(vecho) "Support boot_v1.4 and +"
        else
            ifeq ($(size_map), 5)
		$(vecho) "Support boot_v1.4 and +"
            else
		$(vecho) "Support boot_v1.2 and +"
            endif
        endif
    endif
	$(Q) mv eagle.app.flash.bin $(FW_BASE)/upgrade/$(BIN_NAME).bin
	$(Q) rm eagle.app.v6.*
	$(vecho) "Generate $(BIN_NAME).bin successully in folder $(FW_BASE)/upgrade"
	$(vecho) "boot.bin------->0x00000"
	$(vecho) "$(BIN_NAME).bin--->$(addr)"
endif
	$(vecho) "Done"

$(APP_AR): $(OBJ)
	$(vecho) "AR $@"
	$(Q) $(AR) cru $@ $^

checkdirs: $(BUILD_DIR) $(FW_BASE)

$(BUILD_DIR):
	$(Q) mkdir -p $@

$(FW_BASE):
	$(Q) mkdir -p $@
	$(Q) mkdir -p $@/upgrade

flashboot:
ifeq ($(app), 0)
	$(vecho) "No boot needed."
else
    ifneq ($(boot), new)
	$(vecho) "Flash boot_v1.1 and +"
	$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x00000 $(SDK_BASE)/bin/boot_v1.1.bin
    else
    	ifeq ($(size_map), 6)
		$(vecho) "Flash rboot"
		$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x00000 rboot.bin
        else
            ifeq ($(size_map), 5)
		$(vecho) "Flash boot_v1.5 and +"
		$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x00000 $(SDK_BASE)/bin/boot_v1.6.bin
            else
		$(vecho) "Flash boot_v1.2 and +"
		$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x00000 $(SDK_BASE)/bin/boot_v1.2.bin
            endif
        endif
    endif
endif

flash: all
ifeq ($(app), 0)
	$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x00000 $(FW_BASE)/eagle.flash.bin 0x10000 $(FW_BASE)/eagle.irom0text.bin
else
ifeq ($(boot), none)
	$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x00000 $(FW_BASE)/eagle.flash.bin 0x10000 $(FW_BASE)/eagle.irom0text.bin
else
	$(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) $(addr) $(FW_BASE)/upgrade/$(BIN_NAME).bin
endif
endif

# ===============================================================
# From http://bbs.espressif.com/viewtopic.php?f=10&t=305
# master-device-key.bin is only need if using espressive services
# master_device_key.bin 0x3e000 is not used , write blank
# See 2A-ESP8266__IOT_SDK_User_Manual__EN_v1.1.0.pdf
# http://bbs.espressif.com/download/file.php?id=532
#
# System parameter area is the last 16KB of flash
# 512KB flash - system parameter area starts from 0x7C000
# 	download blank.bin to 0x7E000 as initialization.
# 1024KB flash - system parameter area starts from 0xFC000
# 	download blank.bin to 0xFE000 as initialization.
# 2048KB flash - system parameter area starts from 0x1FC000
# 	download blank.bin to 0x1FE000 as initialization.
# 4096KB flash - system parameter area starts from 0x3FC000
# 	download blank.bin to 0x3FE000 as initialization.
# ===============================================================

# MAKE VERSION FILE
version:
    $(file > app.ver, MAJOR=$(MAJ)\nMINOR=$(MIN))
    $(vecho) "Version file written to app.ver"

# FLASH SIZE
flashinit:
	$(vecho) "Flash init data default and blank data."
	$(ESPTOOL) -p $(ESPPORT) write_flash $(flashimageoptions) 0x3fc000 $(SDK_BASE)/bin/esp_init_data_default.bin 0x3fe000 $(SDK_BASE)/bin/blank.bin

rebuild: clean all

clean:
	$(Q) rm -f $(APP_AR)
	$(Q) rm -f $(TARGET_OUT)
	$(Q) rm -rf $(BUILD_DIR)
	$(Q) rm -rf $(BUILD_BASE)
	$(Q) rm -rf $(FW_BASE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call compile-objects,$(bdir))))

#flashall:
#    #SPECIFICALLY FOR RBOOT + FLASH MAP = 6 (APP1 + APP2)
#    $(ESPTOOL) -p $(ESPPORT) -b $(BAUD) write_flash $(flashimageoptions) 0x0000 rboot.bin 0x02000 user1.4096.new.6.bin 0x102000 user2.4096.new.6.bin 0x3fc000 esp_init_data_default.bin 0x3fe000 blank.bin
#
#flasherase:
#    #ERASE FLASH
#    $(ESPTOOL) -p $(ESPPORT) -b $(BAUD) erase_flash

help:
	@echo "# Root Level Makefile"
	@echo "# Version 2.0"
	@echo "# (c) by CHERTS <sleuthhound@gmail.com>"
	@echo "#"
	@echo "# Modified By Ankit bhatnagar"
	@echo "# ankit.bhatnagarindia@gmail.com"
	@echo "#"
	@echo "#		- uses rboot for bootloader (instead of espressif boot)"
	@echo "#		- flash (irom0) offset modified to 0x2000 instead of 0x1000"
	@echo "#			as rboot using 2 sectors as compared to 1 by espressif"
	@echo "#			supplied"
	@echo "#		- Note : Need to also change the irom0 base address in the"
	@echo "#						 ld files from 0x40201010 to 0x40202010"
	@echo "#"
	@echo "#		USAGE:"
	@echo "#		make BOOT=<none|new|old> APP=<1|2> SPI_SIZE_MAP=<1..6>"
	@echo "#"
	@echo "#		TO BURN:"
	@echo "#		esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash -fs 32m-c1 -fm qio 0x0000 rboot.bin 0x02000 user1.4096.new.6.bin 0x102000" @echo "     user2.4096.new.6.bin 0x3fc000 ../../esp-open-sdk/sdk/bin/esp_init_data_default.bin 0x3fe000 ../../esp-open-sdk/sdk/bin/blank.bin"
