// GPIO pins for Hall Effect Sensors (via Logic Level Shifter)
#define SENSOR1_PIN 33
#define SENSOR2_PIN 25
#define SENSOR3_PIN 26
#define SENSOR4_PIN 27

// GPIO pins for LEDs
#define LED1_PIN 32
#define LED2_PIN 14
#define LED3_PIN 12
#define LED4_PIN 13

void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Testing Hall Effect Sensors with LEDs and Logic Level Shifter");

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
}

void loop() {
  // Read sensor states
  int sensor1State = digitalRead(SENSOR1_PIN);
  int sensor2State = digitalRead(SENSOR2_PIN);
  int sensor3State = digitalRead(SENSOR3_PIN);
  int sensor4State = digitalRead(SENSOR4_PIN);

  // Print sensor states to Serial Monitor
  Serial.print("Sensor 1: "); Serial.print(sensor1State);
  Serial.print(" | Sensor 2: "); Serial.print(sensor2State);
  Serial.print(" | Sensor 3: "); Serial.print(sensor3State);
  Serial.print(" | Sensor 4: "); Serial.println(sensor4State);

  // Control LEDs based on sensor states
  // Turn on LED if the corresponding sensor detects a magnet (state LOW)
  digitalWrite(LED1_PIN, !sensor1State);
  digitalWrite(LED2_PIN, !sensor2State);
  digitalWrite(LED3_PIN, !sensor3State);
  digitalWrite(LED4_PIN, !sensor4State);

  delay(500); // Delay for readability
}
