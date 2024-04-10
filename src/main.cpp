#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define ledPin1   GPIO_NUM_14
#define ledPin2   GPIO_NUM_27
#define ledPin3   GPIO_NUM_16
#define servoPin  GPIO_NUM_17

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool button1;
  bool button2;
  bool button3;
  short potValue;
} struct_message;

// Create a struct_message called myData
struct_message myData;

Servo servo;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print(" RED: ");
  Serial.print(myData.button1);
  digitalWrite(ledPin1, myData.button1);
  Serial.print(" GREEN: ");
  Serial.print(myData.button2);
  digitalWrite(ledPin2, myData.button2);
  Serial.print(" BLUE: ");
  Serial.print(myData.button3);
  digitalWrite(ledPin3, myData.button3);
  Serial.print(" POT: ");
  Serial.print(myData.potValue);
  Serial.print(" SERVO: ");
  uint8_t pwm = map(myData.potValue, 0, 1023, 0, 180);
  Serial.print(pwm);
  servo.write(pwm);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  servo.setPeriodHertz(50);// Standard 50hz servo
  servo.attach(servoPin, 500, 2400);
  servo.write(90);
}
 
void loop() {

}