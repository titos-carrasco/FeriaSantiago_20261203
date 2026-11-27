# necesitamos esto aqui
ifeq ($(OS),Windows_NT)
EMULATOR := E:/Software/Commodore/GTK3VICE-3.9-win64/bin/x64sc.exe
PETCAT   := E:/Software/Commodore/GTK3VICE-3.9-win64/bin/petcat.exe
C1541    := E:/Software/Commodore/GTK3VICE-3.9-win64/bin/c1541.exe
else
EMULATOR := /usr/bin/x64
PETCAT	 := /usr/bin/petcat
C1541	 := /usr/bin/c1541
endif

DISK_DIR   := ../disk
DISK_D64   := $(DISK_DIR)/feriatic.d64
DISK_ID := "feriatic,01"

TARGET	   := sensors
TARGET_SRC := ./$(TARGET).bas
TARGET_PRG := $(DISK_DIR)/$(TARGET).prg

.PHONY: all clean run

all: $(DISK_D64)

$(DISK_D64): $(TARGET_PRG) | $(DISK_DIR)
	if [ ! -f $(DISK_D64) ]; then \
		echo "Creando '$(DISK_D64)' ..."; \
		$(C1541) -format $(DISK_ID) d64 $(DISK_D64); \
	fi
	$(info Agregando '$(TARGET_PRG)')
	$(C1541) -attach $(DISK_D64) -delete $(TARGET)
	$(C1541) -attach $(DISK_D64) -write $(TARGET_PRG) $(TARGET)
	$(C1541) -attach $(DISK_D64) -list

$(TARGET_PRG): $(TARGET_SRC) | $(DISK_DIR)
	$(info Generando '$(TARGET_PRG)')
	$(PETCAT) -w2 -o $(TARGET_PRG) -- $(TARGET_SRC)

$(DISK_DIR):
	mkdir -p $(DISK_DIR)

clean:
	rm -f $(TARGET_PRG)

run: $(DISK_D64)
	$(EMULATOR) -autostart "$(DISK_D64):$(TARGET)"
