# BYD Font Viewer

This is a simple font browser for the APKLVSR/BuyDisplay-style 2.8" 320x240 ESP32 touchscreen board from your Amazon link. It runs in landscape, displays the full printable ASCII range for one font at a time, and advances to the next font whenever the screen is pressed.

## Hardware config

- Reuses the working board setup from `../HouseStatusDisplay`
- `esp32dev` PlatformIO target
- `LovyanGFX` display stack
- `Panel_ILI9342` in landscape mode (`320x240`)
- `XPT2046` touch with the same calibration and pins as the sibling project

Pins baked into `src/main.cpp`:

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

If you later change board variants, update the `LGFX` config block in `src/main.cpp`.

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
