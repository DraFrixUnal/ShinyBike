#include <Wire.h>

#define HC05_RX_PIN 3 // Pin RX del HC-05 conectado al pin 3 (GPIO3) del ESP32
#define HC05_TX_PIN 1 // Pin TX del HC-05 conectado al pin 1 (GPIO1) del ESP32
#define BUTTON_Right 27 // Pin del botón conectado al pin 12 (GPIO12) del ESP32
#define BUTTON_Left 12  //etc etc etc
#define BUTTON_Both 35
#define BUTTON_Night 34
#define LED_Right 32 
#define LED_Left 25

bool l = false;  //Asignacion de variables de control
bool r = false;
bool b = false;
unsigned long currentTime=0;
unsigned long previousTime=0;
int bp = 0;
bool button27 = false;
bool button12 = false;
bool button13 = false;
bool button14 = false; 

void setup() {
  Serial.begin(9600, SERIAL_8N1, HC05_RX_PIN, HC05_TX_PIN); // Inicia la comunicación serial con el HC-05
  pinMode(BUTTON_Right, INPUT_PULLUP); // Configura los pin de los botónes y leds como entrada como corresponda
  pinMode(BUTTON_Left, INPUT_PULLUP);
  pinMode(BUTTON_Both, INPUT_PULLUP);
  pinMode(BUTTON_Night, INPUT_PULLUP); 
  pinMode(LED_Right, OUTPUT);
  pinMode(LED_Left,OUTPUT);
}

void loop() {
  //Lectura
  button27 = digitalRead(BUTTON_Right);
  button12 = digitalRead(BUTTON_Left); // Lee el estado actual del botón
  button13 = digitalRead(BUTTON_Both);
  button14 = digitalRead(BUTTON_Night);

  //Modificacion de variables de control
  if (button27 == HIGH) {
    Serial.write(65);
    l = true;
    r = false;
    b = false;
  }
  if (button12 == HIGH) {
    Serial.write(66);
    r = true;  
    l = false;
    b = false;
  }
  if (button13 == HIGH) {
    Serial.write(67);
    delay(200);
    if(l == true || r == true || b == true){
      l = false; r = false; b = false;
    }else{
      b = true;
    }
  }
  if (button14 == HIGH) {
    Serial.write(68); 
  }


  //Decisiones
  if(r == true){
    digitalWrite(LED_Right,HIGH);
  }else{
    digitalWrite(LED_Right,LOW);
  }
  if(l == true){
    digitalWrite(LED_Left,HIGH);
  }else{
    digitalWrite(LED_Left,LOW);
  }
  if(b == true){
    currentTime = millis();
    if(bp == 0 && currentTime-previousTime > 1000){
      previousTime = currentTime;
      bp = 1;
    }else if(bp == 1 && currentTime-previousTime > 1000){
      previousTime = currentTime;
      bp = 0;
    }
    if(bp == 0){
      digitalWrite(LED_Right,HIGH);
      digitalWrite(LED_Left,HIGH);
    }
    if(bp == 1){
      digitalWrite(LED_Right,LOW);
      digitalWrite(LED_Left,LOW);
    }
  }
}


/*  
    #include <MPU6050.h>
    MPU6050 mpu;
  
  *En setup*

    Wire.begin();
    mpu.initialize();

  *Fuera de setup pero antes de loop*
    int16_t accelerometerXprev = mpu.getAccelerationX();
    int16_t accelerometerYprev = mpu.getAccelerationY();
    int16_t accelerometerZprev = mpu.getAccelerationZ();
    int16_t gyroscopeXprev = mpu.getRotationX();
    int16_t gyroscopeYprev = mpu.getRotationY();
    int16_t gyroscopeZprev = mpu.getRotationZ();

  *En loop*
    int16_t accelerometerX = mpu.getAccelerationX();
    int16_t accelerometerY = mpu.getAccelerationY();
    int16_t accelerometerZ = mpu.getAccelerationZ();
    int16_t gyroscopeX = mpu.getRotationX();
    int16_t gyroscopeY = mpu.getRotationY();
    int16_t gyroscopeZ = mpu.getRotationZ();

    if(accelerometerX < accelerometerXprev){
      ta frenando, aqui se prioriza prender todos los leds en rojo. Al dejar de frenar pasara al else 
    }else{
      //Codigo no documentado del loop

      Si r,l y b son falsos podemos entrar a utilizar la automatizacion del mpu6050
      if(r == false && l == false && b == false){
        if(gyroscopeX < -75){
          prender l
        }
        if(gyroscopeX > 75){
          prender r
        }
      }
    }

    Como se puede dar cuenta el mpu6050 debe calibrarse y puede ampliar sus funciones. Pero con que sepa cuando se frena
    o cuando se esta girando(sin dirrecionales) es suficiente por el momento.
    Tambien y es un poco obvio, al prender el led del controlador tambien se debe enviar el comando, el unico nuevo
    seria el del frendo, siendo Serial.write(69); :)
*/