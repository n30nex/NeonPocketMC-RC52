#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

namespace {

constexpr uint8_t kTftSck = 30;
constexpr uint8_t kTftMiso = 12;
constexpr uint8_t kTftDc = 28;
constexpr uint8_t kTftReset = 10;
constexpr uint8_t kTftEnable = 45;
constexpr uint8_t kTftBacklight = 9;
constexpr uint32_t kSpiFrequency = 8000000;

bool spi_started = false;

void showPinMap(uint8_t mosi, uint8_t cs, uint16_t color, const char* label) {
  digitalWrite(kTftBacklight, LOW);
  digitalWrite(kTftEnable, HIGH);
  delay(250);

  if (spi_started) SPI1.end();
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  digitalWrite(36, HIGH);
  digitalWrite(37, HIGH);
  SPI1.setPins(kTftMiso, kTftSck, mosi);

  Arduino_HWSPI bus(kTftDc, cs, &SPI1, true);
  Arduino_NV3001B panel(&bus, kTftReset, 3, true, 128, 220, 0, 0, 0, 0);

  digitalWrite(kTftEnable, LOW);
  delay(20);
  Serial.print("RC52_DISPLAY_DIAG map=");
  Serial.println(label);
  const bool ready = panel.begin(kSpiFrequency);
  spi_started = true;
  Serial.print("RC52_DISPLAY_DIAG begin=");
  Serial.println(ready ? "ok" : "failed");
  if (ready) {
    panel.setRotation(3);
    panel.fillScreen(color);
    panel.setTextColor(RGB565_WHITE);
    panel.setTextSize(2);
    panel.setCursor(12, 20);
    panel.println("RC52 TFT TEST");
    panel.setTextSize(1);
    panel.setCursor(12, 52);
    panel.print("Pins ");
    panel.println(label);
  }
  digitalWrite(kTftBacklight, HIGH);
  delay(5000);
}

} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(kTftEnable, OUTPUT);
  pinMode(kTftBacklight, OUTPUT);
  digitalWrite(kTftEnable, HIGH);
  digitalWrite(kTftBacklight, LOW);
}

void loop() {
  showPinMap(35, 37, RGB565_RED, "35 / 37 RED");
  showPinMap(34, 36, RGB565_BLUE, "34 / 36 BLUE");
}
