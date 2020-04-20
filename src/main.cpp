#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>
#include <stdio.h>

//Bluetooth 
#include "BluetoothSerial.h"

char t_char [8];
char h_char [8];
char dallas_t_char [8];
int count =0;
String BTID = "ESP32_B001";

BluetoothSerial SerialBT;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
  }

  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected");
  }
}


#define DHTPIN 23     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
const int ledPin = 2; // ledPin refers to ESP32 GPIO 23
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 15
// Setup PT100 a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  sensors.begin();     //dallas start
  dht.begin();
  SerialBT.register_callback(callback);

  if(!SerialBT.begin(BTID)){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized");
  }
      // initialize digital pin ledPin as an output.


    delay(2000);
    
    digitalWrite(ledPin,  LOW);    // turn the LED off by making the voltage LOW
    
  
}

void loop() {
  // put your main code here, to run repeatedly:

    digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(15000);
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    dtostrf(h,2, 2,h_char); // convertion float to string
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    dtostrf(t,2, 1,t_char); // convertion float to string
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).


    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
    }
    else
    {

        // Create the payload for publishing
        sensors.requestTemperatures();
        float dallas_t = sensors.getTempCByIndex(0);
        dtostrf(dallas_t,2, 1,dallas_t_char); // convertion float to string
        sensors.requestTemperatures();
        dallas_t = sensors.getTempCByIndex(0);
        count++;
        // Serial.print("count = ");
        // Serial.println(count);
        // SerialBT.print("count = ");
        // SerialBT.println(count);
        // Serial.print("Dallas_t = ");
        // Serial.print(dallas_t);
        // Serial.println(" C");
        // SerialBT.print("Dallas_t = ");
        // SerialBT.print(dallas_t);
        // SerialBT.println(" C");

        // Serial.print("DHT_t = ");
        // Serial.print(t);
        // Serial.println(" C");
        // SerialBT.print("DHT_t = ");
        // SerialBT.print(t);
        // SerialBT.println(" C");
        // Serial.print("DHT_h = ");
        // Serial.print(h);
        // Serial.println(" %");
        // SerialBT.print("DHT_h = ");
        // SerialBT.print(h);
        // SerialBT.println( "%");
        //graphic display
        //SerialBT.print("count/Dallas_t/DHT_t/DHT_h");
        //SerialBT.print(",");
        //SerialBT.print(count);
        //SerialBT.print(",");
        
        SerialBT.print("  ;  ");
        SerialBT.print(BTID);
        SerialBT.print("  ;  ");
        SerialBT.print(dallas_t);
        SerialBT.print("  ;  ");
        SerialBT.print(t);
        SerialBT.print("  ;  ");
        SerialBT.println(h);
  


    }
    
    digitalWrite(ledPin,  LOW);
    delay(10000);
}