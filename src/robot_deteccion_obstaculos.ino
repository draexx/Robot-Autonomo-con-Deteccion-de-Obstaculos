#include <Servo.h>

// Parámetros configurables
#define DISTANCIA_MINIMA 10      // Distancia mínima para detectar un obstáculo (en cm)
#define ANGULO_CENTRO 90         // Ángulo central del servo (en grados)
#define ANGULO_IZQUIERDA 180     // Ángulo para mirar a la izquierda (en grados)
#define ANGULO_DERECHA 0         // Ángulo para mirar a la derecha (en grados)
#define VELOCIDAD_AVANCE 120     // Velocidad de avance normal (0-255)
#define VELOCIDAD_AVANCE_LENTA 80 // Velocidad de avance reducida después del giro (0-255)
#define VELOCIDAD_GIRO 80        // Velocidad de giro (0-255)
#define TIEMPO_GIRO 300          // Tiempo de giro reducido (en ms)
#define TIEMPO_ESPERA_SERVO 300  // Tiempo de espera reducido para que el servo se mueva (en ms)
#define TIEMPO_FRENADO 100       // Tiempo de frenado suave (en ms)
#define TIEMPO_AVANCE 100        // Tiempo de avance entre verificaciones de distancia (en ms)

// Definición de pines
const int trigPin = 2;      // Pin Trig del HC-SR04
const int echoPin = 3;      // Pin Echo del HC-SR04
const int servoPin = 9;     // Pin del servo
const int in1 = 4;          // IN1 del L298N
const int in2 = 5;          // IN2 del L298N
const int in3 = 7;          // IN3 del L298N
const int in4 = 8;          // IN4 del L298N
const int enA = 6;          // ENA del L298N
const int enB = 11;         // ENB del L298N
const int ledVerde = A0;    // Pin del LED verde
const int ledRojo = A1;     // Pin del LED rojo

// Variables
Servo servo;
int distanciaFrente = 0;
int distanciaIzquierda = 0;
int distanciaDerecha = 0;

void setup() {
  // Configuración de pines
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(ledVerde, OUTPUT);
    pinMode(ledRojo, OUTPUT);

  // Inicializar servo
    servo.attach(servoPin);
    servo.write(ANGULO_CENTRO);  // Posición central del servo

  // Iniciar comunicación serial (para depuración)
    Serial.begin(9600);
}

void loop() {
  // Medir distancia al frente
    distanciaFrente = medirDistancia();

  // Mostrar distancia en el monitor serial
    Serial.print("Distancia al frente: ");
    Serial.println(distanciaFrente);

  // Lógica de movimiento
    if (distanciaFrente < DISTANCIA_MINIMA) {
        // Encender LED rojo y apagar LED verde
        digitalWrite(ledRojo, HIGH);
        digitalWrite(ledVerde, LOW);

        // Frenado suave
        detener();

        // Escanear el entorno
        distanciaIzquierda = escanear(ANGULO_IZQUIERDA);
        distanciaDerecha = escanear(ANGULO_DERECHA);

        // Tomar una decisión (priorizar giro a la derecha)
        if (distanciaDerecha > DISTANCIA_MINIMA) {
        girarDerecha();
        } else if (distanciaIzquierda > DISTANCIA_MINIMA) {
        girarIzquierda();
        } else {
        // Si no hay espacio a la derecha ni a la izquierda, retroceder y girar 180 grados
        retroceder();
        girar180();
        }
        delay(TIEMPO_GIRO);  // Tiempo de giro reducido

        // Avanzar lentamente después del giro
        avanzarLentamente();
    } else {
        // Encender LED verde y apagar LED rojo
        digitalWrite(ledVerde, HIGH);
        digitalWrite(ledRojo, LOW);

        // Asegurarse de que el servo esté en la posición central
        servo.write(ANGULO_CENTRO);

        // Avanzar
        avanzar();
    }
}

// Función para medir la distancia con el HC-SR04
int medirDistancia() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duracion = pulseIn(echoPin, HIGH);
    int distancia = duracion * 0.034 / 2;  // Convertir a cm
    return distancia;
}

// Función para escanear en una dirección específica
int escanear(int angulo) {
    servo.write(angulo);            // Girar servo al ángulo especificado
    delay(TIEMPO_ESPERA_SERVO);     // Esperar a que el servo se mueva
    int distancia = medirDistancia();  // Medir distancia
    servo.write(ANGULO_CENTRO);     // Volver a la posición central
    delay(TIEMPO_ESPERA_SERVO);     // Esperar a que el servo se mueva
    return distancia;
}

// Función para avanzar
void avanzar() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, VELOCIDAD_AVANCE);  // Velocidad motor A
    analogWrite(enB, VELOCIDAD_AVANCE);  // Velocidad motor B
}

// Función para avanzar lentamente después del giro
void avanzarLentamente() {
    unsigned long tiempoInicio = millis();
    while (millis() - tiempoInicio < TIEMPO_AVANCE) {
        distanciaFrente = medirDistancia();
        if (distanciaFrente < DISTANCIA_MINIMA) {
        // Si hay un obstáculo, detenerse
        detener();
        return;
        }
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        analogWrite(enA, VELOCIDAD_AVANCE_LENTA);  // Velocidad reducida motor A
        analogWrite(enB, VELOCIDAD_AVANCE_LENTA);  // Velocidad reducida motor B
        delay(50);  // Pequeña pausa entre verificaciones
    }
}

// Función para frenar suavemente
void frenarSuavemente() {
    int velocidadActual = VELOCIDAD_AVANCE;
    while (velocidadActual > 0) {
        analogWrite(enA, velocidadActual);  // Reducir velocidad motor A
        analogWrite(enB, velocidadActual);  // Reducir velocidad motor B
        velocidadActual -= 10;              // Disminuir la velocidad gradualmente
        delay(TIEMPO_FRENADO);              // Pequeña pausa entre reducciones
    }
    detener();  // Detener completamente los motores
}

// Función para detener
void detener() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);  // Detener motor A
    analogWrite(enB, 0);  // Detener motor B
}

// Función para girar a la izquierda
void girarIzquierda() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, VELOCIDAD_GIRO);  // Velocidad motor A
    analogWrite(enB, VELOCIDAD_GIRO);  // Velocidad motor B
}

// Función para girar a la derecha
void girarDerecha() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, VELOCIDAD_GIRO);  // Velocidad motor A
    analogWrite(enB, VELOCIDAD_GIRO);  // Velocidad motor B
}

// Función para retroceder
void retroceder() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, VELOCIDAD_GIRO);  // Velocidad motor A
    analogWrite(enB, VELOCIDAD_GIRO);  // Velocidad motor B
    delay(TIEMPO_GIRO);                // Tiempo de retroceso
}

// Función para girar 180 grados
void girar180() {
    girarDerecha();  // Girar a la derecha durante el doble del tiempo de giro normal
    delay(TIEMPO_GIRO * 2);  // Tiempo de giro para 180 grados
}