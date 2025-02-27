#include <Arduino.h>

volatile int interruptCounter = 0;  // Contador de interrupciones
int totalInterruptCounter = -2;      // Total de interrupciones (empieza en 1)
hw_timer_t *timer = NULL;           // Puntero al temporizador
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;  // Mutex para protección de datos compartidos

// Función que se llama cuando el temporizador genera una interrupción
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);  // Entrar en sección crítica para evitar acceso concurrente
  interruptCounter++;  // Incrementar el contador de interrupciones
  portEXIT_CRITICAL_ISR(&timerMux);   // Salir de la sección crítica
}

void setup() {
  Serial.begin(9600);  // Iniciar comunicación serie
  timer = timerBegin(0, 80, true);  // Iniciar el temporizador (prescaler 80, temporizador 0)
 
  timerAttachInterrupt(timer, &onTimer, true);  // Asociar la función de interrupción al temporizador
  timerAlarmWrite(timer, 1000000, true);  // Establecer la alarma del temporizador para 1 segundo (1000000 microsegundos)
  timerAlarmEnable(timer);  // Habilitar la alarma del temporizador
}

void loop() {
  if (interruptCounter > 0) {  // Si se ha producido alguna interrupción
    portENTER_CRITICAL(&timerMux);  // Entrar en sección crítica
    interruptCounter--;  // Decrementar el contador de interrupciones (una vez procesada)
    portEXIT_CRITICAL(&timerMux);   // Salir de la sección crítica

    totalInterruptCounter++;  // Incrementar el contador total de interrupciones
    Serial.print("An interrupt has occurred. Total number: ");
    Serial.println(totalInterruptCounter);  // Imprimir el total de interrupciones
  }
}

