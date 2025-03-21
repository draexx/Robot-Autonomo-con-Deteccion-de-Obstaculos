# Guía de Calibración del Robot con Detección de Obstáculos

## Introducción

Esta guía detalla el proceso de calibración necesario para optimizar el funcionamiento del Robot con Detección de Obstáculos. Una calibración adecuada garantiza que los sensores detecten correctamente la línea y que el robot responda de manera precisa a los obstáculos.
## Índice
- [Guía de Calibración del Robot con Detección de Obstáculos](#guía-de-calibración-del-robot-con-detección-de-obstáculos)
  - [Introducción](#introducción)
  - [Índice](#índice)
  - [1. Calibración del Sensor Ultrasónico](#1-calibración-del-sensor-ultrasónico)
    - [Herramientas necesarias](#herramientas-necesarias)
    - [Procedimiento](#procedimiento)
  - [2. Calibración de los Motores](#2-calibración-de-los-motores)
    - [Materiales necesarios:](#materiales-necesarios)
    - [Pasos:](#pasos)
  - [3. Calibración de los LEDs Indicadores](#3-calibración-de-los-leds-indicadores)
    - [Procedimiento](#procedimiento-1)
  - [4. Pruebas Finales](#4-pruebas-finales)
  - [5. Resolución de Problemas Comunes](#5-resolución-de-problemas-comunes)
    - [El robot no detecta la línea correctamente](#el-robot-no-detecta-la-línea-correctamente)
    - [El robot no responde a obstáculos](#el-robot-no-responde-a-obstáculos)
    - [Los LEDs no se encienden](#los-leds-no-se-encienden)
  - [6. Valores Óptimos Recomendados](#6-valores-óptimos-recomendados)

---

## 1. Calibración del Sensor Ultrasónico

### Herramientas necesarias
- Regla o cinta métrica
- Obstáculos de prueba

### Procedimiento

1. **Verificación de la distancia medida**
   - Coloca un obstáculo a una distancia conocida (por ejemplo, 10 cm) frente al sensor.
   - Utiliza el siguiente código para verificar la precisión de las mediciones:
   
   ```arduino
   void setup() {
     Serial.begin(9600);
     pinMode(TRIG_PIN, OUTPUT);
     pinMode(ECHO_PIN, INPUT);
   }
   
   void loop() {
     long duration, distance;
     digitalWrite(TRIG_PIN, LOW);
     delayMicroseconds(2);
     digitalWrite(TRIG_PIN, HIGH);
     delayMicroseconds(10);
     digitalWrite(TRIG_PIN, LOW);
     duration = pulseIn(ECHO_PIN, HIGH);
     distance = (duration / 2) / 29.1;
     Serial.print("Distancia: ");
     Serial.print(distance);
     Serial.println(" cm");
     delay(500);
   }
   ```

2. **Ajuste de parámetros de detección**
   - Modifica los valores `MIN_DISTANCE` y `OBSTACLE_CHECK_INTERVAL` según sea necesario:

   ```arduino
   #define MIN_DISTANCE 15      // Distancia mínima antes de considerar un obstáculo (cm)
   #define OBSTACLE_CHECK_INTERVAL 100  // Intervalo para comprobar obstáculos (ms)
   ```

---

## 2. Calibración de los Motores

### Materiales necesarios:
- Superficie plana y lisa.
- Cinta métrica.

### Pasos:
1. Coloca el robot sobre una superficie plana.
2. Enciende el robot y observa si avanza en línea recta.
3. Si el robot se desvía:
   - Ajusta la velocidad de los motores usando la función `setMotorSpeeds()` en el código.
   - Incrementa o reduce la velocidad de uno de los motores para compensar la desviación.
4. Repite el proceso hasta que el robot avance en línea recta.

---

## 3. Calibración de los LEDs Indicadores

### Procedimiento

1. **Verificación de funcionamiento**
   - Ejecuta la función `testLEDs()` incluida en el setup para comprobar que ambos LEDs funcionan correctamente.
   - Los LEDs deberían encenderse y apagarse secuencialmente.

2. **Ajuste de brillo (opcional)**
   - Si los LEDs son demasiado brillantes o tenues, puedes usar PWM para ajustar su intensidad:

   ```arduino
   // En lugar de:
   digitalWrite(LED_GREEN_PIN, HIGH);

   // Usa:
   analogWrite(LED_GREEN_PIN, 150); // Valor entre 0-255
   ```

---

## 4. Pruebas Finales

Una vez completada la calibración, realiza pruebas en diferentes entornos para asegurar que el robot funcione correctamente en todas las condiciones.

---

## 5. Resolución de Problemas Comunes

### El robot no detecta la línea correctamente

### El robot no responde a obstáculos
1. Comprueba que el sensor ultrasónico esté correctamente conectado.
2. Verifica que `DISTANCIA_MINIMA` sea apropiado para tu aplicación.
3. Asegúrate de que el LED rojo se encienda cuando se detecta un obstáculo.

### Los LEDs no se encienden
1. Verifica las conexiones de los LEDs y resistencias.
2. Comprueba que los pines definidos (`ledVerde` y `ledRojo`) coincidan con tu montaje físico.
3. Asegúrate de que las resistencias limitadoras sean del valor adecuado (220-330 ohms).

---

## 6. Valores Óptimos Recomendados

Para condiciones estándar (línea negra de 2cm de ancho sobre fondo blanco, iluminación interior normal), recomendamos los siguientes valores iniciales:

```arduino
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
```

Recuerda que estos valores pueden requerir ajustes dependiendo de:
- El tipo de superficie
- Las condiciones de iluminación
- El peso del robot
- El estado de la batería

Documenta tus valores calibrados para referencia futura.