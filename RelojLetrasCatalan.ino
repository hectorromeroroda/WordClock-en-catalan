


#include <DS1302.h>

// Inicializacion del modulo.
DS1302 rtc(2, 3, 4);
Time t;

// ASIGNACION DE PINES PULSADOR
const int pulsador_mas = 8;
const int pulsador_menos = 7;
 
// VARIABLES DE ESTADO DE BOTONES
int valor_pulsador_mas;
int valor_pulsador_menos;

void setup() {
// CONFIGURAR PINES COMO ENTRADAS
 pinMode(pulsador_mas, INPUT);
  pinMode(pulsador_menos, INPUT);
// Inicializacion del puerto serie.
Serial.begin(9600);
}

void loop() {
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
<<<<<<< Updated upstream
=======


    if (t.hour > 12) {
      hora_pm = t.hour-12;  
    }

      switch (hora_pm) {
        case 1:
<<<<<<< Updated upstream
         
=======
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
               
>>>>>>> Stashed changes
          break;
        case 2:
          
          break;
        case 3:
          
          break;
        case 4:
          
          break;
        case 5:
          
          break;
        case 6:
          
          break;
        case 7:
          
          break;
        case 8:
          
          break;
        case 9:
          
          break;
        case 10:
          
          break;
        case 11:
          
          break;
        case 12:
          
          break;
 
       }

       
>>>>>>> Stashed changes
    }
  
  }


  
  
  
  
  delay(1000);  // Demora para no sobrecargar las comunicaciones con el modulo.
}
