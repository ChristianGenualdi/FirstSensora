#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* mqtt_server = "172.16.206.91";

const char* mqtt_user = "mosquitto";
const char* mqtt_password = "dietpi";

String thingId = "DMO_" + String(WiFi.macAddress().c_str());

String mqttTopicPubish_sensor = "iort2023/" + myName + "/sensor";
String mqttTopicSubscibe_config = "iort2023/" + myName + "/config";
String mqttTopicPubish_event = "iort2023/" + myName + "/event";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastMsg = 0;

void mqttReceive(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0;
  String Payload = String((char*)payload);
  String Topic = String(topic);

  Serial.println("Message arrived " + Topic + " " + Payload );

  if (Topic == mqttTopicSubscibe_config) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, Payload);
    //impostare i parametri della temperatura
    alertTemperature = doc["temperature"];
    //impostare i parametri del buzzer da node-red
    frequency  = doc["frequency"];
    duration = doc["duration"];
       //impostare i parametri del controllo luce da node-red
    ValLuceAperto = doc["ValLuceAperto"];
    ValLuceChiuso = doc["ValLuceChiuso"];
  }
}
void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe(mqttTopicSubscibe_buzzer.c_str());
      mqttClient.subscribe(mqttTopicSubscibe_config.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
