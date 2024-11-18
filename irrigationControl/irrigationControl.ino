#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de la red Wi-Fi
const char* ssid = "IZZI-E002-2.4G";
const char* password = "189C275CE002";

// Configuración del broker MQTT
const char* mqttServer = "192.168.0.11";
const int mqttPort = 1883;

// Pines del LED
const int ledPin = 14;  // LED controlado por la humedad

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

    // Control del LED del pin 13 basado en el mensaje de ambos tópicos
    if (message =="ON") {
        mqttClient.publish("estadoRiego", "En proceso");
        digitalWrite(ledPin, HIGH);
        Serial.println("Activar bomba"); 
    } else if (message =="OFF") {
      mqttClient.publish("estadoRiego", "Completado"); 
        digitalWrite(ledPin, LOW);
        Serial.println("Apagar bomba");
    }
}

// Conectar al broker MQTT
void reconnect() {
    while (!mqttClient.connected()) {
        Serial.print("Intentando conectar al broker MQTT...");
        if (mqttClient.connect("broker_Alexa")) {
            Serial.println("Conectado a broker_Alexa");
            mqttClient.subscribe("controlBombaAgua");
            mqttClient.subscribe("datosHumedad");
            Serial.println("ESP32 suscrita a topicos");
            break;
        } else {
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

    // Configurar los pines del LED
    pinMode(ledPin, OUTPUT);  

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
    for(int i=0; i< 50;i++){
        Serial.println("EN EL LOOP");
        delay(100);
    }
    mqttClient.loop();
}
