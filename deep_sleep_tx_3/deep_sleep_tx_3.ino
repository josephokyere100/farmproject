//Node three has mac address of 08:d1:f9:27:b9:a0
#include <esp_now.h>
#include <WiFi.h>

#include "DHT.h"


#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 5
#define DHTPIN 4 
#define DHTTYPE DHT22
#define soil 35

const int wet = 2000;
const int dry = 3200;

DHT dht(DHTPIN, DHTTYPE);

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFC, 0xB4, 0x67, 0x51, 0x5A, 0x1C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int node_id; 
  float temp;
  float humid;
  int moisture;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  
  Serial.println(F("DHTxx test!"));
  dht.begin();
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  //   //Soil moisture
  // int sensVal = analogRead(soil);
  // int moist = map(sensVal,wet,dry,100,0);

  // //Temperature and humidity
  // float h = dht.readHumidity();
  // // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();
  // if (isnan(h) || isnan(t)) {
  //   //Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }
      //Soil moisture
  int moist = 30;
  //Temperature and humidity
  float h = 20.30;
  // Read temperature as Celsius (the default)
  float t = 25.30;

  // Set values to send
  myData.node_id = 3;
  myData.humid = h;
  myData.temp = t;
  myData.moisture = moist;
   
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%");
  Serial.println("");
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("°C");
  Serial.println("");
  Serial.print("Moisture = ");
  Serial.print(moist);
  Serial.println("%");
  delay(2000);

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000); 

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP* uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
 
void loop() {

  // //Soil moisture
  // int sensVal = analogRead(soil);
  // int moist = map(sensVal,wet,dry,100,0);

  // //Temperature and humidity
  // float h = dht.readHumidity();
  // // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();
  // if (isnan(h) || isnan(t)) {
  //   //Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }


  // // Set values to send
  // myData.humid = h;
  // myData.temp = t;
  // myData.moisture = moist;
   
  // // Send message via ESP-NOW
  // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  // if (result == ESP_OK) {
  //   Serial.println("Sent with success");
  // }
  // else {
  //   Serial.println("Error sending the data");
  // }
  //delay(2000);

  // Serial.print("Humidity: ");
  // Serial.print(h);
  // Serial.print("%");
  // Serial.println("");
  
  // Serial.print("Temperature: ");
  // Serial.print(t);
  // Serial.print("°C");
  // Serial.println("");
  // Serial.print("Moisture = ");
  // Serial.print(moist);
  // Serial.println("%");

}
