#define trigPin 9
#define echoPin 10
#define flowPin 3

long duration;
float distance;
int tankHeight = 100;
float waterLevel;

int flowCount = 0;
bool lastFlowState = LOW;

// Analog sensor pins
int soilPin = A0;
int rainPin = A1;
int turbPin = A2;
int tdsPin = A3;
int tempPin = A4;
int humPin = A5;

// Smoothed water level
float smoothWater = 0;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(flowPin, INPUT);
}

void loop() {

  // Soil Moisture
  int soil = analogRead(soilPin);

  // Rain Sensor
  int rain = analogRead(rainPin);

  // Turbidity
  int turbidity = analogRead(turbPin);

  // TDS
  int tds = analogRead(tdsPin);
  float tds_ppm = tds * 0.5;

  // Temperature (LM35 simulated)
  float temperature = analogRead(tempPin) * 0.488;

  // Humidity (simulated)
  float humidity = map(analogRead(humPin), 0, 1023, 20, 90);

  // Ultrasonic Water Level
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration > 0) {
    distance = duration * 0.034 / 2;
    waterLevel = tankHeight - distance;

    if (waterLevel < 0) waterLevel = 0;
    if (waterLevel > tankHeight) waterLevel = tankHeight;

    smoothWater = 0.7 * smoothWater + 0.3 * waterLevel;
  }

  // Flow Sensor (edge detection)
  bool currentFlowState = digitalRead(flowPin);

  if (currentFlowState == HIGH && lastFlowState == LOW) {
    flowCount++;
  }
  lastFlowState = currentFlowState;

  // Groundwater (simulated)
  float groundwater = map(soil, 0, 1023, 0, 100);

  // Pressure (simulated)
  float pressure = map(rain, 0, 1023, 980, 1050);

  // Output
  Serial.println("===== SENSOR DATA =====");

  Serial.print("Soil Moisture: "); Serial.println(soil);
  Serial.print("Rain: "); Serial.println(rain);

  Serial.print("Temperature: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);

  Serial.print("Water Level (Smoothed): ");
  Serial.println(smoothWater);

  Serial.print("Flow Count: "); Serial.println(flowCount);

  Serial.print("Turbidity: "); Serial.println(turbidity);
  Serial.print("TDS (ppm): "); Serial.println(tds_ppm);

  Serial.print("Groundwater Level: "); Serial.println(groundwater);
  Serial.print("Pressure: "); Serial.println(pressure);

  Serial.println("========================\n");

  delay(1000);
}