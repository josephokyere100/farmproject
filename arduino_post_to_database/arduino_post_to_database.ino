#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>

#include <Wire.h>

#include <esp_now.h>
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

//Structure to hold readings from each node
struct_message node1;
struct_message node2;
struct_message node3;
struct_message node4;

struct_message nodes[4]={node1, node2, node3, node4};


const char* ssid = "Joseph";
const char* password = "awdrgy2k24";
String serverPath;
WebServer server(80);

String serverName = "http://192.168.121.18/agric_project/farmproject/"; //update with correct IP address 
//String message = "Humidity Values: <br/>";
//http://localhost/agric_project/agric_data.php?nodeID={nodeID}&temperature={temperature}&humidity={humidity}&moisture={moisture}
//void doGETRequest(int nodevar);


//

  float board1temp;
  float board1humid;
  int board1moist;
    float board2temp;
  float board2humid;
  int board2moist;
    float board3temp;
  float board3humid;
  int board3moist;
    float board4temp;
  float board4humid;
  int board4moist;
//==========================================
void wifi_init(){
    WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

   // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // put your setup code here, to run once:
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

//  ArduinoOTA.setHostname("NNA_ESP32");
//  ArduinoOTA.setPassword("pass");
//  ArduinoOTA.begin();   // initialise ArduinoOTA

}


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.node_id, len);
  // Update the structures with the new incoming data
  nodes[myData.node_id-1].temp = myData.temp;
  nodes[myData.node_id-1].humid = myData.humid;
  nodes[myData.node_id-1].moisture = myData.moisture;

  Serial.printf("temperature value: %f \n", nodes[myData.node_id-1].temp);
  Serial.printf("humidity value: %f \n", nodes[myData.node_id-1].humid);
  Serial.printf("Moisture value: %d \n", nodes[myData.node_id-1].moisture);
  Serial.println();
}

void setup() { 
  
   
  Serial.begin(115200);
  
  //wifi_init();
    WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // put your setup code here, to run once:
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

   WiFi.begin(ssid, password);
   WiFi.printDiag(Serial);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  xTaskCreate( 
    board1, 
    "This is task 1", 
    2048, 
    NULL, 
    1, 
    NULL 
  ); 
 
    xTaskCreate( 
    board2, 
    "This is task 2", 
    2048, 
    NULL, 
    1, 
    NULL 
  ); 
 
    xTaskCreate( 
    board3, 
    "This is task 3", 
    2048, 
    NULL, 
    3, 
    NULL 
  );

    xTaskCreate( 
    board4, 
    "This is task 4", 
    2048, 
    NULL, 
    3, 
    NULL 
  );  
  
} 
 


void loop()
{
  // float board1temp = nodes[0].temp;
  // float board1humid = nodes[0].humid;
  // int board1moist = nodes[0].moisture;
  // Serial.print("The temperature from node 1 is: ");
  // Serial.println(board1temp);
  // Serial.print("The humidity from node 1 is: ");
  // Serial.println(board1humid);
  // Serial.print("The moisture from node 1 is: ");
  // Serial.println(board1moist);
  //  delay(2000);//allow the cpu to switch to other tasks

  }

//=======================================  
// This void is called every time we have a message from the broker


//Task for board 1
void board1(void*parameters){ 

for(;;){ 
  board1temp = nodes[0].temp;
  board1humid = nodes[0].humid;
  board1moist = nodes[0].moisture;
  Serial.print("The temperature from node 1 is: ");
  Serial.println(board1temp);
  Serial.print("The humidity from node 1 is: ");
  Serial.println(board1humid);
  Serial.print("The moisture from node 1 is: ");
  Serial.println(board1moist);

  doGETRequest(0);
  vTaskDelay(30000);
 
} 
 
}
//Task for board 2 
void board2(void*parameters){ 

for(;;){ 
   board2temp = nodes[1].temp;
   board2humid = nodes[1].humid;
   board2moist = nodes[1].moisture;
  // board2temp = 3.52;
  // board2humid = 3.54;
  // board2moist = 5;

  doGETRequest(1);
  vTaskDelay(30000);
} 
 
} 

//Task for board 3
void board3(void*parameters){ 

for(;;){ 
   board3temp = nodes[2].temp;
   board3humid = nodes[2].humid;
   board3moist = nodes[2].moisture;
  // board3temp = 3.52;
  // board3humid = 3.54;
  // board3moist = 5;

  doGETRequest(2);
  vTaskDelay(30000);
} 
} 

void board4(void*parameters){ 

for(;;){ 
   board4temp = nodes[3].temp;
   board4humid = nodes[3].humid;
   board4moist = nodes[3].moisture;
  // board4temp = 3.52;
  // board4humid = 3.54;
  // board4moist = 5;

  doGETRequest(3);
  vTaskDelay(30000);
 
} 
 
} 




void doGETRequest(int nodevar){

{ 
  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;  
switch (nodevar) {
  case 0:
    serverPath = serverName + "agric_data.php?nodeID=" + String(nodevar)+ "&temperature="+String(board1temp)+"&humidity="+String(board1humid)+"&moisture="+String(board1moist);
    break;
  case 1:
    serverPath = serverName + "agric_data.php?nodeID=" + String(nodevar)+ "&temperature="+String(board2temp)+"&humidity="+String(board2humid)+"&moisture="+String(board2moist);
    break;
  case 2:
    serverPath = serverName + "agric_data.php?nodeID=" + String(nodevar)+ "&temperature="+String(board3temp)+"&humidity="+String(board3humid)+"&moisture="+String(board3moist);
    break;
  case 3:
    serverPath = serverName + "agric_data.php?nodeID=" + String(nodevar)+ "&temperature="+String(board4temp)+"&humidity="+String(board4humid)+"&moisture="+String(board4moist);
    break;
  default:
    serverPath = serverName + "agric_data.php?nodeID=" + String(nodevar)+ "&temperature="+String(board1temp)+"&humidity="+String(board1humid)+"&moisture="+String(board1moist);
    break;
}    

     Serial.println(serverPath);
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    vTaskDelay(1000);
    }
}
