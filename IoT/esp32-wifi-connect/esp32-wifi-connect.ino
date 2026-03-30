#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Replace with your Wi-Fi credentials
const char* ssid = "";
const char* password = "";
const char* api_key = "gnHaF6c1PwkGoOvjMJ";

// API endpoint
const char* server_url = "https://smart-home-iot.angazaelimu.com/api/dht_data_insert";

/*
 * server auth CA certificate key
 * ------DO NOT EDIT--------
 */
const char* root_cacert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
"-----END CERTIFICATE-----\n" ;

// ==================== SENSORS CONFIG PINS ====================
#define DHTPIN 7       // DHT connected to digital pin 7
#define ONBOARD_LED_PIN 9
#define OLED_RESET -1   // OLED reset (not used)
#define I2C_SCL 2       //SCL
#define I2C_SDA 42      //SDA

// ==================== OBJECT INITIALIZATION ====================
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// WiFi secure client
WiFiClientSecure* client = new WiFiClientSecure;

void setup() {
  Serial.begin(9600);
  delay(1000);
  dht.begin(); // initialize the DHT sensor

  // Signal LED pin function and state init
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  digitalWrite(ONBOARD_LED_PIN, HIGH); // Turn off LED initially

  // Initialize I2C with custom SDA and SCL
  Wire.begin(I2C_SDA, I2C_SCL);  // Important fix for custom I2C pins

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed to initialize.");
    while (1);
  }
  display.display();
  delay(2000);

  // WiFi network init
  connectToWiFi();

  // Set the root certificate
  client->setCACert(root_cacert);
}

void loop() {
  static unsigned long lastPostTime = 0;
  unsigned long currentTime = millis();

  // Check if 1/2 minute has passed since last post
  if (currentTime - lastPostTime >= 30000) {
    lastPostTime = currentTime;

    // Read sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    updateOLED(temperature, humidity);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Post data to server
    postSensorData(temperature, humidity);
  }

  // Small delay to prevent watchdog timer issues
  delay(100);
}

// Handle wifi connection
void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    blinkLED(250); // Blink LED while connecting
  }

  digitalWrite(ONBOARD_LED_PIN, LOW); // Solid LED when connected
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Handle data publishing
void postSensorData(float temperature, float humidity) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Cannot post data.");
    connectToWiFi(); // Attempt to reconnect
    return;
  }

  if (!client) {
    Serial.println("[HTTPS] Unable to create client");
    return;
  }

  HTTPClient https;
  if (https.begin(*client, server_url)) {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String payload = "dht_sensor_API_KEY=" + String(api_key) +
                    "&temperature=" + String(temperature) +
                    "&humidity=" + String(humidity);

    Serial.println("Posting data: " + payload);

    int httpCode = https.POST(payload);
    String response = https.getString();

    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);
    Serial.print("Full Response: ");
    Serial.println(response);

    // Parse and display the important fields from the response data
    parseAndDisplayResponse(response);

    https.end();
  } else {
    Serial.println("[HTTPS] Unable to connect to server");
  }
}

// Handle the response and extract important data
void parseAndDisplayResponse(String jsonResponse) {
  // Parse the JSON response
  JSONVar resObject = JSON.parse(jsonResponse);

  // Check if parsing succeeded
  if (JSON.typeof(resObject) == "undefined") {
    Serial.println("Failed to parse JSON response");
    return;
  }

  // Extract the values
  int user_id = resObject["user_id"];
  int project_id = resObject["project_id"];
  int temp_threshold = (const int)(resObject["temp_threshold"]);
  int humid_threshold = (const int)(resObject["humid_threshold"]);

  // Display the extracted values in a formatted way
  Serial.println("\n=== Project Data ===");
  Serial.print("User ID:        "); Serial.println(user_id);
  Serial.print("Project ID:     "); Serial.println(project_id);
  Serial.print("Project API Key:     "); Serial.println(api_key);
  Serial.print("Temp Threshold: "); Serial.print(temp_threshold); Serial.println("°C");
  Serial.print("Humid Threshold: "); Serial.print(humid_threshold); Serial.println("%");
  Serial.println("=============================\n");
}

// Handle onboard led blink
void blinkLED(int duration) {
  digitalWrite(ONBOARD_LED_PIN, LOW);
  delay(duration);
  digitalWrite(ONBOARD_LED_PIN, HIGH);
  delay(duration);
}

// ==================== FUNCTION TO UPDATE OLED ====================
void updateOLED(float temperature, float humidity) {
  display.clearDisplay();

  // Title
  display.setTextSize(1); // Smaller title to save space
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Smart System");

  // Divider line
  display.setCursor(0, 10);
  display.println("--------------------");

  // Temperature
  display.setTextSize(2); // Larger text for key values
  display.setCursor(0, 20);
  display.print("T: ");
  display.print(temperature, 1); // 1 decimal place
  display.println("C");

  // Humidity
  display.setCursor(0, 45);
  display.print("H: ");
  display.print(humidity, 1);
  display.println("%");

  display.display();
}
