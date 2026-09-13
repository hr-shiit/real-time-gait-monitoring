#include <Arduino.h>
#include <Wire.h>

#define MUX_ADDR 0x70
#define MPU_ADDR 0x68

const float ALPHA = 0.96; 
const uint8_t NUM_SENSORS = 5;
const uint8_t channels[NUM_SENSORS] = {0, 1, 2, 3, 4}; 

const unsigned long INTER_CHANNEL_DELAY_MS = 5;  
const unsigned long LOOP_DELAY_MS          = 20; // ~40-50 Hz stream rate

struct FilteredSensor {
  float pitch = 0;
  float roll  = 0;
  unsigned long lastTime = 0;
  bool active = false;
};

FilteredSensor sensors[NUM_SENSORS];

void selectMuxChannel(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void writeRegister(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

bool readRawDataSafe(uint8_t reg, int16_t &result) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false; 

  uint8_t bytesReceived = Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)2);
  if (bytesReceived == 2) {
    result = (Wire.read() << 8) | Wire.read();
    return true;
  }
  return false;
}

void setupMPU() {
  writeRegister(0x6B, 0x00); 
  delay(5);
  writeRegister(0x1A, 0x04); 
  delay(5);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);
  Wire.setClock(400000); // 400kHz Fast Mode for fast multi-sensor polling

  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    selectMuxChannel(channels[i]);
    setupMPU();

    int16_t raw_ax, raw_ay, raw_az;
    if (readRawDataSafe(0x3B, raw_ax) && readRawDataSafe(0x3D, raw_ay) && readRawDataSafe(0x3F, raw_az)) {
      float ax = raw_ax / 16384.0;
      float ay = raw_ay / 16384.0;
      float az = raw_az / 16384.0;

      sensors[i].pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / M_PI;
      sensors[i].roll  = atan2(ay, az) * 180.0 / M_PI;
      sensors[i].active = true;
    } else {
      sensors[i].active = false;
    }
    
    sensors[i].lastTime = millis();
    delay(10);
  }
}

void loop() {
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    if (!sensors[i].active) continue;

    selectMuxChannel(channels[i]);

    int16_t raw_ax, raw_ay, raw_az, raw_gx, raw_gy;
    
    if (readRawDataSafe(0x3B, raw_ax) && 
        readRawDataSafe(0x3D, raw_ay) && 
        readRawDataSafe(0x3F, raw_az) && 
        readRawDataSafe(0x43, raw_gx) && 
        readRawDataSafe(0x45, raw_gy)) {

      FilteredSensor &s = sensors[i];
      unsigned long currentTime = millis();
      float dt = (currentTime - s.lastTime) / 1000.0;
      if (dt <= 0) dt = 0.001; 
      s.lastTime = currentTime;

      float ax = raw_ax / 16384.0;
      float ay = raw_ay / 16384.0;
      float az = raw_az / 16384.0;

      float gx_rate = raw_gx / 131.0; 
      float gy_rate = raw_gy / 131.0; 

      float accel_pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / M_PI;
      float accel_roll  = atan2(ay, az) * 180.0 / M_PI;

      s.pitch = ALPHA * (s.pitch + gy_rate * dt) + (1.0 - ALPHA) * accel_pitch;
      s.roll  = ALPHA * (s.roll  + gx_rate * dt) + (1.0 - ALPHA) * accel_roll;
    }

    delay(INTER_CHANNEL_DELAY_MS);
  }

  // --- PROCESSING-FRIENDLY STREAM FORMAT ---
  // Just comma-separated values ending with a newline
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensors[i].pitch, 2); Serial.print(",");
    Serial.print(sensors[i].roll, 2);
    if (i < NUM_SENSORS - 1) Serial.print(",");
  }
  Serial.println();

  delay(LOOP_DELAY_MS); 
}