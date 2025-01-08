#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

// Create an instance of the BMP180 sensor
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("BMP180 Sensor Test");

  // Initialize I2C with custom pins
  Wire.begin(21, 22); // SDA = GPIO 21, SCL = GPIO 22

  // Initialize the BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  } else {
    Serial.println("BMP180 sensor initialized successfully!");
  }
}

void loop() {
  // Get a new sensor event
  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure) {
    // Print barometric pressure
    Serial.print("Pressure: ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

    // Print temperature
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Calculate altitude (based on sea level pressure of 1013.25 hPa)
    float altitude = bmp.pressureToAltitude(1013.25, event.pressure);
    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" meters");
  } else {
    Serial.println("Sensor data unavailable.");
  }

  delay(2000); // Wait for 2 seconds before next reading
}
