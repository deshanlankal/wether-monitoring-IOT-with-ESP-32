#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Slt 4G";
const char* password = "12345678";

// Supabase API details
const char* supabaseUrl = "";
const char* supabaseKey = ""; // Use the anon public key

// IP Info API token
const char* ipinfoToken = ""; // Replace with your ipinfo.io token

void sendToSupabase(String city, String region, String country);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    // Initial data upload
    uploadLocationData();
}

void loop() {
    static unsigned long lastUpdate = 0;
    unsigned long currentMillis = millis();

    // Resend location data every 30 minutes
    if (currentMillis - lastUpdate >= 1800000) {
        lastUpdate = currentMillis;
        uploadLocationData();
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
    http.begin(supabaseUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", supabaseKey);
    http.addHeader("Authorization", String("Bearer ") + supabaseKey);

    // Create JSON payload
    String jsonPayload = "{\"city\":\"" + city + "\",\"region\":\"" + region + "\",\"country\":\"" + country + "\"}";
    Serial.println("Sending data to Supabase:");
    Serial.println(jsonPayload);

    int httpCode = http.POST(jsonPayload);
    if (httpCode > 0) {
        Serial.println("Response code: " + String(httpCode));
        String response = http.getString();
        Serial.println("Response: " + response);
    } else {
        Serial.println("Error sending data to Supabase. HTTP Code: " + String(httpCode));
    }

    http.end();
}
