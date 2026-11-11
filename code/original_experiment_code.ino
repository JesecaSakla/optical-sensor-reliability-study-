#include <Wire.h>
#include "MAX30105.h"

MAX30105 sensor;

const int readingsPerTrial = 30;  // 30 readings per trial
const int delayMs = 500;          // 0.5 seconds between readings
int trialNumber = 1;

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  Serial.println("MAX30102 Sensor Reliability Test");

  if (!sensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("ERROR: MAX30102 not found. Check VIN, GND, SDA, and SCL wiring.");
    while (1);
  }

  sensor.setup();
  sensor.setPulseAmplitudeRed(0x1F);
  sensor.setPulseAmplitudeIR(0x1F);
  sensor.setPulseAmplitudeGreen(0);

  Serial.println("Sensor connected.");
  Serial.println("Type a condition name and press Send.");
  Serial.println("Use: Baseline, Motion, Loose Contact, Ambient Light, Environmental");
  Serial.println();
  Serial.println("Condition,Trial,ReadingNumber,Time_s,IR");
}

void loop() {
  if (Serial.available() > 0) {
    String condition = Serial.readStringUntil('\n');
    condition.trim();

    if (condition.length() == 0) {
      return;
    }

    Serial.print("START_TRIAL ");
    Serial.print(condition);
    Serial.print(" Trial ");
    Serial.println(trialNumber);

    unsigned long startTime = millis();

    for (int i = 1; i <= readingsPerTrial; i++) {
      long irValue = sensor.getIR();
      float timeSeconds = (millis() - startTime) / 1000.0;

      Serial.print(condition);
      Serial.print(",");
      Serial.print(trialNumber);
      Serial.print(",");
      Serial.print(i);
      Serial.print(",");
      Serial.print(timeSeconds);
      Serial.print(",");
      Serial.println(irValue);

      delay(delayMs);
    }

    Serial.println("END_TRIAL");
    Serial.println();
    Serial.println("Type the next condition name and press Send.");
    Serial.println();

    trialNumber++;
  }
}
