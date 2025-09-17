# Nom du fichier exécutable
TARGET = firmware.elf

# Chemin des fichiers sources
USER_SRC_DIR = user
USER_SRCS = $(wildcard $(USER_SRC_DIR)/*.c)

# Fichiers objets
OBJS = $(USER_SRCS:.c=.o)

# Outilchain (assure-toi qu'elle est installée et dans le PATH)
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -Wall -ffreestanding -I$(USER_SRC_DIR)
LDFLAGS = -TSTM32L4.ld # remplace par ton script de linkage si différent

# Règle principale
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O ihex $@ firmware.hex
	$(OBJCOPY) -O binary $@ firmware.bin
	$(SIZE) $@

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(USER_SRC_DIR)/*.o $(TARGET) firmware.hex firmware.bin

.PHONY: all clean
