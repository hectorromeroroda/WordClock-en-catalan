#include <DS1302.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 60 

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

// Inicializacion del modulo.
DS1302 rtc(2, 3, 4);
Time t;

// ASIGNACION DE PINES PULSADOR
const int pulsador_mas = 8;
const int pulsador_menos = 7;

//Color del led en RGB
const int red = 255;
const int green = 255;
const int blue = 255;
 
// VARIABLES DE ESTADO DE BOTONES
int valor_pulsador_mas;
int valor_pulsador_menos;
int hora_pm;

void setup() {
  // CONFIGURAR PINES COMO ENTRADAS
  pinMode(pulsador_mas, INPUT);
  pinMode(pulsador_menos, INPUT);

   // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  // Inicializacion del puerto serie.
  Serial.begin(9600);
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  
  // Obtencion de datos
  t = rtc.getTime();

  // Obtencion estado pulsador
  valor_pulsador_mas = digitalRead(pulsador_mas);
  valor_pulsador_menos = digitalRead(pulsador_menos);

  // SE OPRIMIO EL BOTON DE ENCENDIDO?
  if (valor_pulsador_mas == HIGH) {
    // Desproteger contra escritura
    rtc.halt(false);
    rtc.writeProtect(false);
    rtc.setTime(t.hour + 1, t.min, t.sec);  // Configurar hora en formato 24hs con min y seg: 17:00:00 HORAS.
    
  }else{
    if (valor_pulsador_menos == HIGH) {
      rtc.halt(false);
      rtc.writeProtect(false);
      rtc.setTime(t.hour - 1, t.min, t.sec);  // Configurar hora en formato 24hs con min y seg: 17:00:00 HORAS.
    }else{
      // Publicar en LCD
      // Se publicara el dia de la semana
      Serial.print("HOY:"); 
      if (t.dow == 1) Serial.print("lun");  // La variable t.dow (dia de la semana) tedra valor de 1 para dia lunes y 7 para domingo.
      if (t.dow == 2) Serial.print("mar");
      if (t.dow == 3) Serial.print("mie");
      if (t.dow == 4) Serial.print("jue");
      if (t.dow == 5) Serial.print("vie");
      if (t.dow == 6) Serial.print("sab");
      if (t.dow == 7) Serial.print("dom");
      
      Serial.println();  // Linea aparte
      
      // Se publicaran datos de fecha, en numeros.
      Serial.print("DIA:");
      Serial.print(t.date, DEC);  // Dia del mes
      
      Serial.println();
      
      // Mes
      Serial.print("MES:");
      Serial.print(t.mon);
      
      Serial.println();
      
      // Año
      Serial.print("DE:");
      Serial.print(t.year, DEC);
      
      Serial.println();
      
      Serial.print("HORA:");  // Hora en formato 0-23.
      Serial.print(t.hour, DEC);
      Serial.print(", MIN:");  // Minutos.
      Serial.print(t.min, DEC);
      Serial.print(", SEG:");  // Segundos.
      Serial.print(t.sec, DEC);
      
      Serial.println();
      Serial.println();
      Serial.println();


    if (t.hour > 12) {
      hora_pm = t.hour-12;  
    }

      switch (hora_pm) {
        case 1:
             switch (t.min) {
                case 0:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(6, pixels.Color(red, green, blue));
                     pixels.setPixelColor(7, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
               
          break;
        case 2:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 3:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 4:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 5:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 6:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
                  
          break;
        case 7:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                      Serial.print("funciona");
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 8:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 9:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 10:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 11:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
        case 12:
            switch (t.min) {
                case 0:
                 
                  break;
                case 5:
                  
                  break;
                case 10:
                  
                  break;
                case 15:
                  
                  break;
                case 20:
                  
                  break;
                case 25:
                  
                  break;
                case 30:
                  
                  break;
                case 35:
                  
                  break;
                case 40:
                  
                  break;
                case 45:
                  
                  break;
                case 50:
                  
                  break;
                case 55:
                  
                  break;
                }
          
          break;
 
       }

       
    }
  
  }


  
  
  
  
  delay(1000);  // Demora para no sobrecargar las comunicaciones con el modulo.
}
