//including the libraries
#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
#include <DHT22.h>

const char* transmit_id = "TX_01"; // Unique ID for this transmitter

//pin definition
#define DHTPIN 7
#define soilpin 5
#define lightsensor 4
int trigPin=35;
int echoPin=38;
#define redled 16
#define greenled 33
#define blueled 19
#define relaypin 18

//dht settings
#define DHTTYPE DHT22// declaring dht type either dht 22 or dht 11
DHT22 dht(DHTPIN);//defining an object where we will map the functions to. in this case our object dht


int pingTravelTime;
float pingTravelDistance;
float distanceToTarget;

float moisturepercentage;//variable for storing the moisture percentage
float lightintesitypercentage;//variable for storing the light intensity percentage
float soilmoisturecondition;///boolean to tell us if the moisture percentage is above our threshold
float lightintensitycondition;
float soiltemperaturecondition;
float tanklevel;

int soilthreshold=35;//moisture threshold value
int lightthreshold=35;//light intensity threshold value
int soiltemperaturethreshold=35;//soil temperature threshold value
float soilvalue;

float maximumwaterlevel=100;
float mediumwaterlevel=50;
float minimumwaterlevel=10;


/* Definitions */

#define ESPNOW_WIFI_CHANNEL 6

/* Classes */

// Creating a new class that inherits from the ESP_NOW_Peer class is required.

class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer {
public:
  // Constructor of the class using the broadcast address
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Broadcast_Peer() {
    remove();
  }

  // Function to properly initialize the ESP-NOW and register the broadcast peer
  bool begin() {
    if (!ESP_NOW.begin() || !add()) {
      log_e("Failed to initialize ESP-NOW or register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to send a message to all devices within the network
  bool send_message(const uint8_t *data, size_t len) {
    if (!send(data, len)) {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};


// Create a broadcast peer object
ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, nullptr);
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);//intializing the serial monitor
  Serial.println(F("DHTxx test!"));
  pinMode(soilpin,INPUT);//Making the soil pin as an input pin
  pinMode(relaypin, OUTPUT);// Making the relay pin as an output pin
  pinMode(lightsensor,INPUT);//Making the light sensor as an input pin
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(blueled,OUTPUT);
  pinMode(greenled,OUTPUT);
  pinMode(redled,OUTPUT);

  // dht.begin();//initializing the dht object we created earlier

    //Serial.begin(115200);

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Register the broadcast peer
  if (!broadcast_peer.begin()) {
    Serial.println("Failed to initialize broadcast peer");
    Serial.println("Reebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  // Serial.printf("ESP-NOW version: %d, max data length: %d\n", ESP_NOW.getVersion(), ESP_NOW.getMaxDataLen());

  Serial.println("Setup complete. Broadcasting messages every 5 seconds.");
}

void loop() {
  // put your main code here, to run repeatedly:
  tanklevel=checktanklevel();
  soilmoisturecondition=checksoilmoisture();
  delay(1000);
  lightintensitycondition=checklightcondition();
  delay(1000);
  soiltemperaturecondition=checksoiltemperature(); 
  delay(1000);
  broadcastingMessage(tanklevel,soilmoisturecondition,lightintensitycondition,soiltemperaturecondition);
}
double checksoiltemperature(){
    // Read temperature as Celsius (the default)
  float t = dht.getTemperature();
  Serial.print("soil tempt = ");
  Serial.println(t);
  delay (500);
  return t;

}
double checksoilmoisture(){
  Serial.print("soil level = ");
  Serial.println(analogRead(soilpin));
  delay (500);
  soilvalue=analogRead(soilpin);
  moisturepercentage=map(soilvalue,1023,0,0,100);//dry soil =1023 wet soil=0
  
  moisturepercentage = constrain(moisturepercentage, 0, 100);
  return moisturepercentage;
  
}
double checklightcondition(){
  Serial.print("light intensity = ");
  Serial.println(analogRead(lightsensor));
  delay (500);
  float light_Intensity_Value= analogRead(lightsensor);
  lightintesitypercentage = map(light_Intensity_Value,0,1023,0,100);//dark=0 light =1023
  lightintesitypercentage = constrain(lightintesitypercentage,0,100);
  return lightintesitypercentage;
}
double checktanklevel(){
  digitalWrite(trigPin,LOW);
delayMicroseconds(10);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
pingTravelTime=pulseIn(echoPin,HIGH);
delay(25);
distanceToTarget=(pingTravelTime/2)*0.343;//distance in mm
if (distanceToTarget<=maximumwaterlevel&&distanceToTarget>mediumwaterlevel){
  digitalWrite(greenled,HIGH);//turn on green if max level reached
  digitalWrite(redled,LOW);
  digitalWrite(blueled,LOW);
}
else if (distanceToTarget<=mediumwaterlevel&&distanceToTarget>minimumwaterlevel){
  digitalWrite(greenled,LOW);
  digitalWrite(redled,LOW);
  digitalWrite(blueled,HIGH);//turn on blue if medium level reached
}
else if (distanceToTarget<minimumwaterlevel){
  digitalWrite(greenled,LOW);
  digitalWrite(redled,HIGH);//turn on red if minimum level reached
  digitalWrite(blueled,LOW);
}
Serial.print("water level = ");
Serial.println(distanceToTarget);
return distanceToTarget;
}


void broadcastingMessage(float tanklevel,float soilmoisturecondition,float lightintesitycondition,float soiltemperaturecondition){
  // Broadcast a message to all devices within the network
  char data[128]; // increased from 32 — more data needs more space
    
  snprintf(data, sizeof(data), "TransmitID:%s,TankLevel:%.2f,SoilMoisture:%.3f,LightIntensity:%.3f,SoilTemperature:%.3f",
    transmit_id,
    tanklevel,
    soilmoisturecondition,
    lightintensitycondition,
    soiltemperaturecondition
  );

  Serial.printf("Broadcasting message: %s\n", data);

  if (!broadcast_peer.send_message((uint8_t *)data, sizeof(data))) {
    Serial.println("Failed to broadcast message");
  }

  delay(2500);
}