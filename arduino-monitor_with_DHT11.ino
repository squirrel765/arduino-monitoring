#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED 설정
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 설정
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// 시리얼 및 센서 데이터 저장
String receivedData = "";
float cpuTemp = 0.0;
int cpuUsage = 0;
float gpuTemp = 0.0;
int gpuUsage = 0;
float temperature = 0.0;
float humidity = 0.0;

unsigned long lastDHTUpdate = 0;
const unsigned long DHT_INTERVAL = 2000;

void setup() {
  Serial.begin(9600);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("System Monitor"));
  display.display();
}

void loop() {
  // 시리얼 수신 처리
  if (Serial.available()) {
    receivedData = Serial.readStringUntil('\n');
    receivedData.trim();  // 줄바꿈 제거
    parseSerialData(receivedData);
  }

  // DHT11 데이터 업데이트 (2초 간격)
  if (millis() - lastDHTUpdate > DHT_INTERVAL) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    lastDHTUpdate = millis();
  }

  // OLED 출력
  updateDisplay();
  delay(500);  // 출력 주기 조절
}

// 시리얼 문자열 파싱
void parseSerialData(String data) {
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

  // 제목 가운데 정렬 (x=22 정도)
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 4);  // ← 가운데 정렬 + y=4로 내려줌
  display.println(F("System Monitor"));

  // 정보 정렬
  display.setCursor(0, 16);
  display.print(F("CPU: "));
  display.print(cpuTemp, 1); display.print(F("C  "));
  display.print(cpuUsage); display.println(F("%"));

  display.setCursor(0, 28);
  display.print(F("GPU: "));
  display.print(gpuTemp, 1); display.print(F("C  "));
  display.print(gpuUsage); display.println(F("%"));

  display.setCursor(0, 42);
  display.print(F("Room:"));
  display.print(temperature, 1); display.print(F("C  "));
  display.print(humidity, 1); display.println(F("%"));

  display.display();
}
