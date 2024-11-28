#include <WiFi.h>
#include <ThingSpeak.h>

// Datos de tu red WiFi y canal ThingSpeak
const char* ssid = "TU_SSID";
const char* password = "TU_CONTRASEÑA_WIFI";
const unsigned long channelID = TU_CANAL_ID;
const char* writeAPIKey = "TU_API_KEY";

// Definir pines de los sensores
const int mq135Pin = 34;  // MQ135 a pin analógico
const int mq2Pin = 35;    // MQ2 a pin analógico
const int mq6Pin = 32;    // MQ6 a pin analógico
const int lm35Pin = 33;   // LM35 a pin analógico

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
  
  ThingSpeak.begin(client);
}

void loop() {
  // Leer los valores de los sensores
  int mq135Value = analogRead(mq135Pin);
  int mq2Value = analogRead(mq2Pin);
  int mq6Value = analogRead(mq6Pin);
  int lm35Value = analogRead(lm35Pin);

  // Convertir los valores de LM35 a temperatura en Celsius (aproximación)
  float temperature = (lm35Value / 1024.0) * 5.0 * 100.0;

  // Enviar los datos a ThingSpeak
  ThingSpeak.setField(1, mq135Value);
  ThingSpeak.setField(2, mq2Value);
  ThingSpeak.setField(3, mq6Value);
  ThingSpeak.setField(4, temperature);

  int httpCode = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (httpCode == 200) {
    Serial.println("Datos enviados con éxito");
  } else {
    Serial.println("Error al enviar datos");
  }

  delay(20000); // Enviar cada 20 segundos
}