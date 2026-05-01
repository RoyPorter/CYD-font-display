#include <Arduino.h>
#include <LovyanGFX.hpp>

namespace {

constexpr uint8_t kRotation = 0;
constexpr int16_t kHeaderHeight = 14;
constexpr int16_t kMargin = 4;
constexpr uint32_t kTouchDebounceMs = 180;

constexpr uint16_t kBackground = 0x0000;
constexpr uint16_t kHeaderBackground = 0x1042;
constexpr uint16_t kDivider = 0x4208;
constexpr uint16_t kHeaderText = 0xFFFF;
constexpr uint16_t kHintText = 0xBDF7;

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9342 panel_;
  lgfx::Bus_SPI bus_;
  lgfx::Light_PWM light_;
  lgfx::Touch_XPT2046 touch_;

 public:
  LGFX() {
    {
      auto cfg = bus_.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_dc = 2;
      cfg.freq_write = 27000000;
      cfg.freq_read = 16000000;
      bus_.config(cfg);
      panel_.setBus(&bus_);
    }
    {
      auto cfg = panel_.config();
      cfg.pin_cs = 15;
      cfg.pin_rst = -1;
      cfg.pin_busy = -1;
      cfg.memory_width = 320;
      cfg.memory_height = 240;
      cfg.panel_width = 320;
      cfg.panel_height = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      panel_.config(cfg);
    }
    {
      auto cfg = light_.config();
      cfg.pin_bl = 21;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      light_.config(cfg);
      panel_.setLight(&light_);
    }
    {
      auto cfg = touch_.config();
      cfg.x_min = 300;
      cfg.x_max = 3900;
      cfg.y_min = 3700;
      cfg.y_max = 200;
      cfg.pin_int = -1;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      cfg.spi_host = -1;
      cfg.freq = 1000000;
      cfg.pin_sclk = 25;
      cfg.pin_mosi = 32;
      cfg.pin_miso = 39;
      cfg.pin_cs = 33;
      touch_.config(cfg);
      panel_.setTouch(&touch_);
    }
    setPanel(&panel_);
  }
};

LGFX lcd;

struct FontCandidate {
  const char* name;
  const lgfx::IFont* font;
};

struct FontGroup {
  const char* name;
  uint16_t color;
  const FontCandidate* candidates;
  size_t candidateCount;
};

struct ResolvedFont {
  const char* groupName;
  const char* selectedName;
  const lgfx::IFont* font;
  uint16_t color;
};

const FontCandidate kBuiltinCandidates[] = {
    {"Font8", &fonts::Font8},
    {"Font7", &fonts::Font7},
    {"Font6", &fonts::Font6},
    {"Font4", &fonts::Font4},
    {"Font2", &fonts::Font2},
    {"Font0", &fonts::Font0},
};

const FontCandidate kDejaVuCandidates[] = {
    {"DejaVu24", &fonts::DejaVu24},
    {"DejaVu18", &fonts::DejaVu18},
    {"DejaVu12", &fonts::DejaVu12},
    {"DejaVu9", &fonts::DejaVu9},
};

const FontCandidate kFreeMonoCandidates[] = {
    {"FreeMono24", &fonts::FreeMono24pt7b},
    {"FreeMono18", &fonts::FreeMono18pt7b},
    {"FreeMono12", &fonts::FreeMono12pt7b},
    {"FreeMono9", &fonts::FreeMono9pt7b},
};

const FontCandidate kFreeMonoBoldCandidates[] = {
    {"FreeMonoBold24", &fonts::FreeMonoBold24pt7b},
    {"FreeMonoBold18", &fonts::FreeMonoBold18pt7b},
    {"FreeMonoBold12", &fonts::FreeMonoBold12pt7b},
    {"FreeMonoBold9", &fonts::FreeMonoBold9pt7b},
};

const FontCandidate kFreeMonoObliqueCandidates[] = {
    {"FreeMonoOblique24", &fonts::FreeMonoOblique24pt7b},
    {"FreeMonoOblique18", &fonts::FreeMonoOblique18pt7b},
    {"FreeMonoOblique12", &fonts::FreeMonoOblique12pt7b},
    {"FreeMonoOblique9", &fonts::FreeMonoOblique9pt7b},
};

const FontCandidate kFreeSansCandidates[] = {
    {"FreeSans24", &fonts::FreeSans24pt7b},
    {"FreeSans18", &fonts::FreeSans18pt7b},
    {"FreeSans12", &fonts::FreeSans12pt7b},
    {"FreeSans9", &fonts::FreeSans9pt7b},
};

