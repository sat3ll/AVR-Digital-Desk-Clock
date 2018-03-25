
```
This code was written a long time ago when I began toying with C and MCUs.
Toolchains, equipments and anything not in the repo I don't possess it anymore.
Uploaded for archival reasons.
```

AVR Digital Desk Clock
=

<p align="center"><img src="/misc/Clock.JPG" width="240" height="239")></p>

A digital clock using the Nokia 5110 LCD (PCD8544), DS1307 RTC module and a
megaAVR chip (ATmega328P) @ 16MHz.  
This project assumes the RTC module is already with the correct date set and
provides no way to adjust it.  
A prebuilt binary is provided in the `builds` directory.


Instructions
=

Wire the following:

| DS1307 | AVR / Arduino Uno R3   |
| ------ | --------------------   |
| SCL    | PC5 / Analog Input 5   |
| SDA    | PC4 / Analog Input 4   |
| VCC    | 5V or 3.3V             |
| GND    | GND                    |

| PCD8544 | AVR / Arduino Uno R3   |
| ------- | --------------------   |
| RST     | PB1 / Digital Pin 9    |
| CE      | PB2 / Digital Pin 10   |
| DC      | PB0 / Digital Pin 8    |
| Din     | PB3 / Digital Pin 11   |
| Clk     | PB5 / Digital Pin 13   |
| VCC     | 3.3V or 5V             |
| GND     | GND                    |



Depending on your LCD module, you may need to use
voltage level converters.
