import processing.serial.*;

Serial myPort;
int heelVal = 0;
int ballVal = 0;

void setup() {
  size(800, 500);
  printArray(Serial.list());
  
  // Replace 0 with the array index corresponding to your ESP32's COM port
  myPort = new Serial(this, Serial.list()[0], 115200);
  myPort.bufferUntil('\n');
}

void draw() {
  background(240);
  
  // Header text
  fill(50);
  textSize(20);
  text("Single-Foot Pressure Monitor", 30, 40);
  
  // Display numeric values
  textSize(16);
  fill(200, 0, 0);
  text("Heel Value: " + heelVal, 30, 80);
  fill(0, 0, 200);
  text("Ball Value: " + ballVal, 400, 80);
  
  // Map 12-bit ADC values (0 - 4095) to visual bar heights
  float maxBarHeight = 300;
  float heelBarHeight = map(heelVal, 0, 4095, 0, maxBarHeight);
  float ballBarHeight = map(ballVal, 0, 4095, 0, maxBarHeight);
  
  // Draw Heel Bar (Red)
  fill(220, 50, 50);
  rect(100, height - 80 - heelBarHeight, 120, heelBarHeight);
  fill(50);
  text("Heel", 140, height - 50);
  
  // Draw Ball Bar (Blue)
  fill(50, 50, 220);
  rect(450, height - 80 - ballBarHeight, 120, ballBarHeight);
  fill(50);
  text("Ball of Foot", 470, height - 50);
}

void serialEvent(Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);
    
    // Parses incoming Arduino string format: "Heel:XXXX\tBall:YYYY"
    String[] list = splitTokens(inString, ":\t");
    if (list.length >= 4) {
      heelVal = int(list[1]);
      ballVal = int(list[3]);
    }
  }
}