#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int pirPin = D2;
const int doorSensor = D3;
const int relayPin = D1;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(doorSensor, INPUT);
  pinMode(relayPin, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int motion = digitalRead(pirPin);
  int door = digitalRead(doorSensor);

  if (motion == HIGH) {
    client.publish("home/security", "Motion Detected");
  }

  if (door == HIGH) {
    client.publish("home/security", "Door Opened");
  }

  delay(2000);
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ArduinoClient");
  }
}
