#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h> // For BMP180
#include <DHT.h>

// Supabase credentials
const char* supabaseWeatherDataUrl = "";
const char* supabaseWeatherLocationUrl = "";
const char* supabaseKey = "";

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// IP Info API token
const char* ipinfoToken = ""; // Replace with your ipinfo.io token

// GPIO pins for sensors and LEDs
#define SENSOR1_PIN 33  // North
#define SENSOR2_PIN 25  // South
#define SENSOR3_PIN 26  // East
#define SENSOR4_PIN 27  // West
#define LED1_PIN 32     // North LED
#define LED2_PIN 14     // South LED
#define LED3_PIN 12     // East LED
#define LED4_PIN 13     // West LED
#define DHT_PIN 4
#define DHT_TYPE DHT11
#define LDR_PIN 34
#define RAIN_DO_PIN 35
#define RAIN_AO_PIN 36

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  // Initialize sensors
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor!");
    while (1);
  }

  // Set sensor and LED pins
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(RAIN_DO_PIN, INPUT);

  // Initial location data upload
  uploadLocationData();
}

void loop() {
  static unsigned long lastWeatherDataUpdate = 0;
  static unsigned long lastLocationUpdate = 0;
  unsigned long currentMillis = millis();

  // Send weather data every 5 seconds
  if (currentMillis - lastWeatherDataUpdate >= 5000) {
    lastWeatherDataUpdate = currentMillis;
    sendWeatherData();
  }

  // Resend location data every 30 minutes
  if (currentMillis - lastLocationUpdate >= 1800000) {
    lastLocationUpdate = currentMillis;
    uploadLocationData();
  }
}

void sendWeatherData() {
  // Read sensor states for wind direction
  int sensor1State = digitalRead(SENSOR1_PIN);
  int sensor2State = digitalRead(SENSOR2_PIN);
  int sensor3State = digitalRead(SENSOR3_PIN);
  int sensor4State = digitalRead(SENSOR4_PIN);

  // Determine wind direction
  String windDirection = "No Wind";
  if (sensor1State == LOW) windDirection = "North";
  if (sensor2State == LOW) windDirection = "South";
  if (sensor3State == LOW) windDirection = "East";
  if (sensor4State == LOW) windDirection = "West";

  // Control LEDs
  digitalWrite(LED1_PIN, (windDirection == "North") ? HIGH : LOW);
  digitalWrite(LED2_PIN, (windDirection == "South") ? HIGH : LOW);
  digitalWrite(LED3_PIN, (windDirection == "East") ? HIGH : LOW);
  digitalWrite(LED4_PIN, (windDirection == "West") ? HIGH : LOW);

  // Read temperature and humidity
  float temperatureDHT = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read BMP180 data
  float temperatureBMP = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0; // Convert to hPa
  float altitude = bmp.readAltitude(101325);

  // Read LDR state
  int ldrState = digitalRead(LDR_PIN);

  // Read rain sensor
  int rainDigital = digitalRead(RAIN_DO_PIN);
  int rainAnalog = analogRead(RAIN_AO_PIN);

  // Log sensor data
  Serial.println("Weather Data:");
  Serial.print("Temperature (DHT): "); Serial.println(temperatureDHT);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Temperature (BMP): "); Serial.println(temperatureBMP);
  Serial.print("Pressure: "); Serial.println(pressure);
  Serial.print("Altitude: "); Serial.println(altitude);
  Serial.print("LDR State: "); Serial.println(ldrState);
  Serial.print("Rain (Digital): "); Serial.println(rainDigital);
  Serial.print("Rain (Analog): "); Serial.println(rainAnalog);
  Serial.print("Wind Direction: "); Serial.println(windDirection);

  // Send data to Supabase
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(supabaseWeatherDataUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", supabaseKey);
    http.addHeader("Authorization", "Bearer " + String(supabaseKey));

    // JSON payload
    StaticJsonDocument<256> doc;
    doc["tempOut"] = temperatureBMP;
    doc["humOut"] = humidity;
    doc["pressure"] = pressure;
    doc["rainAnalog"] = rainAnalog;
    doc["rainDigital"] = rainDigital == LOW;
    doc["airQuality"] = 42.5; // Placeholder for air quality sensor
    doc["windDirection"] = windDirection;
    doc["altitude"] = altitude;
    doc["ldrStatus"] = ldrState == HIGH;

    String payload;
    serializeJson(doc, payload);

    // HTTP POST
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      Serial.print("Weather Data Response Code: ");
      Serial.println(httpResponseCode);
      Serial.println("Response: " + http.getString());
    } else {
      Serial.print("POST Error: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected!");
  }
}

void uploadLocationData() {
  HTTPClient http;
  String url = String("http://ipinfo.io/json?token=") + ipinfoToken;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("IP Info Response:");
    Serial.println(payload);

    // Parse JSON using ArduinoJson
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.println("Failed to parse JSON:");
      Serial.println(error.c_str());
      return;
    }

    String city = doc["city"] | "Unknown City";
    String region = doc["region"] | "Unknown Region";
    String country = doc["country"] | "Unknown Country";

    Serial.println("Parsed Location:");
    Serial.println("City: " + city);
    Serial.println("Region: " + region);
    Serial.println("Country: " + country);

    // Send data to Supabase
    sendToSupabase(city, region, country);
  } else {
    Serial.println("Error getting IP info. HTTP Code: " + String(httpCode));
  }

  http.end();
}

void sendToSupabase(String city, String region, String country) {
  HTTPClient http;
  http.begin(supabaseWeatherLocationUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabaseKey);
  http.addHeader("Authorization", String("Bearer ") + supabaseKey);

  // Create JSON payload
  String jsonPayload = "{\"city\":\"" + city + "\",\"region\":\"" + region + "\",\"country\":\"" + country + "\"}";
  Serial.println("Sending data to Supabase:");
  Serial.println(jsonPayload);

  int httpCode = http.POST(jsonPayload);
  if (httpCode > 0) {
    Serial.println("Location Response Code: " + String(httpCode));
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error sending data to Supabase. HTTP Code: " + String(httpCode));
  }

  http.end();
}
