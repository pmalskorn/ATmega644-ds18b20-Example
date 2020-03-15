#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "OneWire.h"
#include "ds18.h"

int main(void)
{
	onewire_t ow;
	ow.ddr = &DDRB;
	ow.bit = (uint8_t) 1;
	ow.port = &PORTB;
	ow.pin = &PINB;
	
	while (1)
	{
		//volatile rom_t rom = ds18b20_read_rom(&ow);
		//volatile uint8_t rom_crc = ds18b20_crc_rom(rom);
		ds18b20_skip_rom(&ow);
		ds18b20_convert_t(&ow);
		
		//rom = ds18b20_read_rom(&ow);
		//rom_crc = ds18b20_crc_rom(rom);
		ds18b20_skip_rom(&ow);
		volatile scratchpad_t scratchpad = ds18b20_read_scratchpad(&ow);
		volatile uint8_t scratchpad_crc = ds18b20_crc_scratchpad(&scratchpad);
		
		
		
		scratchpad.crc++;
		_delay_ms(1000);
		
	}
}

