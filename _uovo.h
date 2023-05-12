#include <WEMOS_SHT3X.h>
#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#define BUZZER D5

SHT3X sht30(0x45);
BH1750 lightMeter;

int alertTemperature = 30;
int frequency = 1000;
int duration = 2000;
int ValLuceChiuso = 20;
int ValLuceAperto = 80;

void setup_uovo() {
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin();
  pinMode(BUZZER, OUTPUT);
}
int OLDfrigoAperto = -1;

int frigoAperto;

void loop_uovo() {
  /*luce */
  float lux = lightMeter.readLightLevel();
  Serial.print("Light : ");
  Serial.println(lux);
  /* temperatura */
  if (sht30.get() == 0) {
    Serial.print("Temperature in Celsius : ");
    Serial.println(sht30.cTemp);
    Serial.print("Relative Humidity : ");
    Serial.println(sht30.humidity);
    Serial.print("Battery : ");
    Serial.println(analogRead(A0) * 4.5 / 1024.0);
    Serial.println();
  }
  else
  {
    Serial.println("Error!");
  }
  if (sht30.cTemp >= alertTemperature) {
    tone(BUZZER, frequency, duration);  //emette una nota di 1000Hz x 2000 ms
    delay(1000);              //Attende 1 secondi
    noTone(BUZZER);           //Arresta l'emissione
    delay(1000);              //Attende 1 secondo
  }
}
