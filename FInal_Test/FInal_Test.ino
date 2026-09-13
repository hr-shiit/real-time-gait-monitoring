#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu(Wire);

const int heelPin = 34;
const int ballPin = 35;
const int sampleDelay = 150; // Increased delay for slower, readable updates (in milliseconds)

// Function to select TCA9548A multiplexer channel (0 to 7)
void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL for ESP32
  analogSetAttenuation(ADC_11db); // Full-scale voltage range (~3.3V)

  // Initialize all 5 IMUs through the multiplexer channels 0 through 4
  for (int i = 0; i < 5; i++) {
    tcaselect(i);
    mpu.begin();
  }
}

void loop() {
  // Read FSR sensors independently
  int heelVal = analogRead(heelPin);
  int ballVal = analogRead(ballPin);

  // Print separate, explicit tags for each FSR
  Serial.print("Heel:");
  Serial.print(heelVal);
  Serial.print(",Ball:");
  Serial.print(ballVal);

  // Read and print each of the 5 IMUs (Channels 0 to 4)
  for (int i = 0; i < 5; i++) {
    tcaselect(i);
    mpu.update();
    
    Serial.print(",I");
    Serial.print(i);
    Serial.print(":");
    Serial.print(mpu.getAccX(), 2);
    Serial.print(",");
    Serial.print(mpu.getAccY(), 2);
    Serial.print(",");
    Serial.print(mpu.getAccZ(), 2);
  }
  
  Serial.println();
  delay(sampleDelay); // Controlled loop delay
}