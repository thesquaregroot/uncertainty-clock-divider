// Adapted from https://github.com/oamodular/uncertainty/blob/main/software/stable/uncertainty/uncertainty.ino
// Original license: https://github.com/oamodular/uncertainty/blob/main/LICENSE.md
// License for this code: https://creativecommons.org/licenses/by-sa/4.0/

#include <Arduino.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "fp.hpp"

using namespace fp;

typedef fp_t<int, 12> cv_t;

// number of output gates
#define NUM_GATES 8

cv_t lowToHighThreshold = cv_t(1.5);
cv_t highToLowThreshold = cv_t(0.5);

// ADC input pin
int inputPin = 26;
// hold pins for gates
int gatePins[] = {27,28,29,0,3,4,2,1};

uint8_t value = 0;
bool wasClock = false;
bool wasReset = false;

void setup() {
  // 2x overclock for MAX POWER
  set_sys_clock_khz(250000, true);

  Serial.begin(9600);

  // initialize ADC
  adc_init();
  adc_gpio_init(inputPin);
  adc_select_input(0);
  gpio_set_pulls(inputPin, false, false);
  
  // initialize gate out pins
  for(int i=0; i<NUM_GATES; i++) {
    int pin = gatePins[i];
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
  }
}

void loop() {
  cv_t input = (cv_t(adc_read())>>12)*cv_t(10.0) - cv_t(5.0);

  // schmitt triggering
  cv_t clockThreshold = (wasClock ? highToLowThreshold : lowToHighThreshold);
  cv_t resetThreshold = (wasReset ? highToLowThreshold : lowToHighThreshold);

  bool isClock = input > clockThreshold;
  bool isReset = input < -resetThreshold;
  bool isResetTrigger = !wasReset && isReset;
  bool isClockTrigger = !wasClock && isClock;

  if (isResetTrigger) {
    value = 0;
  }
  else if (isClockTrigger) {
    value -= 1;
  }

  gpio_put(gatePins[0], value & 0x01);
  gpio_put(gatePins[1], value & 0x02);
  gpio_put(gatePins[2], value & 0x04);
  gpio_put(gatePins[3], value & 0x08);
  gpio_put(gatePins[4], value & 0x10);
  gpio_put(gatePins[5], value & 0x20);
  gpio_put(gatePins[6], value & 0x40);
  gpio_put(gatePins[7], value & 0x80);

  wasClock = isClock;
  wasReset = isReset;
}
