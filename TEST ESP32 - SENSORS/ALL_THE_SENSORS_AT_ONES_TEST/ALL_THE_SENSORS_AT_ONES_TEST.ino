#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h> // For BMP180

#include <DHT.h>

// GPIO pins for Hall Effect Sensors (via Logic Level Shifter)
#define SENSOR1_PIN 33  // North
#define SENSOR2_PIN 25  // South
#define SENSOR3_PIN 26  // East
#define SENSOR4_PIN 27  // West

// GPIO pins for LEDs
#define LED1_PIN 32  // North LED
#define LED2_PIN 14  // South LED
#define LED3_PIN 12  // East LED
#define LED4_PIN 13  // West LED

// DHT Sensor pin and type
#define DHT_PIN 4     // Pin where the DHT sensor is connected
#define DHT_TYPE DHT11 // DHT11 or DHT22 depending on your sensor

// LDR Module pin
#define LDR_PIN 34 // Pin connected to the LDR module's DO pin

// Rain Sensor pins
#define RAIN_DO_PIN 35 // Digital pin for rain sensor
#define RAIN_AO_PIN 36 // Analog pin for rain sensor

DHT dht(DHT_PIN, DHT_TYPE); // Initialize DHT sensor
Adafruit_BMP085 bmp;        // Initialize BMP180 sensor

void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Testing Sensors: Wind, DHT11, BMP180, LDR, and Rain Sensor");

  // Set sensor pins as input
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);

  // Set LED pins as output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  // Set LDR pin as input
  pinMode(LDR_PIN, INPUT);

  // Set Rain Sensor pins
  pinMode(RAIN_DO_PIN, INPUT); // Digital pin for rain sensor

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check connections!");
    while (1);
  }
}

void loop() {
  // Read sensor states for wind direction
  int sensor1State = digitalRead(SENSOR1_PIN);  // North
  int sensor2State = digitalRead(SENSOR2_PIN);  // South
  int sensor3State = digitalRead(SENSOR3_PIN);  // East
  int sensor4State = digitalRead(SENSOR4_PIN);  // West

  // Determine wind direction based on sensor states
  String windDirection = "No Wind";  // Default

  if (sensor1State == LOW) {
    windDirection = "North";
    digitalWrite(LED1_PIN, HIGH);
  } else {
    digitalWrite(LED1_PIN, LOW);
  }

  if (sensor2State == LOW) {
    windDirection = "South";
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED2_PIN, LOW);
  }

  if (sensor3State == LOW) {
    windDirection = "East";
    digitalWrite(LED3_PIN, HIGH);
  } else {
    digitalWrite(LED3_PIN, LOW);
  }

  if (sensor4State == LOW) {
    windDirection = "West";
    digitalWrite(LED4_PIN, HIGH);
  } else {
    digitalWrite(LED4_PIN, LOW);
  }

  // Print wind direction to Serial Monitor
  Serial.print("Wind Direction: ");
  Serial.println(windDirection);

  // Read temperature and humidity from the DHT sensor
  float humidity = dht.readHumidity();
  float temperatureDHT = dht.readTemperature(); // In Celsius

  if (!isnan(humidity) && !isnan(temperatureDHT)) {
    Serial.print("DHT11 Temperature: ");
    Serial.print(temperatureDHT);
    Serial.print(" °C | Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Read pressure, temperature, and altitude from BMP180
  float temperatureBMP = bmp.readTemperature();
  float pressure = bmp.readPressure();       // In Pascals
  float altitude = bmp.readAltitude(101325); // Assuming sea-level pressure 101325 Pa

  Serial.print("BMP180 Temperature: ");
  Serial.print(temperatureBMP);
  Serial.print(" °C | Pressure: ");
  Serial.print(pressure / 100); // Convert to hPa
  Serial.print(" hPa | Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  // Read LDR state
  int ldrState = digitalRead(LDR_PIN); // HIGH if light is detected, LOW otherwise
  Serial.print("LDR State: ");
  if (ldrState == HIGH) {
    Serial.println("TRUE (Light Detected)");
  } else {
    Serial.println("FALSE (No Light Detected)");
  }

  // Read Rain Sensor state
  int rainDigital = digitalRead(RAIN_DO_PIN); // Digital reading (rain/no rain)
  int rainAnalog = analogRead(RAIN_AO_PIN);   // Analog reading (rain intensity)

  Serial.print("Rain Sensor Digital: ");
  if (rainDigital == LOW) {
    Serial.println("TRUE (Rain Detected)");
  } else {
    Serial.println("FALSE (No Rain)");
  }

  Serial.print("Rain Sensor Analog: ");
  Serial.print(rainAnalog);
  Serial.println(" (Rain Intensity)");

  delay(1000); // Delay for readability
}
