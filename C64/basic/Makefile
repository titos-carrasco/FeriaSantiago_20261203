# necesitamos esto aqui
ifeq ($(OS),Windows_NT)
EMULATOR := E:/Software/Commodore/GTK3VICE-3.9-win64/bin/x64sc.exe
PETCAT   := E:/Software/Commodore/GTK3VICE-3.9-win64/bin/petcat.exe
else
EMULATOR := /usr/bin/x64
PETCAT	 := /usr/bin/petcat
endif

DISK_DIR   := ../disk
TARGET	   := sensors
TARGET_SRC := ./$(TARGET).bas
TARGET_PRG := $(DISK_DIR)/$(TARGET).prg

.PHONY: all clean run

all: $(TARGET_PRG)

$(TARGET_PRG): $(TARGET_SRC) | $(DISK_DIR)
	$(info Generando '$(TARGET_PRG)')
	$(PETCAT) -w2 -o $(TARGET_PRG) -- $(TARGET_SRC)

$(DISK_DIR):
	mkdir -p $(DISK_DIR)

clean:
	rm -f $(TARGET_PRG)

run: $(TARGET_PRG)
	$(EMULATOR) $(TARGET_PRG)
