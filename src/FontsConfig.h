#pragma once

#include <LovyanGFX.hpp>

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

const FontCandidate kOrbitronCandidates[] = {
    {"Orbitron32", &fonts::Orbitron_Light_32},
    {"Orbitron24", &fonts::Orbitron_Light_24},
};

const FontCandidate kRobotoCandidates[] = {
    {"Roboto24", &fonts::Roboto_Thin_24},
};

const FontCandidate kSatisfyCandidates[] = {
    {"Satisfy24", &fonts::Satisfy_24},
};

const FontCandidate kYellowtailCandidates[] = {
    {"Yellowtail32", &fonts::Yellowtail_32},
};

const FontCandidate kAsciiCandidates[] = {
    {"Ascii24x48", &fonts::AsciiFont24x48},
    {"Ascii8x16", &fonts::AsciiFont8x16},
};

const FontCandidate kJapanMinchoCandidates[] = {
    {"Mincho40", &fonts::lgfxJapanMincho_40},
    {"Mincho36", &fonts::lgfxJapanMincho_36},
    {"Mincho32", &fonts::lgfxJapanMincho_32},
    {"Mincho28", &fonts::lgfxJapanMincho_28},
    {"Mincho24", &fonts::lgfxJapanMincho_24},
    {"Mincho20", &fonts::lgfxJapanMincho_20},
    {"Mincho16", &fonts::lgfxJapanMincho_16},
    {"Mincho12", &fonts::lgfxJapanMincho_12},
    {"Mincho8", &fonts::lgfxJapanMincho_8},
};

const FontCandidate kJapanGothicCandidates[] = {
    {"Gothic40", &fonts::lgfxJapanGothic_40},
    {"Gothic36", &fonts::lgfxJapanGothic_36},
    {"Gothic32", &fonts::lgfxJapanGothic_32},
    {"Gothic28", &fonts::lgfxJapanGothic_28},
    {"Gothic24", &fonts::lgfxJapanGothic_24},
    {"Gothic20", &fonts::lgfxJapanGothic_20},
    {"Gothic16", &fonts::lgfxJapanGothic_16},
    {"Gothic12", &fonts::lgfxJapanGothic_12},
    {"Gothic8", &fonts::lgfxJapanGothic_8},
};

const FontCandidate keFontCNCandidates[] = {
    {"CN_16", &fonts::efontCN_16},
};

const FontCandidate keFontJACandidates[] = {
    {"JA_16", &fonts::efontJA_16},
};

const FontCandidate keFontKRCandidates[] = {
    {"KR_16", &fonts::efontKR_16},
};

const FontCandidate keFontTWCandidates[] = {
    {"TW_16", &fonts::efontTW_16},
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
    {"Orbitron", 0x07FF, kOrbitronCandidates,
     sizeof(kOrbitronCandidates) / sizeof(kOrbitronCandidates[0])},
    {"Roboto", 0x7E0, kRobotoCandidates,
     sizeof(kRobotoCandidates) / sizeof(kRobotoCandidates[0])},
    {"Satisfy", 0xF800, kSatisfyCandidates,
     sizeof(kSatisfyCandidates) / sizeof(kSatisfyCandidates[0])},
    {"Yellowtail", 0xFFE0, kYellowtailCandidates,
     sizeof(kYellowtailCandidates) / sizeof(kYellowtailCandidates[0])},
    {"Ascii Fix", 0xCE79, kAsciiCandidates,
     sizeof(kAsciiCandidates) / sizeof(kAsciiCandidates[0])},
};
