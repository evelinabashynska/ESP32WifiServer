#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <DHT.h>

// --- WiFi ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- MQTT ---
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "esp32/temperature";

// --- DHT ---
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- Об'єкти ---
WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

// --- Функція для з'єднання з Wi-Fi ---
void connectToWiFi() {
  Serial.print("🔌 Підключення до WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" ✅ WiFi OK!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// --- MQTT Reconnect ---
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("🔁 Підключення до MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" ✅ MQTT OK!");
    } else {
      Serial.print("❌ Failed. Код: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// --- Веб-сторінка ---
void handleRoot() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  String html = "<h1>🌡 ESP32 Температура</h1>";
  html += "<p>Температура: " + String(t) + " °C</p>";
  html += "<p>Вологість: " + String(h) + " %</p>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  connectToWiFi();


  server.on("/", handleRoot);
  server.begin();
  Serial.println("🌐 Веб-сервер запущено");

  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
  server.handleClient();

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    String payload = "{\"temp\":" + String(t) + ", \"hum\":" + String(h) + "}";
    client.publish(mqtt_topic, payload.c_str());
    Serial.println("📤 MQTT Publish: " + payload);
  } else {
    Serial.println("⚠️ Неможливо зчитати з сенсора");
  }

  delay(5000);
}
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <DHT.h>

// --- WiFi ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- MQTT ---
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "esp32/temperature";

// --- DHT ---
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- Об'єкти ---
WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

// --- Функція для з'єднання з Wi-Fi ---
void connectToWiFi() {
  Serial.print("🔌 Підключення до WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" ✅ WiFi OK!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// --- MQTT Reconnect ---
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("🔁 Підключення до MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" ✅ MQTT OK!");
    } else {
      Serial.print("❌ Failed. Код: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// --- Веб-сторінка ---
void handleRoot() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  String html = "<h1>🌡 ESP32 Температура</h1>";
  html += "<p>Температура: " + String(t) + " °C</p>";
  html += "<p>Вологість: " + String(h) + " %</p>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  connectToWiFi();


  server.on("/", handleRoot);
  server.begin();
  Serial.println("🌐 Веб-сервер запущено");

  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
  server.handleClient();

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    String payload = "{\"temp\":" + String(t) + ", \"hum\":" + String(h) + "}";
    client.publish(mqtt_topic, payload.c_str());
    Serial.println("📤 MQTT Publish: " + payload);
  } else {
    Serial.println("⚠️ Неможливо зчитати з сенсора");
  }

  delay(5000);
}
