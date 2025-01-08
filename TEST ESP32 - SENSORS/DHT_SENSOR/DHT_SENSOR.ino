#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Define the DHT11 sensor type
#define DHTTYPE DHT11
#define DHTPIN  5 // GPIO pin connected to the DHT11 data pin

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  Serial.println("DHT11 Sensor Reading - ESP32");

  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Read temperature and humidity values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if the readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the readings to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}