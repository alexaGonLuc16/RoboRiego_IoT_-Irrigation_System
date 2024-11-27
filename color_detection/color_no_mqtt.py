import cv2
import numpy as np

imagen = cv2.imread('images/capture_flower.jpg')

# Convertir la imagen a espacio de color HSV
hsv = cv2.cvtColor(imagen, cv2.COLOR_BGR2HSV)

# Definir rangos estrictos para rojo intenso
# Primer rango de rojo (Hue cerca de 0)
rojo_intenso_bajo1 = np.array([0, 150, 150])  # Hue bajo, saturación y valor altos
rojo_intenso_alto1 = np.array([5, 255, 255])  # Hasta 5 para evitar naranja

# Segundo rango de rojo (Hue cerca de 180)
rojo_intenso_bajo2 = np.array([175, 150, 150])  # Valores altos para evitar tonos apagados
rojo_intenso_alto2 = np.array([180, 255, 255])

# Crear máscaras para ambos rangos
mascara_rojo1 = cv2.inRange(hsv, rojo_intenso_bajo1, rojo_intenso_alto1)
mascara_rojo2 = cv2.inRange(hsv, rojo_intenso_bajo2, rojo_intenso_alto2)
# Combinar las máscaras
mascara_rojo_intenso = cv2.bitwise_or(mascara_rojo1, mascara_rojo2)

# Definir los rangos de colores en HSV para detectar el color amarillo
amarillo_bajo = np.array([20, 40, 40])
amarillo_alto = np.array([35, 255, 255])
mascara_amarillo = cv2.inRange(hsv, amarillo_bajo, amarillo_alto)

# Contar los píxeles en cada máscara
conteo_rojo = cv2.countNonZero(mascara_rojo_intenso)
conteo_amarillo = cv2.countNonZero(mascara_amarillo)

# Determinar la flor detectada y enviar los datos a Node-RED mediante MQTT
if conteo_amarillo > conteo_rojo:
    mensaje = "Girasol"
    print("Girasol detectado")
elif conteo_rojo > conteo_amarillo:
    mensaje = "Rosa"
    print("Rosa detectada")    
else:
    mensaje = "No detectada"
    print("No se detectaron flores específicas")

# Mostrar los resultados (opcional)
cv2.imshow('Original', imagen)
cv2.imshow('Flores Rojas (Rosas)', cv2.bitwise_and(imagen, imagen, mask=mascara_rojo_intenso))
cv2.imshow('Flores Amarillas', cv2.bitwise_and(imagen, imagen, mask=mascara_amarillo))
#return mensaje

# Esperar y cerrar ventanas
cv2.waitKey(0)
cv2.destroyAllWindows()