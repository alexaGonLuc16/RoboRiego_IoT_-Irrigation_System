from ultralytics import YOLO

try:
    # Cargar el modelo
    model = YOLO('best_10.pt')
    print("Modelo cargado correctamente.")

    # Ejecutar detección en la imagen y guardar los resultados
    results = model.predict(source="images/foto_1730857976.jpg", save=True, save_txt=True)
    print("Detección completada.")

except Exception as e:
    print("Ocurrió un error:", e)