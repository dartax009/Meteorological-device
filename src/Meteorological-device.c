#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include <tires/usart/usart.h>
#include <tires/i2c/i2c.h>
#include <devices/BME280/BME280.h>

#ifndef __AVR_ATmega328P__
	#include <avr/iom328p.h>
#endif


#define BME_ADDR	0x76


uint8_t itca (char *buff, uint32_t number);


int main()
{
	char mas[225] = {0};
	char data = 0;
	uint16_t i = 0;
	uint8_t b = 0;
	int32_t temp = 0;
	int32_t pres = 0;
	int32_t hum = 0;

	init_usart();
	init_i2c_master ();
	if ( (b=def_init_BME280_i2c(BME_ADDR)) )
	{
		i = itca(mas, b);
		write_usart("ERROR INIT ", 11);
		write_usart(mas, i);
		write_usart("\n", 1);
		return 0;
	}

	while(1)
	{
		read_usart(&data, 1);
		if (data != '1')
			continue;

		b = 0;
		mas[b++] = 't';
		mas[b++] = ':';
		read_temp_BME280_i2c(BME_ADDR, &temp, 0);
		b += itca(&mas[b], temp);
		mas[b++] = ';';

		mas[b++] = 'p';
		mas[b++] = ':';
		read_pres_BME280_i2c(BME_ADDR, &pres, 0);
		b += itca(&mas[b], pres);
		mas[b++] = ';';

		mas[b++] = 'h';
		mas[b++] = ':';
		read_hum_BME280_i2c(BME_ADDR, &hum, 0);
		b += itca(&mas[b], hum);
		mas[b++] = ';';

		write_usart(mas, b);

		_delay_ms(50);
	}
	return 0;
}

uint8_t itca (char *buff, uint32_t number)
{
	uint8_t count = 0;
	uint8_t tmp = 0;
	uint32_t i = 0;
	char time_buff[255];

	do
	{
		tmp = number % 10;
		number /= 10;
		time_buff[count] = '0'+tmp;
		count++;
	}
	while (number != 0);

	time_buff[count] = '\0';

	while (count != 0)
		buff[i++]=time_buff[--count];

	return i;
}

