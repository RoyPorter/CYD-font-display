# Cheap Yellow Display Font Viewer

This is a simple font browser which should work on any CYD board. (Tested on a ESP32-2432S028). It runs in landscape, displays the full printable ASCII range for one font at a time, and advances to the next font whenever the screen is pressed.

## Hardware config

- `esp32dev` PlatformIO target
- `LovyanGFX` display stack
- `Panel_ILI9342` in landscape mode (`320x240`)
- `XPT2046` touch 

Pins baked into `src/DisplayConfig.h`:

| Signal | Pin |
| --- | --- |
| Display SCLK | 14 |
| Display MOSI | 13 |
| Display MISO | 12 |
| Display DC | 2 |
| Display CS | 15 |
| Backlight | 21 |
| Touch SCLK | 25 |
| Touch MOSI | 32 |
| Touch MISO | 39 |
| Touch CS | 33 |

If you have a different board variant, update the `LGFX` config block in `src/DisplayConfig.h`.

## Build and upload

```bash
pio run
pio run --target upload
pio device monitor
```

## Behavior

- Shows printable ASCII characters `32-126`
- Chooses the largest available variant in each font family/style that still fits all printable characters on screen
- Any touch advances to the next font
