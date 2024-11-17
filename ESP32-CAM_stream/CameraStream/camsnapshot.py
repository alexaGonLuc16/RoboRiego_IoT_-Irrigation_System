import cv2
import urllib.request
import numpy as np
import time
import os

# URL del stream de la ESP32-CAM
url = 'http://192.168.0.124/cam-hi.jpg'

# Crear la carpeta 'images' si no existe
if not os.path.exists('images'):
    os.makedirs('images')

# Mostrar el video de la cámara en una ventana
while True:
    # Obtener un frame de la cámara
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    im = cv2.imdecode(imgnp, cv2.IMREAD_COLOR)

    # Verificar si la imagen se cargó correctamente
    if im is None:
        print("No se pudo decodificar la imagen")
        break

    # Mostrar el frame en una ventana
    cv2.imshow('live Cam Testing', im)

    # Esperar a que se presione una tecla
    key = cv2.waitKey(5)

    # Guardar la imagen si se presiona 'q'
    if key == ord('q'):
        # Generar un nombre único para la imagen usando la marca de tiempo
        filename = f'images/foto_{int(time.time())}.jpg'
        
        # Guardar la imagen mostrada en la ventana
        cv2.imwrite(filename, im)
        print(f"Imagen guardada como {filename}")
        break

# Cerrar las ventanas de OpenCV
cv2.destroyAllWindows()
