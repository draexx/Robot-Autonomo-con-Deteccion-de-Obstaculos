## **Informe Técnico: Robot con Detección de Obstáculos y Navegación Autónoma**

### **1. Introducción**
- **Objetivo del proyecto:**
  Desarrollar un robot autónomo capaz de detectar obstáculos mediante un sensor de ultrasonidos HC-SR04, tomar decisiones basadas en las mediciones y navegar en un entorno desconocido.
- **Aplicaciones:**
  Este tipo de robot puede ser utilizado en aplicaciones como limpieza autónoma, exploración de entornos peligrosos o como plataforma educativa para enseñar conceptos de robótica y programación.

---

### **2. Descripción del Proyecto**
- **Componentes principales:**
  - **Microcontrolador:** Arduino (por ejemplo, Arduino Uno).
  - **Sensor de ultrasonidos:** HC-SR04 para medir distancias.
  - **Servo motor:** Para mover el sensor y escanear el entorno.
  - **Puente H:** L298N para controlar los motores DC.
  - **Motores DC:** Dos motores para el movimiento del robot.
  - **LEDs:** Para indicar el estado del robot (verde: avanzando, rojo: detectando obstáculos).
  - **Batería:** Para alimentar el robot.

- **Funcionamiento general:**
  El robot avanza en línea recta mientras mide la distancia al frente con el sensor HC-SR04. Si detecta un obstáculo a menos de una distancia mínima (configurable), se detiene, escanea el entorno girando el servo motor y decide girar a la izquierda, a la derecha o retroceder según el espacio disponible.

---

### **3. Diseño y Implementación**
#### **3.1. Esquema de conexiones**
- **Sensor HC-SR04:**
  - `VCC` → 5V (Arduino).
  - `GND` → GND (Arduino).
  - `Trig` → Pin digital 2 (Arduino).
  - `Echo` → Pin digital 3 (Arduino).

- **Servo motor:**
  - `VCC` → 5V (Arduino).
  - `GND` → GND (Arduino).
  - `Señal` → Pin digital 9 (Arduino).

- **Puente H L298N:**
  - `IN1` → Pin digital 4 (Arduino).
  - `IN2` → Pin digital 5 (Arduino).
  - `IN3` → Pin digital 7 (Arduino).
  - `IN4` → Pin digital 8 (Arduino).
  - `ENA` → Pin digital 6 (Arduino).
  - `ENB` → Pin digital 11 (Arduino).
  - `VCC` → Batería (9V o pack de pilas).
  - `GND` → GND (Arduino y batería).

- **LEDs:**
  - `LED verde` → Pin analógico A0 (Arduino).
  - `LED rojo` → Pin analógico A1 (Arduino).

#### **3.2. Lógica de funcionamiento**
- **Avance:**
  El robot avanza en línea recta a una velocidad constante (`VELOCIDAD_AVANCE`).
- **Detección de obstáculos:**
  Si la distancia al frente es menor que `DISTANCIA_MINIMA`, el robot se detiene y escanea el entorno.
- **Toma de decisiones:**
  - Si hay espacio a la derecha, gira a la derecha.
  - Si no hay espacio a la derecha pero sí a la izquierda, gira a la izquierda.
  - Si no hay espacio en ninguna dirección, retrocede y gira 180 grados.
- **Indicación visual:**
  - `LED verde`: El robot está avanzando.
  - `LED rojo`: El robot ha detectado un obstáculo y está tomando una decisión.

---

### **4. Código Fuente**
- **Lenguaje de programación:** C++ (Arduino IDE).
- **Estructura del código:**
  - **Setup:** Configuración inicial de pines y comunicación serial.
  - **Loop:** Lógica principal del robot (medición de distancia, toma de decisiones y control de motores).
  - **Funciones personalizadas:**
    - `medirDistancia()`: Mide la distancia al frente con el HC-SR04.
    - `escanear()`: Gira el servo motor y mide la distancia en una dirección específica.
    - `avanzar()`, `girarIzquierda()`, `girarDerecha()`, `retroceder()`, `detener()`: Controlan el movimiento del robot.
    - `frenarSuavemente()`: Reduce gradualmente la velocidad antes de detenerse.

- **Fragmento de código clave:**
  ```cpp
  void loop() {
    distanciaFrente = medirDistancia();
    if (distanciaFrente < DISTANCIA_MINIMA) {
      frenarSuavemente();
      distanciaIzquierda = escanear(ANGULO_IZQUIERDA);
      distanciaDerecha = escanear(ANGULO_DERECHA);
      if (distanciaDerecha > DISTANCIA_MINIMA) {
        girarDerecha();
      } else if (distanciaIzquierda > DISTANCIA_MINIMA) {
        girarIzquierda();
      } else {
        retroceder();
        girar180();
      }
      delay(TIEMPO_GIRO);
      avanzarLentamente();
    } else {
      avanzar();
    }
  }
  ```

---

### **5. Pruebas y Resultados**
- **Pruebas realizadas:**
  - Verificación de la medición de distancia con el HC-SR04.
  - Pruebas de movimiento (avance, giro, retroceso).
  - Pruebas de detección de obstáculos y toma de decisiones.
- **Resultados:**
  - El robot es capaz de detectar obstáculos y tomar decisiones de manera autónoma.
  - La velocidad de avance reducida después del giro mejora la detección de obstáculos.
  - El frenado suave evita movimientos bruscos y mejora la estabilidad.

---

### **6. Conclusiones**
- **Logros:**
  - Se implementó un robot autónomo con detección de obstáculos y navegación básica.
  - El proyecto demostró ser una plataforma efectiva para aprender conceptos de robótica y programación.
- **Limitaciones:**
  - El robot puede tener dificultades en entornos con obstáculos muy cercanos o en laberintos complejos.
  - La precisión del sensor HC-SR04 puede verse afectada por superficies irregulares o ruido ambiental.
- **Mejoras futuras:**
  - Implementar un algoritmo de seguimiento de paredes más avanzado.
  - Añadir más sensores (por ejemplo, sensores infrarrojos) para mejorar la detección de obstáculos.
  - Integrar un sistema de navegación basado en mapas.

---

### **7. Anexos**
- **Diagrama de conexiones:** Incluir un esquema gráfico de las conexiones entre los componentes.
- **Fotos del proyecto:** Imágenes del robot ensamblado y en funcionamiento.
- **Enlaces útiles:**
  - Documentación del sensor HC-SR04.
  - Tutoriales de Arduino y puente H L298N.

---

### **8. Referencias**
- Documentación oficial de Arduino: https://www.arduino.cc/
- Datasheet del sensor HC-SR04.
- Manual del puente H L298N.

