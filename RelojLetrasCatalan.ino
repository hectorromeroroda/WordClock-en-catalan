#include <DS1302.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 120 

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
const int pulsador_horas_mas = 8;
const int pulsador_horas_menos = 7;
const int pulsador_minutos_mas = 13;
const int pulsador_minutos_menos = 12;
const int pulsador_rgb_mas = 11;
const int pulsador_rgb_menos = 10;

//Color del led en RGB
const int red = 255;
const int green = 255;
const int blue = 255;
 
// VARIABLES DE ESTADO DE BOTONES
int valor_pulsador_horas_mas;
int valor_pulsador_horas_menos;
int valor_pulsador_minutos_mas;
int valor_pulsador_minutos_menos;
int valor_pulsador_rgb_mas;
int valor_pulsador_rgb_menos;

//VARIABLES PARA CALCULO HORAS
int hora_pm;
int minutos_sueltos;
int minutos_letras;

void setup() {
  // CONFIGURAR PINES COMO ENTRADAS
  pinMode(pulsador_horas_mas, INPUT);
  pinMode(pulsador_horas_menos, INPUT);
  pinMode(pulsador_minutos_mas, INPUT);
  pinMode(pulsador_minutos_menos, INPUT);
  pinMode(pulsador_rgb_mas, INPUT);
  pinMode(pulsador_rgb_menos, INPUT);

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
  minutos_sueltos = 0;
  pixels.clear(); // Set all pixel colors to 'off'
  
  // Obtencion de datos
  t = rtc.getTime();

  // Obtencion estado pulsador
  valor_pulsador_horas_mas = digitalRead(pulsador_horas_mas);
  valor_pulsador_horas_menos = digitalRead(pulsador_horas_menos);
  valor_pulsador_minutos_mas = digitalRead(pulsador_minutos_mas);
  valor_pulsador_minutos_menos = digitalRead(pulsador_minutos_menos);
  valor_pulsador_rgb_mas = digitalRead(pulsador_rgb_mas);
  valor_pulsador_rgb_menos = digitalRead(pulsador_rgb_menos);

  // SE OPRIMIO EL BOTON DE ENCENDIDO?
  if (valor_pulsador_horas_mas == HIGH) {
    // Desproteger contra escritura
    rtc.halt(false);
    rtc.writeProtect(false);
    rtc.setTime(t.hour + 1, t.min, t.sec);  // Configurar hora en formato 24hs con min y seg: 17:00:00 HORAS.
    
  }else{
    if (valor_pulsador_horas_menos == HIGH) {
      rtc.halt(false);
      rtc.writeProtect(false);
      rtc.setTime(t.hour - 1, t.min, t.sec);  // Configurar hora en formato 24hs con min y seg: 17:00:00 HORAS.
    }else{
        if (valor_pulsador_minutos_menos == HIGH) {
        rtc.halt(false);
        rtc.writeProtect(false);
        rtc.setTime(t.hour, t.min - 1, t.sec);  // Configurar hora en formato 24hs con min y seg: 17:00:00 HORAS.
        }else{
          if (valor_pulsador_minutos_mas == HIGH) {
            rtc.halt(false);
            rtc.writeProtect(false);
            rtc.setTime(t.hour, t.min + 1, t.sec);  // Configurar hora en formato 24hs con min y seg: 17:00:00 HORAS.
            }
        }
    }

    if (valor_pulsador_rgb_mas == HIGH) {
    
    
    }else{
      if (valor_pulsador_rgb_menos == HIGH){


        
      }
    }
    
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
    }else{
      hora_pm = t.hour;
    }

    minutos_sueltos = t.min % 5;  
    if (minutos_sueltos != 0) {
      minutos_letras = t.min - minutos_sueltos;
            
      Serial.print(" He entrado en minutos sueltos = ");
      Serial.print(minutos_letras);
      Serial.println();
      Serial.println();
      
      switch (minutos_sueltos) {
        case 1:
            //PRECARGAR LOS LEDS DE LOS MINUTOS SUELTOS (sin hacer el show) !!!!!!!!!!!!!!!!! ----------------------------------------------------------------------
            Serial.print(" Minuto suelto = ");
            Serial.print(minutos_sueltos);
            Serial.println();
            Serial.println();
          break;
        case 2:
            //PRECARGAR LOS LEDS DE LOS MINUTOS SUELTOS (sin hacer el show) !!!!!!!!!!!!!!!!! ----------------------------------------------------------------------
            Serial.print(" Minuto suelto = ");
            Serial.print(minutos_sueltos);
            Serial.println();
            Serial.println();
          break;
        case 3:
            //PRECARGAR LOS LEDS DE LOS MINUTOS SUELTOS (sin hacer el show) !!!!!!!!!!!!!!!!! ----------------------------------------------------------------------
            Serial.print(" Minuto suelto = ");
            Serial.print(minutos_sueltos);
            Serial.println();
            Serial.println();
          break;
        case 4:
            //PRECARGAR LOS LEDS DE LOS MINUTOS SUELTOS (sin hacer el show) !!!!!!!!!!!!!!!!! ----------------------------------------------------------------------
            Serial.print(" Minuto suelto = ");
            Serial.print(minutos_sueltos);
            Serial.println();
            Serial.println();
          break;
      }    
    }else{
      minutos_letras = t.min;
      Serial.print("He entrado en minutos en punto = ");
      Serial.print(minutos_letras);
      Serial.println();
      Serial.println();
    }
    

    

      switch (hora_pm) {
        case 1:
             switch (minutos_letras) {
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
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(6, pixels.Color(red, green, blue));
                     pixels.setPixelColor(7, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));       
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));         
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue)); 
                     pixels.setPixelColor(42, pixels.Color(red, green, blue)); 
                     pixels.setPixelColor(43, pixels.Color(red, green, blue)); 
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));         
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                }     
          break;
        case 2:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(63, pixels.Color(red, green, blue));
                     pixels.setPixelColor(64, pixels.Color(red, green, blue));
                     pixels.setPixelColor(65, pixels.Color(red, green, blue));
                     pixels.setPixelColor(66, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hardware.
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 3:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(60, pixels.Color(red, green, blue));
                     pixels.setPixelColor(61, pixels.Color(red, green, blue));
                     pixels.setPixelColor(62, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 4:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));        
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(67, pixels.Color(red, green, blue));
                     pixels.setPixelColor(68, pixels.Color(red, green, blue));
                     pixels.setPixelColor(69, pixels.Color(red, green, blue));
                     pixels.setPixelColor(70, pixels.Color(red, green, blue));
                     pixels.setPixelColor(71, pixels.Color(red, green, blue));
                     pixels.setPixelColor(72, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 5:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard  
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard  
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard  
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard  
                  break;
                }
          
          break;
        case 6:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));;
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(90, pixels.Color(red, green, blue));
                     pixels.setPixelColor(91, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
                  
          break;
        case 7:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue)); 
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard;
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(57, pixels.Color(red, green, blue));
                     pixels.setPixelColor(58, pixels.Color(red, green, blue));
                     pixels.setPixelColor(59, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 8:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.setPixelColor(89, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(104, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                     
                  break;
                case 50:
                    pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(85, pixels.Color(red, green, blue));
                     pixels.setPixelColor(86, pixels.Color(red, green, blue));
                     pixels.setPixelColor(87, pixels.Color(red, green, blue));
                     pixels.setPixelColor(88, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 9:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                      pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(82, pixels.Color(red, green, blue));
                     pixels.setPixelColor(83, pixels.Color(red, green, blue));
                     pixels.setPixelColor(84, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 10:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                    pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(94, pixels.Color(red, green, blue));
                     pixels.setPixelColor(95, pixels.Color(red, green, blue));
                     pixels.setPixelColor(96, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(52, pixels.Color(red, green, blue));
                     pixels.setPixelColor(53, pixels.Color(red, green, blue));
                     pixels.setPixelColor(54, pixels.Color(red, green, blue));
                     pixels.setPixelColor(55, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
        case 11:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(52, pixels.Color(red, green, blue));
                     pixels.setPixelColor(53, pixels.Color(red, green, blue));
                     pixels.setPixelColor(54, pixels.Color(red, green, blue));
                     pixels.setPixelColor(55, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(52, pixels.Color(red, green, blue));
                     pixels.setPixelColor(53, pixels.Color(red, green, blue));
                     pixels.setPixelColor(54, pixels.Color(red, green, blue));
                     pixels.setPixelColor(55, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(47, pixels.Color(red, green, blue));
                     pixels.setPixelColor(78, pixels.Color(red, green, blue));
                     pixels.setPixelColor(79, pixels.Color(red, green, blue));
                     pixels.setPixelColor(80, pixels.Color(red, green, blue));
                     pixels.setPixelColor(81, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
      case 12:
            switch (minutos_letras) {
                case 0:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 5:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(17, pixels.Color(red, green, blue));
                     pixels.setPixelColor(18, pixels.Color(red, green, blue));
                     pixels.setPixelColor(19, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(105, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 10:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 15:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 20:
                     pixels.setPixelColor(1, pixels.Color(red, green, blue));
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(9, pixels.Color(red, green, blue));
                     pixels.setPixelColor(10, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 25:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 30:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 35:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(20, pixels.Color(red, green, blue));
                     pixels.setPixelColor(21, pixels.Color(red, green, blue));
                     pixels.setPixelColor(22, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 40:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(40, pixels.Color(red, green, blue));
                     pixels.setPixelColor(41, pixels.Color(red, green, blue));
                     pixels.setPixelColor(42, pixels.Color(red, green, blue));
                     pixels.setPixelColor(43, pixels.Color(red, green, blue));
                     pixels.setPixelColor(44, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 45:
                      pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 50:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(12, pixels.Color(red, green, blue));
                     pixels.setPixelColor(13, pixels.Color(red, green, blue));
                     pixels.setPixelColor(14, pixels.Color(red, green, blue));
                     pixels.setPixelColor(15, pixels.Color(red, green, blue));
                     pixels.setPixelColor(27, pixels.Color(red, green, blue));
                     pixels.setPixelColor(28, pixels.Color(red, green, blue));
                     pixels.setPixelColor(29, pixels.Color(red, green, blue));
                     pixels.setPixelColor(30, pixels.Color(red, green, blue));
                     pixels.setPixelColor(31, pixels.Color(red, green, blue));
                     pixels.setPixelColor(32, pixels.Color(red, green, blue));
                     pixels.setPixelColor(34, pixels.Color(red, green, blue));
                     pixels.setPixelColor(35, pixels.Color(red, green, blue));
                     pixels.setPixelColor(36, pixels.Color(red, green, blue));
                     pixels.setPixelColor(37, pixels.Color(red, green, blue));
                     pixels.setPixelColor(39, pixels.Color(red, green, blue));
                     pixels.setPixelColor(45, pixels.Color(red, green, blue));
                     pixels.setPixelColor(46, pixels.Color(red, green, blue));
                     pixels.setPixelColor(73, pixels.Color(red, green, blue));
                     pixels.setPixelColor(74, pixels.Color(red, green, blue));
                     pixels.setPixelColor(75, pixels.Color(red, green, blue));
                     pixels.setPixelColor(76, pixels.Color(red, green, blue));
                     pixels.setPixelColor(77, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                case 55:
                     pixels.setPixelColor(2, pixels.Color(red, green, blue));
                     pixels.setPixelColor(3, pixels.Color(red, green, blue));
                     pixels.setPixelColor(4, pixels.Color(red, green, blue));
                     pixels.setPixelColor(6, pixels.Color(red, green, blue));
                     pixels.setPixelColor(7, pixels.Color(red, green, blue));
                     pixels.setPixelColor(49, pixels.Color(red, green, blue));
                     pixels.setPixelColor(50, pixels.Color(red, green, blue));
                     pixels.setPixelColor(51, pixels.Color(red, green, blue));
                     pixels.setPixelColor(100, pixels.Color(red, green, blue));
                     pixels.setPixelColor(101, pixels.Color(red, green, blue));
                     pixels.setPixelColor(102, pixels.Color(red, green, blue));
                     pixels.setPixelColor(103, pixels.Color(red, green, blue));
                     pixels.setPixelColor(106, pixels.Color(red, green, blue));
                     pixels.setPixelColor(107, pixels.Color(red, green, blue));
                     pixels.setPixelColor(108, pixels.Color(red, green, blue));
                     pixels.setPixelColor(109, pixels.Color(red, green, blue));
                     pixels.setPixelColor(110, pixels.Color(red, green, blue));
                     pixels.show();   // Send the updated pixel colors to the hard
                  break;
                }
          
          break;
 
       }

       
    
  
  }


  
  
  
  
  delay(1000);  // Demora para no sobrecargar las comunicaciones con el modulo.
}
