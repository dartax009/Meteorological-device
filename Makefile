P_NAME = Meteorological-device

CFLAGS = -Wall -Os -std=c99

#Контроллер
DEVICE = atmega328p
# Тактовая частота 16 МГц
CLOCK = 16000000
COM_PORT = COM5
#Файлик прошивки
AVRDUDE = avrdude -CD:/WinAVR/bin/avrdude.conf -v -p$(DEVICE) -carduino -P$(COM_PORT) -b57600 -D

COMPIL = E:/Coder_experiments/$(P_NAME)/src/

COMPONETS += $(MY_INCLUDE)tires/usart/usart.c
COMPONETS += $(MY_INCLUDE)tires/i2c/i2c.c
COMPONETS += $(MY_INCLUDE)devices/BME280/BME280.c

MY_INCLUDE = E:/Coder_experiments/include/
INCLUDE = -IE:/Coder_experiments/include/

CREATIN_B += E:/Coder_experiments/$(P_NAME)/bin/
CREATIN_D += E:/Coder_experiments/$(P_NAME)/debug/

DIR = avr-gcc -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

RM = rmdir /q /s

COMPILE = $(DIR) $(CFLAGS)


all: $(P_NAME).hex

flash: all
	$(AVRDUDE) -Uflash:w:$(CREATIN_B)$(P_NAME).hex:i

clean:
	if exist "bin" $(RM) bin

# Создает .hex файл
$(P_NAME).hex:	$(P_NAME).elf
	avr-objcopy -j .text -j .data -O ihex $(CREATIN_B)$(P_NAME).elf $(CREATIN_B)$(P_NAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(CREATIN_B)$(P_NAME).elf

# Создает .elf файл
$(P_NAME).elf:
	if exist "bin" make clean
	md bin
	$(COMPILE) -o $(CREATIN_B)$(P_NAME).elf $(COMPONETS) $(INCLUDE) $(COMPIL)$(P_NAME).c