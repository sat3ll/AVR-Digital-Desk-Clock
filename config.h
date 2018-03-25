/*
 * config.h
 *
 * Author: sat3ll
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#define SS PB2 //Chip Enable || Data can only be sent when SCE is LOW (p. 7) || PB2 = SS
#define RESET PB1 //LCD Reset
#define DC PB0 //Command or Display | LOW = Command | HIGH = Display
#define MOSI PB3 //Data / MOSI
#define SCLK PB5  //Clock / SCLK

#define DDR_LCD DDRB
#define PORT_LCD PORTB

#ifndef F_CPU
#define F_CPU 16000000
#endif

#endif /* CONFIG_H_ */
