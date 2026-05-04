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

- Displays characters `0x20–0xFF` for each font variant
- Every size of every font family is shown as its own page — tap to advance
- Large fonts that don't fit all characters on screen simply truncate at the bottom

## Adding a font

### Prerequisites

Install freetype and build the Adafruit `fontconvert` tool once:

```bash
brew install freetype

git clone --depth=1 https://github.com/adafruit/Adafruit-GFX-Library.git /tmp/adafruit-gfx
cd /tmp/adafruit-gfx/fontconvert
gcc -Wall -I/opt/homebrew/Cellar/freetype/$(ls /opt/homebrew/Cellar/freetype)/include/freetype2 \
    fontconvert.c -L/opt/homebrew/Cellar/freetype/$(ls /opt/homebrew/Cellar/freetype)/lib \
    -lfreetype -o fontconvert
```

### Convert a TTF

Run `fontconvert` once per point size you want. Pass `32 255` to include the full Latin-1 range (required for `£` and other extended characters):

```bash
FONTCONVERT=/tmp/adafruit-gfx/fontconvert/fontconvert
TTF=/path/to/MyFont-Regular.ttf

$FONTCONVERT "$TTF" 9  32 255 > src/MyFont9pt7b.h
$FONTCONVERT "$TTF" 12 32 255 > src/MyFont12pt7b.h
$FONTCONVERT "$TTF" 18 32 255 > src/MyFont18pt7b.h
$FONTCONVERT "$TTF" 24 32 255 > src/MyFont24pt7b.h
```

> **Note:** when the last character is 255, `fontconvert` names the structs `…8b` instead of `…7b`. Check the generated file for the exact struct name.

### Register the font in FontsConfig.h

1. Add `#include` lines near the top of `src/FontsConfig.h`:

```cpp
#include "MyFont9pt7b.h"
#include "MyFont12pt7b.h"
#include "MyFont18pt7b.h"
#include "MyFont24pt7b.h"
```

2. Add a candidates array (use the exact struct names from the generated headers):

```cpp
const FontCandidate kMyFontCandidates[] = {
    {"MyFont24", (const lgfx::IFont*)&MyFont_Regular24pt8b},
    {"MyFont18", (const lgfx::IFont*)&MyFont_Regular18pt8b},
    {"MyFont12", (const lgfx::IFont*)&MyFont_Regular12pt8b},
    {"MyFont9",  (const lgfx::IFont*)&MyFont_Regular9pt8b},
};
```

3. Add an entry to `kFontGroups[]`:

```cpp
{"MyFont", 0xFFFF, kMyFontCandidates,
 sizeof(kMyFontCandidates) / sizeof(kMyFontCandidates[0])},
```

The colour value is RGB565. `0xFFFF` = white; pick any colour you like.

### Google Fonts

Download TTF files from the [Google Fonts GitHub mirror](https://github.com/google/fonts). Each font family has a directory under `ofl/` or `apache/` containing the `.ttf` files. Check character coverage by opening the font's specimen page on [fonts.google.com](https://fonts.google.com) and clicking the **Glyphs** tab — look for U+00A3 (`£`) in the Latin Extended block.
