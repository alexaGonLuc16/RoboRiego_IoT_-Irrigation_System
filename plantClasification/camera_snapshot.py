import cv2 
import time
import subprocess

# Iniciar la cámara utilizando la URL de la ESP32-CAM con "/stream"
cam = cv2.VideoCapture("http://192.168.1.17:81/stream")

# Verificar si la cámara se ha abierto correctamente
if not cam.isOpened():
    print("No se pudo abrir la cámara")
    exit()

# Mostrar el video de la cámara en una ventana
while True:
    # Leer un cuadro de la cámara
    ret, frame = cam.read()

    # Si no se capturó correctamente el cuadro, salir del bucle
    if not ret:
        print("No se pudo recibir el cuadro")
        break

    # Mostrar el cuadro en la ventana
    cv2.imshow("Presiona 's' para tomar una foto", frame)

    # Esperar a que se presione una tecla
    key = cv2.waitKey(1)

    # Si se presiona 's', guardar la imagen con un nombre único
    if key == ord('s'):
        # Generar un nombre único usando la marca de tiempo
        filename = f"foto_{int(time.time())}.jpg"
        cv2.imwrite(filename, frame)
        print(f"Foto guardada como '{filename}'")

        # Ejecuta el segundo archivo y pasa el nombre de la imagen como argumento
        subprocess.run(["python", "analizar_foto.py", filename])
        break

    # Si se presiona 'q', salir sin guardar la imagen
    elif key == ord('q'):
        print("Salida sin guardar la imagen")
        break

# Liberar la cámara y cerrar las ventanas
cam.release()
cv2.destroyAllWindows()
