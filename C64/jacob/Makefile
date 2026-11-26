# necesitamos esto aqui
ifeq ($(OS),Windows_NT)
CC65_BIN := E:/Software/cc65/bin
CC65_INC := E:/Software/cc65/include
EMULATOR := E:/Software/Commodore/GTK3VICE-3.9-win64/bin/x64sc.exe
else
CC65_BIN := /usr/bin
CC65_INC := /usr/share/cc65/include
EMULATOR := /usr/bin/x64
endif

MAKEFLAGS += --warn-undefined-variables

# definiciones del systema de destino
SYSTEM     		:= c64
SYSTEM_LIB 		:= c64.lib
SYSTEM_CONF		:= c64.cfg

# definiciones para mi programa
BUILD_DIR 		:= build
SRC_DIR   		:= .

# definiciones de lo que generaremos
DISK_DIR   		:= ../disk
TARGET			:= jacob
TARGET_EXE    	:= $(DISK_DIR)/$(TARGET).prg
TARGET_MAP    	:= $(BUILD_DIR)/$(TARGET).map
TARGET_LABELS 	:= $(BUILD_DIR)/$(TARGET).lbl

# los flags para las diferentes etapas
CFLAGS  		:= -t $(SYSTEM) -I $(CC65_INC) -I $(SRC_DIR) -T -Osir
ASFLAGS 		:= -t $(SYSTEM)
LDFLAGS 		:= -C $(SYSTEM_CONF) -m $(TARGET_MAP) -Ln $(TARGET_LABELS) -vm

SRC_C65 := $(wildcard $(SRC_DIR)/*.c)
SRC_ASM := $(wildcard $(SRC_DIR)/*.s)
OBJ_C65 := $(SRC_C65:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_ASM := $(SRC_ASM:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

# --------------------------------------------
CC := $(CC65_BIN)/cc65
CA := $(CC65_BIN)/ca65
LD := $(CC65_BIN)/ld65

.PRECIOUS: $(BUILD_DIR)/%.s
.PHONY: all clean run

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJ_C65) $(OBJ_ASM)
	$(info Generando '$(TARGET_EXE)')
	$(LD) $(LDFLAGS) $(OBJ_C65) $(OBJ_ASM) $(SYSTEM_LIB) -o $@

$(BUILD_DIR)/%.s: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s | $(BUILD_DIR)
	$(CA) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CA) $(ASFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -fr $(BUILD_DIR)

run: $(TARGET_EXE)
	$(EMULATOR) $(TARGET_EXE)
