// Pines de control para Motor 1
const int in1Pin1 = 5;  // Pin de dirección 1 del motor 1
const int in2Pin1 = 18; // Pin de dirección 2 del motor 1
const int enablePin1 = 19; // Pin de velocidad PWM del motor 1

// Pines de control para Motor 2
const int in1Pin2 = 21; // Pin de dirección 1 del motor 2
const int in2Pin2 = 22; // Pin de dirección 2 del motor 2
const int enablePin2 = 23; // Pin de velocidad PWM del motor 2

// Variables de configuración
const int velocidad = 150;         // Velocidad del motor (0 a 255 para PWM)
const int tiempoMovimiento = 2000; // Tiempo que los motores estarán activos en milisegundos
const int tiempoPausa = 5000;      // Tiempo de pausa entre movimientos

void setup() {
  // Configura los pines como salida
  pinMode(in1Pin1, OUTPUT);
  pinMode(in2Pin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);

  pinMode(in1Pin2, OUTPUT);
  pinMode(in2Pin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  Serial.begin(9600);

  // Ejecuta el movimiento hacia adelante dos veces
  for (int i = 0; i < 2; i++) {
    moverAdelante();
    delay(tiempoMovimiento);  // Mantiene el motor encendido por el tiempo especificado

    detener();
    delay(tiempoPausa);       // Pausa entre movimientos
  }

  // Movimiento en reversa después de las dos veces en adelante
  moverAtras();
  delay(tiempoMovimiento); // Tiempo para moverse en reversa
  
  detener(); // Detiene los motores después del movimiento en reversa
}

void loop() {
  // El loop queda vacío ya que no necesitamos ejecutar nada en bucle.
}

// Función para mover los motores hacia adelante
void moverAdelante() {
  digitalWrite(in1Pin1, HIGH);
  digitalWrite(in2Pin1, LOW);
  analogWrite(enablePin1, velocidad);

  digitalWrite(in1Pin2, HIGH);
  digitalWrite(in2Pin2, LOW);
  analogWrite(enablePin2, velocidad);
}

// Función para mover los motores en reversa
void moverAtras() {
  digitalWrite(in1Pin1, LOW);
  digitalWrite(in2Pin1, HIGH);
  analogWrite(enablePin1, velocidad);

  digitalWrite(in1Pin2, LOW);
  digitalWrite(in2Pin2, HIGH);
  analogWrite(enablePin2, velocidad);
}

// Función para detener los motores
void detener() {
  analogWrite(enablePin1, 0);
  analogWrite(enablePin2, 0);
}