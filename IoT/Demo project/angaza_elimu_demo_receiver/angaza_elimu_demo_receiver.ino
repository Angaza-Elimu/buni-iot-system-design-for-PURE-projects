#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>
#include <vector>

/* Definitions */
#define ESPNOW_WIFI_CHANNEL 6

/* Global Variables — parsed sensor data stored here */
float tankLevel          = 0.0;
float soilMoisture       = 0.0;
float lightIntensity     = 0.0;
float soilTemperature    = 0.0;
char received_transmit_id[32] = "None"; // To show which transmitter sent the data
int soilmoisturethreshold=35;

int relaypin=20;
/* Classes */
class ESP_NOW_Peer_Class : public ESP_NOW_Peer {
public:
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk)
    : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  ~ESP_NOW_Peer_Class() {}

  bool add_peer() {
    if (!add()) {
      log_e("Failed to register the broadcast peer");
      return false;
    }
    return true;
  }

  // Called automatically when a message is received from the master
  void onReceive(const uint8_t *data, size_t len, bool broadcast) {
    Serial.printf("\nReceived from master " MACSTR " (%s)\n",
      MAC2STR(addr()), broadcast ? "broadcast" : "unicast");

    // Cast received bytes to a string
    char received[128];
    memset(received, 0, sizeof(received));
    memcpy(received, data, len < sizeof(received) ? len : sizeof(received) - 1);

    Serial.printf("Raw message: %s\n", received);

    // Parse the string into individual variables
    // Expected format: "TransmitID:TX_01,TankLevel:45.23,SoilMoisture:67.123,LightIntensity:23.45,SoilTemperature:28.3"
    sscanf(received,
      "TransmitID:%[^,],TankLevel:%f,SoilMoisture:%f,LightIntensity:%f,SoilTemperature:%f",
      received_transmit_id,
      &tankLevel,
      &soilMoisture,
      &lightIntensity,
      &soilTemperature
    );

    // Print parsed values
    Serial.println("Parsed values:");
    Serial.printf("  Transmit ID     : %s\n", received_transmit_id);
    Serial.printf("  Tank Level      : %.3f mm\n", tankLevel);
    Serial.printf("  Soil Moisture   : %.2f %%\n", soilMoisture);
    Serial.printf("  Light Intensity : %.2f %%\n", lightIntensity);
    Serial.printf("  Soil Temperature: %.2f C\n",  soilTemperature);
  }
};

/* Global list of masters */
std::vector<ESP_NOW_Peer_Class *> masters;

/* Callback — called when a message arrives from an unknown/new peer */
void register_new_master(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) {
    Serial.printf("Unknown peer " MACSTR " sent a broadcast message\n", MAC2STR(info->src_addr));
    Serial.println("Registering as master...");

    ESP_NOW_Peer_Class *new_master = new ESP_NOW_Peer_Class(
      info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, nullptr
    );

    if (!new_master->add_peer()) {
      Serial.println("Failed to register new master");
      delete new_master;
      return;
    }

    masters.push_back(new_master);
    Serial.printf("Master registered. Total masters: %zu\n", masters.size());

  } else {
    log_v("Received unicast from " MACSTR " — ignoring", MAC2STR(info->src_addr));
  }
}

/* setup */
void setup() {
  Serial.begin(9600);
  pinMode(relaypin,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Slave — Waiting for master...");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW. Rebooting in 5s...");
    delay(5000);
    ESP.restart();
  }

  ESP_NOW.onNewPeer(register_new_master, nullptr);

  Serial.println("Setup complete. Listening for broadcasts...");
}

/* loop */
void loop() {
    // Debug print every 1 seconds
  static unsigned long last_debug = 0;
  if (millis() - last_debug > 1000) {
    last_debug = millis();
    Serial.printf("\n--- Readings from %s ---\n", received_transmit_id);
    Serial.printf("Tank Level      : %.2f mm\n", tankLevel);
    Serial.printf("Soil Moisture   : %.3f %%\n", soilMoisture);
    Serial.printf("Light Intensity : %.3f %%\n", lightIntensity);
    Serial.printf("Soil Temperature: %.3f C\n",  soilTemperature);
            
  }
  // Example: print a warning if tank is low
  while (soilMoisture<soilmoisturethreshold) {
    Serial.println("WARNING: Soil moisture critically low!");
    digitalWrite(relaypin,HIGH);
  }
  delay(2000);
}