const FontCandidate kFreeSansBoldCandidates[] = {
    {"FreeSansBold24", &fonts::FreeSansBold24pt7b},
    {"FreeSansBold18", &fonts::FreeSansBold18pt7b},
    {"FreeSansBold12", &fonts::FreeSansBold12pt7b},
    {"FreeSansBold9", &fonts::FreeSansBold9pt7b},
};

const FontCandidate kFreeSansObliqueCandidates[] = {
    {"FreeSansOblique24", &fonts::FreeSansOblique24pt7b},
    {"FreeSansOblique18", &fonts::FreeSansOblique18pt7b},
    {"FreeSansOblique12", &fonts::FreeSansOblique12pt7b},
    {"FreeSansOblique9", &fonts::FreeSansOblique9pt7b},
};

const FontCandidate kFreeSerifCandidates[] = {
    {"FreeSerif24", &fonts::FreeSerif24pt7b},
    {"FreeSerif18", &fonts::FreeSerif18pt7b},
    {"FreeSerif12", &fonts::FreeSerif12pt7b},
    {"FreeSerif9", &fonts::FreeSerif9pt7b},
};

const FontCandidate kFreeSerifBoldCandidates[] = {
    {"FreeSerifBold24", &fonts::FreeSerifBold24pt7b},
    {"FreeSerifBold18", &fonts::FreeSerifBold18pt7b},
    {"FreeSerifBold12", &fonts::FreeSerifBold12pt7b},
    {"FreeSerifBold9", &fonts::FreeSerifBold9pt7b},
};

const FontCandidate kFreeSerifItalicCandidates[] = {
    {"FreeSerifItalic24", &fonts::FreeSerifItalic24pt7b},
    {"FreeSerifItalic18", &fonts::FreeSerifItalic18pt7b},
    {"FreeSerifItalic12", &fonts::FreeSerifItalic12pt7b},
    {"FreeSerifItalic9", &fonts::FreeSerifItalic9pt7b},
};

const FontCandidate kTomThumbCandidates[] = {
    {"TomThumb", &fonts::TomThumb},
};

const FontGroup kFontGroups[] = {
    {"Built-in", 0xFFE0, kBuiltinCandidates,
     sizeof(kBuiltinCandidates) / sizeof(kBuiltinCandidates[0])},
    {"DejaVu", 0xFFFF, kDejaVuCandidates,
     sizeof(kDejaVuCandidates) / sizeof(kDejaVuCandidates[0])},
    {"Free Mono", 0x5DFF, kFreeMonoCandidates,
     sizeof(kFreeMonoCandidates) / sizeof(kFreeMonoCandidates[0])},
    {"Mono Bold", 0xFEC0, kFreeMonoBoldCandidates,
     sizeof(kFreeMonoBoldCandidates) / sizeof(kFreeMonoBoldCandidates[0])},
    {"Mono Oblique", 0x07FF, kFreeMonoObliqueCandidates,
     sizeof(kFreeMonoObliqueCandidates) / sizeof(kFreeMonoObliqueCandidates[0])},
    {"Free Sans", 0xAFE5, kFreeSansCandidates,
     sizeof(kFreeSansCandidates) / sizeof(kFreeSansCandidates[0])},
    {"Sans Bold", 0xFF7F, kFreeSansBoldCandidates,
     sizeof(kFreeSansBoldCandidates) / sizeof(kFreeSansBoldCandidates[0])},
    {"Sans Oblique", 0x07E0, kFreeSansObliqueCandidates,
     sizeof(kFreeSansObliqueCandidates) / sizeof(kFreeSansObliqueCandidates[0])},
    {"Free Serif", 0xF81F, kFreeSerifCandidates,
     sizeof(kFreeSerifCandidates) / sizeof(kFreeSerifCandidates[0])},
    {"Serif Bold", 0xFD20, kFreeSerifBoldCandidates,
     sizeof(kFreeSerifBoldCandidates) / sizeof(kFreeSerifBoldCandidates[0])},
    {"Serif Italic", 0xC618, kFreeSerifItalicCandidates,
     sizeof(kFreeSerifItalicCandidates) / sizeof(kFreeSerifItalicCandidates[0])},
    {"TomThumb", 0xAD7F, kTomThumbCandidates,
     sizeof(kTomThumbCandidates) / sizeof(kTomThumbCandidates[0])},
};

ResolvedFont resolvedFonts[sizeof(kFontGroups) / sizeof(kFontGroups[0])] = {};
constexpr size_t kFontCount = sizeof(kFontGroups) / sizeof(kFontGroups[0]);

size_t currentFontIndex = 0;
bool touchLatched = false;
uint32_t lastAdvanceMs = 0;

int16_t bodyTop() { return kHeaderHeight + 2; }
int16_t bodyBottom() { return lcd.height() - 2; }
int16_t bodyRight() { return lcd.width() - kMargin; }

