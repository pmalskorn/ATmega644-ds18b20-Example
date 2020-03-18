#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "OneWire.h"
#include "ds18b20.h"

#define SimpleRead
//#define SelectRead
//#define ResolutionConfig
//#define testEEPROM

int main(void)
{
	onewire_t ow;
	ow.ddr = &DDRB;
	ow.bit = (uint8_t) 1;
	ow.port = &PORTB;
	ow.pin = &PINB;
	
	while (1)
	{
		#ifdef SimpleRead	
			ds18b20_skip_rom(&ow);
			ds18b20_convert_t(&ow);
			
			ds18b20_skip_rom(&ow);
			scratchpad_t scratchpad = ds18b20_read_scratchpad(&ow);
			uint8_t scratchpad_crc = ds18b20_crc_scratchpad(&scratchpad);
		#endif
		
		#ifdef SelectRead
			rom_t rom = ds18b20_read_rom(&ow);
			uint8_t rom_crc = ds18b20_crc_rom(rom);
			
			ds18b20_match_rom(&ow, rom);
			ds18b20_convert_t(&ow);
			
			ds18b20_match_rom(&ow, rom);
			scratchpad_t scratchpad = ds18b20_read_scratchpad(&ow);
			scratchpad.crc++;
		#endif
				
		#ifdef ResolutionConfig		
			ds18b20_skip_rom(&ow);
			scratchpad_t scratchpad = ds18b20_read_scratchpad(&ow);

			scratchpad.config = BIT_RESOLUTION_12;
			ds18b20_skip_rom(&ow);
			ds18b20_write_scratchpad(&ow, &scratchpad);
			
			ds18b20_skip_rom(&ow);
			ds18b20_convert_t(&ow);
			
			ds18b20_skip_rom(&ow);
			scratchpad = ds18b20_read_scratchpad(&ow);
			volatile uint8_t scratchpad_crc = ds18b20_crc_scratchpad(&scratchpad);
			
			ds18b20_skip_rom(&ow);
			ds18b20_copy_scratchpad(&ow);
		#endif
		
		#ifdef testEEPROM
			ds18b20_skip_rom(&ow);
			scratchpad_t scratchpad = ds18b20_read_scratchpad(&ow);
			
			//set initial resolution to 9 bit
			scratchpad.config = BIT_RESOLUTION_9;
			ds18b20_skip_rom(&ow);
			ds18b20_write_scratchpad(&ow, &scratchpad);
			
			//read resolution
			ds18b20_skip_rom(&ow);
			scratchpad = ds18b20_read_scratchpad(&ow);
			//TESTRESULT ON LCD
			
			//save resolution to EEPROM
			ds18b20_skip_rom(&ow);
			ds18b20_copy_scratchpad(&ow);
			
			//set resolution to 11 bit
			scratchpad.config = BIT_RESOLUTION_11;
			ds18b20_skip_rom(&ow);
			ds18b20_write_scratchpad(&ow, &scratchpad);
			
			//read resolution
			ds18b20_skip_rom(&ow);
			scratchpad = ds18b20_read_scratchpad(&ow);
			//TESTRESULT ON LCD
			
			//revoke old 9 bit resolution to 11 bit
			ds18b20_skip_rom(&ow);
			ds18b20_recall_eeprom(&ow);
			
			//read resolution
			ds18b20_skip_rom(&ow);
			scratchpad = ds18b20_read_scratchpad(&ow);
			//RESULT ON LCD
		#endif
	}
}

