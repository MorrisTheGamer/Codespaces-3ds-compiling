#---------------------------------------------------------------------------------
# TARGET ist der Name der App
#---------------------------------------------------------------------------------
TARGET      := VirtualClicker
APP_TITLE   := Virtualle Clicker
APP_AUTHOR  := Morris
APP_DESC    := Virtualle Clicker

# Verzeichnisse
SOURCES     := source
INCLUDES    := include
ROMFS       := romfs
BUILD       := build

# Bibliotheken (Reihenfolge ist wichtig für den Linker!)
LIBS        := -lcitro2d -lcitro3d -lctru -lcurl -lmbedtls -lmbedx509 -lmbedcrypto -lz -lm

#---------------------------------------------------------------------------------
# Ab hier die Standard-Logik für devkitARM
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Bitte DEVKITARM in der Umgebung setzen. export DEVKITARM=<pfad zu>/devkitARM")
endif

include $(DEVKITARM)/3ds_rules

# Regeln für das Erstellen der 3dsx mit Metadaten
all: $(TARGET).3dsx

$(TARGET).3dsx: $(TARGET).elf
	@smdhtool --create "$(APP_TITLE)" "$(APP_DESC)" "$(APP_AUTHOR)" $(TARGET).smdh
	@3dsxtool $(TARGET).elf $(TARGET).3dsx --smdh=$(TARGET).smdh --romfs=$(ROMFS)

$(TARGET).elf: $(SOURCES)
	@mkdir -p $(BUILD)
	@$(MAKE) -C $(BUILD) -f $(CURDIR)/Makefile.logic

clean:
	@rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).elf $(TARGET).smdh
	@echo "Clean done."