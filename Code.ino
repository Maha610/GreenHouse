#include <LiquidCrystal.h>
#include <DHT.h>

// LCD Pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Relay control pins
const int LIGHT_PIN = 6;  // Relay for Light
const int MOTOR_PIN = 7;  // Relay for Water Pump
const int FAN_PIN = 5;    // Relay for Fan
const int HUMIDITER_PIN =4;

// DHT11 Sensor
#define DHTPIN A0         // Digital pin connected to DHT11
#define DHTTYPE DHT11     // DHT 11 sensor type
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize LCD
  lcd.begin(20, 4);
  lcd.print("    GREEN HOUSE    ");
  
  // Set relay pins as OUTPUT
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(HUMIDITER_PIN,OUTPUT);
  
  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature(); // in Celsius
  float humidity = dht.readHumidity();       // in percentage
  
  // Read light intensity from LDR (connected to A1)
  int light = (analogRead(A1) / 1024.0) * 100.0; // Scale to 0-100%

  // Read soil moisture from Soil Sensor (connected to A2)
  int soil = (analogRead(A2) / 1024.0) * 100.0;
  
  // Check if DHT readings failed
  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0, 3);
    lcd.print("DHT Sensor Error!");
    delay(2000);
    return;
  }

  // Display values on LCD
  lcd.setCursor(0, 1);
  lcd.print(" T=");
  lcd.print((int)temperature); // Display integer value
  lcd.print("C");

  lcd.setCursor(11, 1);
  lcd.print("L=");
  lcd.print(light);
  lcd.print("% ");

  lcd.setCursor(0, 2);
  lcd.print(" S=");
  lcd.print(soil);
  lcd.print("%");

  lcd.setCursor(11, 2);
  lcd.print("H=");
  lcd.print((int)humidity); // Display integer value
  lcd.print("%");

  // Control devices based on sensor values
  digitalWrite(LIGHT_PIN, (light < 20) ? HIGH : LOW);  // Turn light ON if light < 20%
  digitalWrite(MOTOR_PIN, (soil < 35) ? HIGH : LOW);   // Turn pump ON if soil < 35%
  digitalWrite(FAN_PIN, (temperature > 25) ? HIGH : LOW); // Turn fan ON if temp > 25°C
  digitalWrite(HUMIDITER_PIN, (humidity > 60) ? HIGH : LOW); // Turn humiditer ON if humidity > 60%
  
  // Display device status on the fourth row
  lcd.setCursor(0, 3);
  lcd.print("L:"); lcd.print((light < 20) ? "ON " : "OFF");
  lcd.print("M:"); lcd.print((soil < 35) ? "ON " : "OFF");
  lcd.print("F:"); lcd.print((temperature > 25) ? "ON " : "OFF");
  lcd.print("H:"); lcd.print((humidity > 60) ? "ON" : "OFF");
  delay(2000); // Update every 2 seconds (DHT11 has a 1Hz sampling rate)
}