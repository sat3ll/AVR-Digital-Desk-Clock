/*
 * Copyright (c) 2014, sat3ll
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation and/or 
 * other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS 
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 * EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "include/libpcd8544.h"

/* Command Masks */
#define SET_VOP 0x80
#define SET_BIAS 0x10
#define TEMP_COEF 0x04
#define FUNC_SET 0x20
#define MODE 0x08
#define X_MASK 0x80
#define Y_MASK 0x40

/* Send a byte through SPI */
void write(uint8_t byte)
{
	PORT_LCD &= ~(1<<SS);
	SPDR = byte;
	while(!(SPSR & (1<<SPIF)));
	PORT_LCD |= (1<<SS);
}

/* Reset signal for the pcd8544 */
static void reset(void)
{
	PORT_LCD |= (1<<RESET);
	_delay_us(10); /* Let it stabilize */
	PORT_LCD &= ~(1<<RESET);
	_delay_us(1); /* minimum delay is 100nS, refer to PCD8544 datasheet, p.20 */
	PORT_LCD |= (1<<RESET); /* RESET is active LOW therefore, set this HIGH */
}

/* Go to the X=0, Y=0 coordinates of the LCD */
void origin(void)
{
	PORT_LCD &= ~(1<<DC);
	write(0 | Y_MASK);
	write(0 | X_MASK);
}

/* Erase the Screen by filling it with 0x00 */
void erase_full(void)
{
	horizontal_mode();
	origin();
	
	PORT_LCD |= (1<<DC);
	for (int i=0;i <= 504; i++)
		write(0x00);
}

/*
 * Vlcd = 3.06 + VOP6_VOP0 * 0.06
 * VOP must be higher than 50 (DECIMAL) in order to be able to see the LCD
 * and lower than 90 (DECIMAL) (8.5 Volts) to avoid damage. */
void init(void)
{
	DDR_LCD |= ( (1<<MOSI)|(1<<SCLK)|(1<<RESET)|(1<<SS)|(1<<DC) ); /* Set the required pins */
	SPCR = ((1<<SPE)|(1<<MSTR)); /* SPI Enable, MSB first, Master Mode, Phase 0, SPR1_SPR0 0 */
	reset(); /* Starting with a RESET signal as indicated in datasheet */
	PORT_LCD |= (1<<SS); /* Chip SPI is initialised when SS is HIGH */
	
	write(1 | FUNC_SET); /* Extended instruction */
	write(60 | SET_VOP); /* Values can be between 60 - 70 */
	write(2 | TEMP_COEF); /* Temperature Coefficient */
	write(3 | SET_BIAS); /* Voltage bias */
	write(0 | FUNC_SET); /* Basic instruction */
	write(4 | MODE); /* Normal display mode, non-inverted color display */
	erase_full();
}

/* Increments pixel through the Y axis */
void vertical_mode(void)
{
	PORT_LCD &= ~(1<<DC);
	write(0b00100010); /* Vertical mode */
}

/* Increments pixel through the X axis */
void horizontal_mode(void)
{
	PORT_LCD &= ~(1<<DC);
	write(0b00100000); /* Horizontal mode */
}

/* Go to X coordinate */
void x_position(uint8_t coord)
{
	if (coord < 84)
	{
		PORT_LCD &= ~(1<<DC);
		write(coord | X_MASK);
	}
	else
		return; /* won't go to upper address / invalid value. */
}

/* Go to Y Bank */
void y_bank(uint8_t bank)
{
	if (bank < 6)
	{
		PORT_LCD &= ~(1<<DC);
		write(bank | Y_MASK);
	}
	else
		return; //won't go to upper address / invalid value. */
}
