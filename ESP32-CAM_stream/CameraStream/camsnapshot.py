import cv2
import urllib.request
import numpy as np
import os
import time

# URL de la cámara IP
url = 'http://192.168.0.124/cam-hi.jpg'
cv2.namedWindow("live Cam Testing", cv2.WINDOW_AUTOSIZE)

# Crear la carpeta 'images' si no existe
output_dir = 'images'
os.makedirs(output_dir, exist_ok=True)

# Leer y mostrar frames del video
while True:
    # Leer un frame del flujo de video
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    im = cv2.imdecode(imgnp, cv2.IMREAD_COLOR)  # Usar IMREAD_COLOR para asegurarse de que se lee correctamente
    
    if im is None:
        print("Failed to decode the image")
        break

    # Mostrar el frame en una ventana
    cv2.imshow('live Cam Testing', im)

    # Esperar a que se presione la tecla 'q'
    key = cv2.waitKey(5)
    if key == ord('q'):
        # Generar un nombre de archivo único basado en el tiempo actual
        filename = os.path.join(output_dir, f'foto_{int(time.time())}.jpg')
        
        # Guardar la imagen en la carpeta 'images'
        if cv2.imwrite(filename, im):  # Verificar si la imagen se guardó correctamente
            print(f"Image saved as {filename}")
        else:
            print("Error saving the image")
        break

cv2.destroyAllWindows()