uint16_t charWidth(char c) {
  char buffer[2] = {c, '\0'};
  uint16_t width = lcd.textWidth(buffer);
  if (width == 0 && c == ' ') {
    width = lcd.textWidth("i");
  }
  return max<uint16_t>(width, 1);
}

bool fontFits(const lgfx::IFont* font) {
  lcd.setFont(font);

  const int16_t left = kMargin;
  const int16_t right = bodyRight();
  const int16_t bottom = bodyBottom();
  const int16_t lineAdvance = max<int16_t>(1, lcd.fontHeight());

  int16_t cursorX = left;
  int16_t cursorY = bodyTop();

  for (char c = 32; c <= 126; ++c) {
    const uint16_t width = charWidth(c);
    if (cursorX + width > right) {
      cursorX = left;
      cursorY += lineAdvance;
    }

    if (cursorY + lineAdvance > bottom) {
      return false;
    }

    cursorX += width;
  }

  return true;
}

ResolvedFont selectLargestFittingFont(const FontGroup& group) {
  const FontCandidate* fallback = &group.candidates[group.candidateCount - 1];

  for (size_t i = 0; i < group.candidateCount; ++i) {
    if (fontFits(group.candidates[i].font)) {
      return {group.name, group.candidates[i].name, group.candidates[i].font,
              group.color};
    }
  }

  return {group.name, fallback->name, fallback->font, group.color};
}

void resolveFonts() {
  for (size_t i = 0; i < kFontCount; ++i) {
    resolvedFonts[i] = selectLargestFittingFont(kFontGroups[i]);
  }
}

void drawHeader(const ResolvedFont& entry) {
  lcd.fillRect(0, 0, lcd.width(), kHeaderHeight, kHeaderBackground);
  lcd.drawFastHLine(0, kHeaderHeight - 1, lcd.width(), kDivider);

  lcd.setFont(&fonts::Font0);
  lcd.setTextColor(kHeaderText, kHeaderBackground);
  lcd.drawString(entry.selectedName, kMargin, 3);

  const char* hint = "tap";
  lcd.setTextColor(kHintText, kHeaderBackground);
  lcd.drawString(hint, lcd.width() - kMargin - lcd.textWidth(hint), 3);

  char indexLabel[24];
  snprintf(indexLabel, sizeof(indexLabel), "%u/%u",
           static_cast<unsigned>(currentFontIndex + 1),
           static_cast<unsigned>(kFontCount));
  const int16_t hintX = lcd.width() - kMargin - lcd.textWidth(hint);
  lcd.drawString(indexLabel, hintX - 4 - lcd.textWidth(indexLabel), 3);
}

void drawFontBody(const ResolvedFont& entry) {
  lcd.fillRect(0, kHeaderHeight, lcd.width(), lcd.height() - kHeaderHeight,
               kBackground);
  lcd.setFont(entry.font);
  lcd.setTextColor(entry.color, kBackground);

  const int16_t left = kMargin;
  const int16_t right = bodyRight();
  const int16_t bottom = bodyBottom();
  const int16_t lineAdvance = max<int16_t>(1, lcd.fontHeight());

  int16_t cursorX = left;
  int16_t cursorY = bodyTop();

  for (char c = 32; c <= 126; ++c) {
    const uint16_t width = charWidth(c);
    if (cursorX + width > right) {
      cursorX = left;
      cursorY += lineAdvance;
    }

    if (cursorY + lineAdvance > bottom) {
      break;
    }

    char buffer[2] = {c, '\0'};
    lcd.drawString(buffer, cursorX, cursorY);
    cursorX += width;
  }
}

void renderCurrentFont() {
  const ResolvedFont& entry = resolvedFonts[currentFontIndex];
  lcd.startWrite();
  drawHeader(entry);
  drawFontBody(entry);
  lcd.endWrite();
}

void advanceFont() {
  currentFontIndex = (currentFontIndex + 1) % kFontCount;
  renderCurrentFont();
}

void initDisplay() {
  lcd.init();
  lcd.setRotation(kRotation);
  lcd.setBrightness(255);
  lcd.fillScreen(kBackground);
}

void pollTouch() {
  uint16_t touchX;
  uint16_t touchY;
  const bool touched = lcd.getTouch(&touchX, &touchY);
  const uint32_t now = millis();

  if (touched && !touchLatched && now - lastAdvanceMs >= kTouchDebounceMs) {
    touchLatched = true;
    lastAdvanceMs = now;
    advanceFont();
  } else if (!touched) {
    touchLatched = false;
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  initDisplay();
  resolveFonts();
  renderCurrentFont();
}

void loop() {
  pollTouch();
  delay(20);
}
