#include <Arduino.h>
#include <LovyanGFX.hpp>

#include "DisplayConfig.h"
#include "FontsConfig.h"

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

LGFX lcd;

// Font configs are included from FontsConfig.h

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

  for (uint16_t c = 32; c <= 255; ++c) {
    const uint16_t width = charWidth(static_cast<char>(c));
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

  for (uint16_t c = 32; c <= 255; ++c) {
    const uint16_t width = charWidth(static_cast<char>(c));
    if (cursorX + width > right) {
      cursorX = left;
      cursorY += lineAdvance;
    }

    if (cursorY + lineAdvance > bottom) {
      break;
    }

    char buffer[2] = {static_cast<char>(c), '\0'};
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
