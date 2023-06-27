#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>    

#define LED_PIN 6
#define LED_PIN2 7
#define NUM_LEDS 1

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); //Definimos la tira led, en nuestro caso una tira casera
Adafruit_NeoPixel strip2(NUM_LEDS, LED_PIN2, NEO_GRB + NEO_KHZ800); //Definimos la tira led, en nuestro caso una tira casera

SoftwareSerial miBT(10,11);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

bool mode = false; //Asignacion de variables de control
bool r = false;
bool l = false;
bool b = false;
int bp = 0;
unsigned long currentTime = 0;
unsigned long previousTime = 0;

void setup()
{
  Serial.begin(9600);   // Inicializamos  el puerto serie  
  Serial.println("listo");
  miBT.begin(9600);       // Inicializamos el puerto serie BT (Para Modo AT 2)velocidad= 38400
  strip.begin(); //Iniciamos las tiras led
  strip.show(); 
  strip2.begin();
  strip2.show(); 
}
 
void loop(){
  if(miBT.available()){    // Si llega un dato por el puerto BT se analiza y decide
      //Aqui se notara que el codigo es muy parecido al del Controlador :)
      char c = miBT.read();
      if(c == 'A' || c == 65){
        Serial.print(c);
        r = true;
        l = false;
        b = false;
      }
      if(c == 'B' || c == 66){
        Serial.print(c);
        l = true;
        r = false;
        b = false;
      }
      if(c == 'C' || c == 67){
        Serial.print(c);
        delay(200);
        if(l == true || r == true || b == true){
          l = false; r = false; b = false;
        }else{
          b = true;
        }
      }
      if(c == 'D' || c == 68){
        Serial.print(c);
        if(mode){mode = false;}else{mode = true;}
      }
  }
  if(mode == true){
      strip.setPixelColor(0, strip.Color(0, 0, 255));  
      strip2.setPixelColor(0, strip.Color(0, 0, 255));  
      if(l == true){
        strip2.setPixelColor(0, strip.Color(0, 255, 0));
        strip.setPixelColor(0, strip.Color(0, 0, 255));  
      }else{
        strip.setPixelColor(0, strip.Color(0, 0, 255));  
        strip2.setPixelColor(0, strip.Color(0, 0, 255));  
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
          strip.setPixelColor(0, strip.Color(0, 255, 0));
          strip2.setPixelColor(0, strip.Color(0, 255, 0));
        }
        if(bp == 1){
          strip.setPixelColor(0, strip.Color(0, 0, 255));  
          strip2.setPixelColor(0, strip.Color(0, 0, 255));
        }
      }

   }else{
      if(l == true){
        strip2.setPixelColor(0, strip.Color(0, 255, 0));
        strip.setPixelColor(0, strip.Color(0, 0, 0));  
      }else{
        strip.setPixelColor(0, strip.Color(0, 0, 0));  
        strip2.setPixelColor(0, strip.Color(0, 0, 0));  
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
          strip.setPixelColor(0, strip.Color(0, 255, 0));
          strip2.setPixelColor(0, strip.Color(0, 255, 0));
        }
        if(bp == 1){
          strip.setPixelColor(0, strip.Color(0, 0, 0));  
          strip2.setPixelColor(0, strip.Color(0, 0, 0));
        }
      }
   }
    if(r == true){
      strip.setPixelColor(0, strip.Color(0, 255, 0));  
    }else{
      if(b == false){
        if(mode == true){
          strip.setPixelColor(0, strip.Color(0, 0, 255)); 
        }else{
          strip.setPixelColor(0, strip.Color(0, 0, 0)); 
        }
      }
    }
    strip.show();
    strip2.show();
}


/*
  En la creacion de la tira led en el codigo es importante ver que uno puede determinar la cantidad de leds de la tira
  Esto es importante para diseños finales, donde podrian variar la cantidad de leds. Ademas de poder determinar individualmente
  el color. Por lo que por ejemplo, en la tira led que funcionaria como direccional izquierda podria mostrar un arcoiris.
  Tambien cabe mencionar que los ws2812b individuales son algo complicatos de soldar, por lo que muy probablemente se dañen
  las combinaciones de colores (como nos paso a nosotros), por lo que lo recomendable es que compren la cinta ya hecha. El codigo
  seria el mismo y podrian ampliar la gama de colores utilizados al modificar la funcion strip.Color(Red,Green,Blue)

  Dependiendo del diseño se pueden ampliar la cantidad de strip, pero el como se dejara a disposicion de la intuicion del lector

  Tal como en el comentario final del codigo del Control remoto, en loop existiria el if de freno y el else seria el resto de codigo
  Por lo cual lo extra aqui ya esta dicho.

  if(Serial.available())  // Si requiririamos enviar algun dato como retroalimentacion (como posible desconexiones o errores), usariamos este codigo
  {
     miBT.write(datos);
  }
  
*/