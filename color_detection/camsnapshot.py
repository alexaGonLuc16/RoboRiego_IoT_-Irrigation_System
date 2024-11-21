import cv2
import urllib.request
import numpy as np
import os
import paho.mqtt.client as mqtt

# Función que se ejecuta cuando te conectas al broker MQTT
def on_connect(client, userdata, flags, reasonCode, properties=None):
    print(f"Conectado con código de resultado {reasonCode}")
    client.subscribe("tomarFoto")

# Función que se ejecuta cuando se recibe un mensaje en un tópico suscrito
def on_message(client, userdata, msg):
    print(f"Mensaje recibido en {msg.topic}: {msg.payload.decode()}")

    # Si el mensaje es 'ON', tomar la foto
    if msg.payload.decode().strip().upper() == 'ON':
        try:
            print("Tomando foto...")
            img_resp = urllib.request.urlopen(url)
            imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
            im = cv2.imdecode(imgnp, cv2.IMREAD_COLOR)

            if im is not None:
                # Crear la carpeta 'images' si no existe
                if not os.path.exists('images'):
                    os.makedirs('images')

                # Generar un nombre único para la imagen usando la marca de tiempo
                filename = f'images/capture_flower.jpg'

                # Guardar la imagen
                cv2.imwrite(filename, im)
                print(f"Imagen guardada como {filename}")
            else:
                print("No se pudo decodificar la imagen.")
        except Exception as e:
            print(f"Error al tomar la foto: {e}")

# Configuración del cliente MQTT
client = mqtt.Client()

# Asignación de funciones de callback
client.on_connect = on_connect
client.on_message = on_message

# Conexión al broker MQTT (puedes cambiar el host y puerto si es necesario)
client.connect("192.168.1.16", 1883, 60)

# Inicio de un hilo para manejar la red y las callbacks
client.loop_start()

# URL del stream de la ESP32-CAM
url = 'http://192.168.1.17/cam-hi.jpg'

# Mantener el script corriendo
try:
    print("Esperando mensajes MQTT...")
    while True:
        pass  # Mantener el script corriendo
except KeyboardInterrupt:
    print("Cerrando cliente MQTT...")
    client.loop_stop()
    client.disconnect()
