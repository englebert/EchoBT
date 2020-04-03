/*
 *
 * [YAW, THROTTLE, ROLL, PITCH, AXIS RX, AXIS RY,
 * (i, 0, 0, 0, 0, 0, DPAD_CENTERED);      
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <BleGamepad.h> 

BleGamepad bleGamepad;

const uint64_t pipeIn = 0xE8E8F0F0E1LL;     // Must be same as the transmission
RF24 radio( 5,  21);                        // Starting up the module on GPIO5 (CE), GPIO21 (CSN)

// The sizeof this struct should not exceed 32 bytes
struct MyData {
  uint8_t ch1;
  uint8_t ch2;
  uint8_t ch3;
  uint8_t ch4;
  uint8_t ch5;
  uint8_t ch6;
  uint8_t ch7;
};
MyData data;

uint32_t nrf24l01LastReceivedTime = 0;

#define RC_CHANNEL_MIN 990
#define RC_CHANNEL_MAX 2010

bool ledState = false;

bool revolverA[4];
bool revolverB[4];
bool revolverC[2];
uint8_t revolverAPosition = 0;
uint8_t revolverAStage = 0;
uint8_t revolverBPosition = 0;
uint8_t revolverBStage = 0;
uint8_t revolverCPosition = 0;
uint8_t revolverCStage = 0;
uint32_t toggleLastClick = 0;
bool toggleH = false;
bool toggleE = false;
bool toggleF = false;
bool toggleA = false;

void resetData() {
  // 'safe' values to use when no radio input is detected
  data.ch1 = 0;
  data.ch2 = 0;
  data.ch3 = 0;
  data.ch4 = 0;
  data.ch5 = 0;
  data.ch6 = 0;
  data.ch7 = 0;

  // Reset whats needed
  for(int i = 0; i < 4; i++) {
    revolverA[i] = false;
    revolverB[i] = false;
  }
}

void setup() {
  // Resetting All channel data and SBUS Data
  resetData();

  // Set up radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // Both endpoints must have this set the same
  radio.setAutoAck(false);

  // Added for extending the range - Englebert
  radio.setPALevel(RF24_PA_MAX);

  // Setting up RC Channel
  radio.setChannel(76);

  // Starting up to listen
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
  

  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleGamepad.begin();
}

uint32_t lastmillis = 0;
char i;
int a;

uint32_t currentMillis = 0;
uint32_t lastStage1Blink = 0;
uint32_t lastTelemetry = 0;
uint32_t lastMillisSeconds = 0;

bool freshData = false;
char throttle = 0;
char yaw = 0;
char pitch = 0;
char roll = 0;

void loop() {
  currentMillis = millis();

  if(currentMillis - lastmillis > 1000) {
    i++;
    Serial.print("RATE: ");
    Serial.print(data.ch1);
    Serial.print(" - ");
    Serial.print(data.ch2);
    Serial.print(" - ");
    Serial.print(data.ch4);
    Serial.print(" - ");
    Serial.print(data.ch4);
    Serial.print(" - ");
    Serial.println(a);
    a = 0;
    lastmillis = millis();
  }

  while(radio.available()) {        
    radio.read(&data, sizeof(MyData));
    nrf24l01LastReceivedTime = millis();
    freshData = true;
    if(bleGamepad.isConnected()) {
      throttle = map(data.ch1, 0, 255, 127, -127);      // Throttle
      yaw = map(data.ch2, 0, 255, -127, 127);           // Yaw
      pitch = map(data.ch3, 0, 255, 127, -127);         // Pitch
      roll = map(data.ch4, 0, 255, -127, 127);          // Roll

      //                [YAW, THROTTLE, ROLL, PITCH]
      bleGamepad.setAxes(yaw, throttle, roll, pitch, 0, 0, DPAD_CENTERED);
    }
    a++;
  }
}
