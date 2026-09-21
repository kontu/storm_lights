# storm_lights:
- Project based/started from:
-- https://www.youtube.com/watch?v=N-U8fBv57Vc
-- https://randommakingencounters.com/lightning-and-thunder-effect-arduino-dfplayer-mini-neopixels/
- Wiring based on youtube video logic/layout; not blog logic/layout
- Likely overbuilt

## Base code:
```
  Nothing happens in a vacuum. Thanks to the following:

  Adafruit
  https://learn.adafruit.com/adafruit-neopixel-uberguide

  Indrek Luuk - Circuit Journal
  circuitjournal.com/how-to-use-the-dfplayer-mini-mp3-module-with-an-arduino

  One Guy, One Blog
  oneguyoneblog.com/2017/11/01/lightning-thunder-arduino-halloween-diy/

  "If I have seen further it is by standing on the shoulders of Giants."
  - Isaac Newton
```

## Parts:
### Development use:
- WS2812B 5050 RGB LED DC4-7V Full Color
-- Small enough to run over USB power only
- Bench DC Powersupply
- G/KF128 2 Pin 2.54mm Pitch PCB Screw Terminal Block Connector for power input

### Final product:
- PSU: https://www.amazon.com/dp/B0G4KLQKR9
- LEDStrip: BTF-LIGHTING WS2812B IC RGB LED Strip,UL Listed,DC5V 16.4FT 150LED IP65 
- Resistors between 470 and 10k ohm (1/4w or 1/2w)
- 2200uf 16v Electrolytic Capactors (On power bus - )
- DFplayer Mini (used clone WWZMDiB 5 Pcs Mini MP3 Module Compatible with DFPlayer Code for Arduino Raspberry Pi ESP32 STM)
- Arduino Nano (used ELEGOO Nano Board V3.0, ATmega328P CH340)
- Solderable Breadboard PCB Board Half Size
- 3 pin 3.5mm Headphone jack (uxcell a12062600ux0366 10 Pcs 3 Pin PCB Mount Female 3.5mm Stereo Jack Socket Connector)
- Powered speakers

## Notes:
- Added 10uf parallel shunts from dfplayer VCC & GND to + and - power bus respectively
- Needed a reset circuit because code wouldn't execute loop() on startup
-- Downside - Can't program unless I hold Arduino reset button and releasing at the right time when "Uploading" appears in IDE
-- Considered doing a 2 position switch to enable/disable this so programming was easier...but didn't have one handy.
-- ```
5V pin ---[10kΩ resistor]---+---[10µF ceramic]--- GND
                             |
                          RESET pin
```
- Built noise filter, on each for both DAC_L and DAC_R Dfplayer mini output
-- ```DAC_L pin → 10µF (series, blocks DC) → 1kΩ (series) → output to headphones
                                          |
                                        100nF (to GND, shunts high-freq noise)
```
- Wire LED's directly to power input location
-- I used solderable headers
-- Wire at least one electrolytic cap to this as well to avoid dropdown
- Added 2 electrolytic caps on power bus before Arduino and before DFPlayer to avoid voltage drops
- 