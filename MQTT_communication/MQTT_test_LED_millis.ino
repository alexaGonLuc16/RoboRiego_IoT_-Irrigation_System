#include <WiFi.h>
#include <PubSubClient.h>
const int sensorPin = 36;
// Configuración de la red Wi-Fi
const char* ssid = "steren_2_4G";
const char* password = "password";


// Configuración del broker MQTT
const char* mqttServer = "192.168.1.12";
const int mqttPort = 1883;

// Pines del LED
const int ledPin = 14; //14
const int ledPin2 = 13; //13 Led boton

// Instancias de cliente Wi-Fi y MQTT
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


// Función de callback para mensajes MQTT
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensaje recibido en el tópico: ");
    Serial.println(topic);

    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Mensaje: ");
    Serial.println(message);

    //control del led basado en el mensaje
    if(message == "ON"){
      digitalWrite(ledPin2, HIGH);
      Serial.println("Imprime on");
    }
    else if(message == "OFF"){
      digitalWrite(ledPin2, LOW);
      Serial.println("Imprime off");
    }
    else{
      Serial.println("Imprime");
    }
    
}

// Conectar al broker MQTT
void reconnect() {
    while (!mqttClient.connected()) {
        Serial.print("Intentando conectar al broker MQTT...");
        if (mqttClient.connect("broker_Alexa")) {
            Serial.println("Conectado");
            mqttClient.subscribe("laptopMensaje");
            mqttClient.subscribe("ControlLed");
            
        }
        else {
            Serial.print("Fallido, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Intentando de nuevo en 5 segundos");
            delay(5000);
        }
        if (mqttClient.connect("broker_celular")) {
            Serial.println("Conectado");
            mqttClient.subscribe("celularMensaje");
        }  
        else {
            Serial.print("Fallido, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Intentando de nuevo en 5 segundos");
            delay(5000);
        }
    }
}

void setup() {
    // Inicializar el puerto serie
    Serial.begin(115200);

    // Configurar el pin del LED
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);  

    // Conectar a la red Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Conectado a Wi-Fi");

    // Configurar el servidor MQTT
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(callback);
}

void loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }

    int sensorValue = map(analogRead(sensorPin),0, 4092, 0, 100);

    tiempoActual= millis();
    if(tiempoActual-tiempoAnterior > 5000){
      Serial.print(sensorValue);
      Serial.println("%"); 
      tiempoAnterior = millis();
    }
    
    mqttClient.loop();
    String str=String(sensorValue);    
    //control del led basado en la humedad
    if(sensorValue >70){
      digitalWrite(ledPin, LOW);
      if(tiempoActual - tiempoAnterior > 5000){
        Serial.println("Humedad Suficiente");
      }
    }
    else if(sensorValue < 70 && tiempoActual%5){
      digitalWrite(ledPin, HIGH);
      if(tiempoActual - tiempoAnterior > 10000){
        Serial.println("Humedad Insuficiente");  
      }
    }
    
    mqttClient.publish("datos_humedad",str.c_str());   
 }
