String myName = "Sensora";

#include "_wifi.h" //import delle altre pagine
#include "_uovo.h"
#include "_mqtt.h"

int SENSOR_TIME = 5000; //ogni quanto mando i messaggi

void setup() {
  Serial.begin(115200); //porta seriale
  setup_wifi();//richiamo il setup di _wifi.h
  randomSeed(micros());
  mqttClient.setServer(mqtt_server, 1883);//invio messagio al Server 
  mqttClient.setCallback(mqttReceive);//risposta del server
  setup_uovo();//richiamo il setup di _uovo.h
}
void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
//connettersi sempre
  unsigned long now = millis();
  if (now - lastMsg > SENSOR_TIME) {
    lastMsg = now;
    int light = analogRead(A0);
    loop_uovo();
    DynamicJsonDocument doc(1024);//creazione Json
    doc["light"] =  lightMeter.readLightLevel();//valori inviati il Json su mqtt
    doc["temperature"] = sht30.cTemp;//valori inviati il Json su mqtt
    doc["humidity"] = sht30.humidity;//valori inviati il Json su mqtt
    doc["Vbattery"] = analogRead(A0) * 4.5 / 1024.0;//valori inviati il Json su mqtt
    String mqttPayload = "";
    serializeJson(doc, mqttPayload);
    Serial.println("Publish message: " + mqttTopicPubish_sensor + " " + mqttPayload);//pubblico in messaggio
    mqttClient.publish(mqttTopicPubish_sensor.c_str(), mqttPayload.c_str());
  }
  if (lightMeter.readLightLevel() > ValLuceAperto) {
    
    frigoAperto = true;
  }
  if (lightMeter.readLightLevel() <= ValLuceChiuso) {
    frigoAperto = false;
  }
  if (frigoAperto != OLDfrigoAperto) {
    OLDfrigoAperto = frigoAperto;
    DynamicJsonDocument doc(1024);
    doc["light"] =  lightMeter.readLightLevel();
    String mqttPayload = "";
    serializeJson(doc, mqttPayload);
    Serial.println("Publish message: " + mqttTopicPubish_event + " " + mqttPayload);
    mqttClient.publish(mqttTopicPubish_event.c_str(), mqttPayload.c_str());
  }
}
