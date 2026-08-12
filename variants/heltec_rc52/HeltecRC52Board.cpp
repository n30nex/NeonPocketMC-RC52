#include "HeltecRC52Board.h"

#include <Wire.h>
#include <nrf_gpio.h>
#include "variant.h"

static constexpr float RC52_ADC_MV_LSB = 3000.0f / 4096.0f;
static float rc52_adc_multiplier = ADC_MULTIPLIER;

#ifdef NRF52_POWER_MANAGEMENT
static const PowerMgtConfig rc52_power_config = {
  PWRMGT_LPCOMP_AIN,
  PWRMGT_LPCOMP_REFSEL,
  PWRMGT_VOLTAGE_BOOTLOCK
};

void HeltecRC52Board::initiateShutdown(uint8_t reason) {
  variant_shutdown();
  nrf_gpio_cfg_sense_input(PIN_USER_BTN, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
  enterSystemOff(reason);
}
#endif

void HeltecRC52Board::begin() {
  NRF52BoardDCDC::begin();

  pinMode(PIN_VBAT_READ, INPUT);
  pinMode(PIN_BAT_CTL, OUTPUT);
  digitalWrite(PIN_BAT_CTL, LOW);

#ifdef NRF52_POWER_MANAGEMENT
  checkBootVoltage(&rc52_power_config);
#endif

  Wire.setPins(PIN_WIRE_SDA, PIN_WIRE_SCL);
  Wire.begin();

  pinMode(RADIOCORE_FEM_EN, OUTPUT);
  digitalWrite(RADIOCORE_FEM_EN, HIGH);
  pinMode(RADIOCORE_VFEM_CTRL, OUTPUT);
  digitalWrite(RADIOCORE_VFEM_CTRL, HIGH);
  delay(10);
}

uint16_t HeltecRC52Board::getBattMilliVolts() {
  analogReadResolution(12);
  analogReference(AR_INTERNAL_3_0);
  pinMode(PIN_BAT_CTL, OUTPUT);
  digitalWrite(PIN_BAT_CTL, HIGH);
  delay(10);
  const int adc = analogRead(PIN_VBAT_READ);
  digitalWrite(PIN_BAT_CTL, LOW);
  return (uint16_t)(adc * RC52_ADC_MV_LSB * rc52_adc_multiplier);
}

bool HeltecRC52Board::setAdcMultiplier(float multiplier) {
  if (multiplier < 1.0f || multiplier > 10.0f) return false;
  rc52_adc_multiplier = multiplier;
  return true;
}

float HeltecRC52Board::getAdcMultiplier() const {
  return rc52_adc_multiplier;
}

void HeltecRC52Board::powerOff() {
  shutdownPeripherals();
#ifdef NRF52_POWER_MANAGEMENT
  initiateShutdown(SHUTDOWN_REASON_USER);
#else
  variant_shutdown();
  NRF52Board::powerOff();
#endif
}
