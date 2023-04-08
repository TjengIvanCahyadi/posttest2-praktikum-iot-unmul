#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <MQ2.h>

#define LED_PIN D1
#define BUZZER_PIN D2
#define DHT_PIN D3
#define DHT_TYPE DHT11
#define MQ2_ANALOG_PIN A0

#define BLYNK_AUTH_TOKEN "Y_mnlCkmKFhOIpLHfyyUsZw9nt0zyEa9"
#define BLYNK_PRINT Serial
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Universitas Mulawarman";  // type your wifi name
char pass[] = "";  // type your wifi password

BlynkTimer timer;

DHT dht(DHT_PIN, DHT_TYPE);

MQ2 mq2(MQ2_ANALOG_PIN);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  mq2.begin();
  timer.setInterval(500L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensor() {
  float temperature = dht.readTemperature();
  float lpgLevel = mq2.readLPG();
  float coLevel = mq2.readCO();
  float smokeLevel = mq2.readSmoke();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // send Temperature to blynk
  Blynk.virtualWrite(V1, temperature);
  // send LPG to blynk
  Blynk.virtualWrite(V2, lpgLevel);
  // send CO to blynk
  Blynk.virtualWrite(V3, coLevel);
  // send Smoke to blynk
  Blynk.virtualWrite(V4, smokeLevel);
  
  // Menampilkan Suhu ke serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Menampilkan Kadar LPG ke serial monitor
  Serial.print("Kadar LPG: ");
  Serial.print(lpgLevel);
  Serial.println(" ppm");

  // Menampilkan Kadar CO ke serial monitor
  Serial.print("Kadar CO: ");
  Serial.print(coLevel);
  Serial.println(" ppm");

  // Menampilkan Kadar Smoke ke serial monitor
  Serial.print("Kadar Smoke: ");
  Serial.print(smokeLevel);
  Serial.println(" ppm");

  Serial.println("");

  // Event log
    if (temperature > 29 || lpgLevel > 0.5 || coLevel > 0.5 || smokeLevel > 0.5) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);      
    }
}