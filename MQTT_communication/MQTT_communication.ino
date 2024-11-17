#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de la red Wi-Fi
const char* ssid = "steren_2_4G";
const char* password = "password";

// Configuración del broker MQTT
const char* mqttServer = "192.168.1.12";
const int mqttPort = 1883;

// Pines del LED
const int sensorPin = 36;
const int ledPin = 14;  // LED controlado por la humedad
const int ledPin2 = 13; // LED controlado por las dos computadoras

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
    if (message == "ON") {
        digitalWrite(ledPin2, HIGH);
        Serial.println("LED del pin 13 encendido");
    } else if (message == "OFF") {
        digitalWrite(ledPin2, LOW);
        Serial.println("LED del pin 13 apagado");
    }

    // Control de riego (recepción de tipo de flor)
    if (message == "Rosa") {
        Serial.println(message, " detectado(a)");
    } else if (message == "Girasol") {
        Serial.println(message, " detectado");
    }
}

// Conectar al broker MQTT
void reconnect() {
    while (!mqttClient.connected()) {
        Serial.print("Intentando conectar al broker MQTT...");
        if (mqttClient.connect("broker_Alexa")) {
            Serial.println("Conectado a broker_Alexa");
            mqttClient.subscribe("laptopMensaje");//motores
            mqttClient.subscribe("flor/deteccion");
            mqttClient.subscribe("laptopRonnie");//creo que esta linea no es  necesaria(topico de sensor)
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

    int sensorValue = map(analogRead(sensorPin), 4095, 2500, 0, 100);
    Serial.print(sensorValue);
    Serial.println("%");
    mqttClient.loop();
    
    String str = String(sensorValue);
      
    // Control del LED del pin 14 basado en la humedad
    if (sensorValue > 70) {
        digitalWrite(ledPin, LOW);
        Serial.println("Humedad suficiente");
    } else if (sensorValue < 70) {
        digitalWrite(ledPin, HIGH);
        Serial.println("Humedad insuficiente");
    }
    
    mqttClient.publish("datos_humedad", str.c_str()); 
    Serial.println(analogRead(sensorPin));
    delay(3000);  
}
