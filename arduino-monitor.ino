#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String receivedData = "";
float cpuTemp = 0.0;
int cpuUsage = 0;
float gpuTemp = 0.0;
int gpuUsage = 0;

void setup() {
  Serial.begin(9600);
  
  // OLED 디스플레이 초기화
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("CPU/GPU Monitor"));
  display.println(F("Waiting for data..."));
  display.display();
}

void loop() {
  // 시리얼 데이터 수신
  if (Serial.available() > 0) {
    receivedData = Serial.readStringUntil('\n');
    parseData(receivedData);
    updateDisplay();
  }
}

void parseData(String data) {
  // 데이터 형식: "CPU_TEMP,CPU_USAGE,GPU_TEMP,GPU_USAGE"
  int index1 = data.indexOf(',');
  int index2 = data.indexOf(',', index1 + 1);
  int index3 = data.indexOf(',', index2 + 1);
  
  if (index1 > 0 && index2 > 0 && index3 > 0) {
    cpuTemp = data.substring(0, index1).toFloat();
    cpuUsage = data.substring(index1 + 1, index2).toInt();
    gpuTemp = data.substring(index2 + 1, index3).toFloat();
    gpuUsage = data.substring(index3 + 1).toInt();
  }
}

void updateDisplay() {
  display.clearDisplay();
  
  // CPU 정보 표시
  display.setCursor(0, 0);
  display.println(F("CPU INFO:"));
  display.print(F("TEMP: "));
  display.print(cpuTemp, 1);
  display.println(F(" C"));
  display.print(F("USAGE: "));
  display.print(cpuUsage);
  display.println(F(" %"));
  
  // GPU 정보 표시
  display.setCursor(0, 32);
  display.println(F("GPU INFO:"));
  display.print(F("TEMP: "));
  display.print(gpuTemp, 1);
  display.println(F(" C"));
  display.print(F("USAGE: "));
  display.print(gpuUsage);
  display.println(F(" %"));
  
  display.display();
}