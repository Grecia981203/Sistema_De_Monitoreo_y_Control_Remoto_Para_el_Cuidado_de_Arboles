#include <TimeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int Led_V = 11;
int Led_S = 12;
int AlimentacionS = 10;

long Med_D;
long Rie_D;
long Horas_Del_Dia = 24;
int hor_M;  //Cada cuantas horas se mide
int hor_R;  //Minutos que se le suman a las horas en que se mide
int min_M;  //Cada cuantas horas se riega
int min_R;  //Minutos que se le suman a las horas en que se riega
int i = 0;
int valorHumedad1;
int valorHumedad2;
int valorHumedad3;
int SensorT1;
int SensorT2;
int SensorT3;
long Prom_SensorT;
int SensorH1_Porc;
int SensorH2_Porc;
int SensorH3_Porc;
long Prom_SensorH;

//Sensores
int pinDatosT = 12;
OneWire oneWireObjeto(pinDatosT);
DallasTemperature sensorDS18B20(&oneWireObjeto);
time_t fecha,Fecha_C, Fecha_R, Fecha_M;

#define sensorH1 A1  // Pin analogico donde se conecta el sensor humedad 1 
#define sensorH2 A2  // Pin analogico donde se conecta el sensor humedad 2
#define sensorH3 A3  // Pin analogico donde se conecta el sensor humedad 3

//Control 
//Constantes-------------

int lectura_SF1 =2;     //Pin lectura de sensor de flujo 1 Agua INT0
int lectura_SF2 =3;     //Pin lectura de sensor de flujo 2 Fertilizante INT1

  //--AJUSTAR--  
int alim_SF1 =8;       //Pin alimentación de sensor de flujo 1
int alim_SF2 =7;       //Pin alimentación de sensor de flujo 2
  //Salida del control
int VS1 =5;       //Pin alimentación de válvula solenoide 1 PWM
int VS2 =6;        //Pin alimentación de válvula solnoide 2 PWM

//Variables--------------

float volumen_a=0, volumen_a_final=0, volumen_f=0, volumen_f_final=0;    
//Para el volumen de fertilizante (Por lo mientras recibida en serial)


volatile int NumPulsosA=0, NumPulsosF=0; //variable para la cantidad de pulsos recibidos

//Calibración<<<<<
float factor_conversion1 = 6.9, factor_conversion2 = 7.0;             //De acuerdo a calibración de sensor realizada previamente<<<<<<
long dt=0;                  //variación de tiempo por cada bucle
long t0=0;                  //tiempo en millis() del bucle anterior

char modo;
char prueba_val;

//Bandera de riego
int band_R = 0;


void Intervalos(){
  Serial.print("Mediciones al día: ");
  while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
  if(Serial.available()){
    Med_D = Serial.parseInt();          
    Serial.println(Med_D);
     }
  Serial.print("Riego al día:");
  while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
   if(Serial.available()){
    Rie_D = Serial.parseFloat();
    Serial.println(Rie_D);          
     }

  //Intervalos de medicion
  hor_M = Horas_Del_Dia/Med_D;
  long Sob_hor_M = Horas_Del_Dia%Med_D;
  if( Sob_hor_M != 0){
    min_M = (Sob_hor_M * 60)/Med_D; 
  }
  else{
    min_M = 0;
  }

  //Intervalos de riego
  hor_R = Horas_Del_Dia/Rie_D;
  long Sob_hor_R = Horas_Del_Dia%Rie_D;
  if( Sob_hor_R != 0){
    min_R = (Sob_hor_R * 60)/Rie_D; 
  }
  else{
    min_R = 0;
  }
}

int Rutina(int Fecha_Accion, int hor_Accion, int min_Accion,time_t fecha_accion, time_t fecha){
int accion;
int Rut_Dia;
int Horas = hour(fecha_accion) + hor_Accion;
long Minutos = minute(fecha_accion) + min_Accion;

  int Add_Horas = Minutos/60;
  long min_sob = Minutos%60;
  //Serial.println(min_sob);
  //int Rut_min = min_sob*60;
  int Rut_min = min_sob;
int Rut_horas = Horas + Add_Horas;

if(Rut_horas>24){
  int Rut_horas_24 = Rut_horas/24;
  long Rut_horas_sob = Rut_horas-24;
  Rut_Dia = day(fecha_accion) + Rut_horas_24;
  Rut_horas = Rut_horas_sob;
  
}
else{
  Rut_Dia = day(fecha_accion);
}

// If de Medicion
//Serial.print("Fecha a llegar  ");
//Serial.println(Rut_Dia);
//Serial.print("  ");
//Serial.print(Rut_horas);
//Serial.print(":");
//Serial.println(Rut_min);
if(Rut_Dia == day(fecha) && Rut_horas == hour(fecha) && Rut_min == minute(fecha)){
  accion = 1;
  fecha_accion = now();}
else{
  accion = 0;
}
return accion;

}


//control----
//----Modo de riego----
void ModoRiego()
{
  //¿Ciclo de riego o prueba?
  Serial.println("S5 Control y distribución de suministros");
  Serial.println("Seleccione el modo de riego:");
  Serial.println("0- Prueba de riego \n1-Ciclo de riego");

}


//----- Función para prueba de apertura de válvulas-----
void Prueba()
{  
  //Mandar 1 a salidas PWM
              while(true){
            while(Serial.available()==0)
              {
                //No hagas nada si no recibes nada
              }
          prueba_val = Serial.read();
          Serial.println(prueba_val);
            switch(prueba_val)
            {
              case '0': Serial.println("Abriendo válvula 1"); 
                        digitalWrite(VS1,HIGH);
                        delay(3000);
                        digitalWrite(VS1,LOW);
                        //Abrir válvula y cerrar con un delay para prueba. LED para observar
                        break;
              case '1': Serial.println("Abriendo válvula 2"); 
                        digitalWrite(VS2,HIGH);
                        delay(1000);
                        digitalWrite(VS2,LOW);
                        //Abrir válvula y cerrar con un delay para prueba. LED para observar
                        break;
              case 'm': ModoRiego();
                        return;
            }
            }
}


float VolumenCaptura_A(){
  Serial.print("Ingresa el volumen de agua requerido[L]:");
       while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
     if(Serial.available() > 0 ){
    float volumen_a_final= Serial.parseFloat();          
    
    return volumen_a_final;
     }
  }


  float VolumenCaptura_F(){
  Serial.print("Ingresa el volumen de fertilizante requerido[L]:");
       while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
     if(Serial.available() > 0 ){
    float volumen_f_final= Serial.parseFloat();          
    
    return volumen_f_final;
     }
  }


//---Funciones que se ejecuta en interrupción---------------
void ContarPulsosAgua()
{ 
  NumPulsosA++;  //incrementamos la variable de pulsos
}

 void ContarPulsosFert ()
{ 
  NumPulsosF++;  //incrementamos la variable de pulsos
} 
//---------------------------------------------------------



//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuecia() 
{
  int frecuencia_a;
  NumPulsosA = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(500);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia_a= NumPulsosA; //Hz(pulsos por segundo)
  return frecuencia_a;
}


int ObtenerFrecueciaF() 
{
  int frecuencia_f;
  NumPulsosF = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(500);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  //Hz(pulsos por segundo)
  frecuencia_f= NumPulsosF;
  return frecuencia_f;
}
//----------------------------------------------------





void CicloRiego()
{
  Serial.println("Entrando a ciclo de riego");
  //Obtenemos datos de sensores
  //---Agua---
  float frecuencia_a=ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m_a=frecuencia_a/factor_conversion1; //calculamos el caudal en L/m
  float caudal_L_h_a=caudal_L_m_a*60; //calculamos el caudal en L/h
  //---Fertilizante---
  float frecuencia_f=ObtenerFrecueciaF();             //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m_f=frecuencia_f/factor_conversion2;  //calculamos el caudal en L/m
  float caudal_L_h_f=caudal_L_m_f*60;                 //calculamos el caudal en L/h

  dt=millis()-t0; //calculamos la variación de tiempo
  t0=millis();

  
  //Conteo de volumen
  volumen_a=volumen_a+(caudal_L_m_a/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)
  volumen_f=volumen_f+(caudal_L_m_f/60)*(dt/1000); 

  //Condicional para abrir/cerrar válvulas
  //---AGUA----
  if(volumen_a<volumen_a_final){

    Serial.println("V_agua Abierta");
    digitalWrite(VS1,HIGH);
  }
  else if(volumen_a>=volumen_a_final){
      
      digitalWrite(VS1,LOW);
      
    Serial.println("V_agua Cerrada");
      
    }
 
      
  //---FERTILIZANTE---
  if(volumen_f<volumen_f_final){
    digitalWrite(VS2,HIGH);
  
    Serial.println("V_fer Abierta");
  } 
  else if(volumen_f>=volumen_f_final){
      
      digitalWrite(VS2,LOW);
      
    Serial.println("V_fert Cerrada");
      }

  
  Serial.print    ("Numero de Pulsos Agua = "); 
  Serial.print    (NumPulsosA); 
  Serial.print    ("\t Numero de Pulsos Fertilizante = ");
  Serial.println  (NumPulsosF);
  Serial.println  ("Caudal agua: \t\t\t Caudal fertilizante:"); 
  Serial.print    (caudal_L_m_a,3);
  Serial.print    ("L/min\t\t\t"); 
  Serial.print    (caudal_L_m_f,3);
  Serial.println  ("L/min");
  Serial.print    (caudal_L_h_a,3); 
  Serial.print    ("L/h\t\t\t");   
  Serial.print    (caudal_L_h_f,3);
  Serial.println  ("L/h");
  Serial.print    (volumen_a,3); 
  Serial.print    (" L\t\t\t\t");
  Serial.print    (volumen_f,3);
  Serial.println  ("L");
  Serial.print    (volumen_a_final);
  Serial.print    (" L\t\t\t\t");
  Serial.print    (volumen_f_final);
  Serial.println  (" L");
//  Serial.print("Volumen_Fi \t");
//  Serial.print(volumen_f);
//  Serial.print("Volumen_Ai \t");
//  Serial.println(volumen_a);
//  Serial.print("Volumen_Ff \t");
//  Serial.print(volumen_f_final);
//  Serial.print("Volumen_Af \t");
//  Serial.println(volumen_a_final);

//  if(volumen_f>=volumen_f_final&&volumen_a>=volumen_a_final){
//      band_R=0;
//    }   

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Led_V, OUTPUT);
  pinMode(Led_S, OUTPUT);
  pinMode(AlimentacionS, OUTPUT);  
  pinMode(sensorH1, INPUT);  
  pinMode(sensorH2, INPUT);
  pinMode(sensorH3, INPUT);
  //---pinMode--- INPUT's
  pinMode(lectura_SF1, INPUT);
  pinMode(lectura_SF2, INPUT);
  //---pinMode---- OUTPUT's
  pinMode(alim_SF1, OUTPUT);  
  pinMode(alim_SF2, OUTPUT);
  pinMode(VS1, OUTPUT);
  pinMode(VS2, OUTPUT);

  //Iniciamos cerradas las válvulas
  digitalWrite(VS1,LOW);
  digitalWrite(VS2,LOW);
  
  // Iniciamos el bus 1-Wire
  sensorDS18B20.begin();


  //Interrupciones
  attachInterrupt(0,ContarPulsosAgua,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  attachInterrupt(1,ContarPulsosFert,RISING);//(Interrupción 1(Pin3),función,Flanco de subida)
  t0=millis();
    
  //setTime(hora,minutos,segundos,dia,mes,anyo);
  setTime(3, 48, 0, 9, 12, 2021);                 //---------------------------------------------MODIFICACIÓN DE FECHA-------------------------------------------------------
  fecha = now();
  Fecha_C = fecha;
  Fecha_R = fecha;
  Fecha_M = fecha;
  Serial.print(hour(fecha));
  Serial.print(":");
  Serial.println(minute(fecha));
  Intervalos();

  volumen_a_final = VolumenCaptura_A();
  Serial.println(volumen_a_final);
  volumen_f_final = VolumenCaptura_F();  
  Serial.println(volumen_f_final);
  }

void loop() {
  // put your main code here, to run repeatedly:
  int accion_M = Rutina(day(Fecha_M),hor_M,min_M,Fecha_M,fecha);
  int accion_R = Rutina(day(Fecha_R),hor_R,min_R,Fecha_R,fecha);
  
  fecha = now();
//  Serial.print("Fecha actual ");
//  Serial.print(day(fecha));
//  Serial.print("  ");
//  Serial.print(hour(fecha));
//  Serial.print(":");
//  Serial.print(minute(fecha));
//  Serial.print(":");
//  Serial.println(second(fecha));
//  
//  Serial.println(accion_M);
//  Serial.println(accion_R);
  
  
  if (accion_M == 1){
    medir();
    digitalWrite(Led_S,HIGH);
    delay(500);
    Fecha_M = now();}else{
      digitalWrite(AlimentacionS,LOW);
      digitalWrite(Led_S,LOW);
      
    }
   if (accion_R == 1){
    medir();
    digitalWrite(AlimentacionS,LOW);
    band_R=1;
    Serial.println("Antes del IF de Regarrr");
    Serial.println(Prom_SensorT);
    Serial.println(Prom_SensorH);
    int i = 0;
    if(Prom_SensorT > 6 && Prom_SensorT<30 && Prom_SensorH < 130){
      Serial.print("Entro al If");
      do{ 
      Serial.print("Regando");
      i = i + 1;
//      digitalWrite(alim_SF1,HIGH);
//      digitalWrite(alim_SF2,HIGH);
      //regar();
//      Serial.print("Volumen_Fi \t");
//      Serial.print(volumen_f);
//      Serial.print("Volumen_Ai \t");
//      Serial.println(volumen_a);
//      Serial.print("Volumen_Ff \t");
//      Serial.print(volumen_f_final);
//      Serial.print("Volumen_Af \t");
//      Serial.print(volumen_a_final);
      //if(volumen_f>=volumen_f_final&&volumen_a>=volumen_a_final){
      if(i == 10){
        //Serial.print("A Bandera de Riego Dentro de If \t");
        Serial.println(band_R);
        band_R=0;
      } 
        Serial.print("B Bandera de Riego Fuera de If \t");
        Serial.println(band_R);
      }while(band_R==1);
         
      Serial.print("Sale de while");
      //digitalWrite(alim_SF1,HIGH);
      //digitalWrite(alim_SF2,HIGH);
      //regar();
      
    }
    digitalWrite(Led_V,HIGH);
    delay(500);
    Fecha_R = now();}else{
      //Serial.println("No hay riego");
      
      digitalWrite(Led_V,LOW);
    }

}

void medir(){
  digitalWrite(AlimentacionS,HIGH);
  valorHumedad1 = map(analogRead(sensorH1),285, 1022, 100, 0);
  valorHumedad2 = map(analogRead(sensorH2),285, 1022, 100, 0);
  valorHumedad3 = map(analogRead(sensorH3),285, 1022, 100, 0);
  // Adquisicion de la lectura de los sensores de temperatura
  sensorDS18B20.requestTemperatures();
 
  // Leemos y mostramos los datos de los sensores DS18B20
  SensorT1 = sensorDS18B20.getTempCByIndex(0);
  SensorT2 = sensorDS18B20.getTempCByIndex(1);
  SensorT3 = sensorDS18B20.getTempCByIndex(2);
  Prom_SensorT = (SensorT1 + SensorT2 + SensorT3)/3;
  SensorH1_Porc = valorHumedad1;
  SensorH2_Porc = valorHumedad2;
  SensorH3_Porc = valorHumedad3;
  Prom_SensorH = (SensorH1_Porc + SensorH2_Porc + SensorH1_Porc)/3;
  
  Serial.print(sensorDS18B20.getTempCByIndex(0));
  Serial.print(";");
  Serial.print(sensorDS18B20.getTempCByIndex(1));
  Serial.print(";");
  Serial.print(sensorDS18B20.getTempCByIndex(2));
  Serial.print(";");
  Serial.print(valorHumedad1);
  Serial.print("; ");
  Serial.print(analogRead(sensorH1));
  Serial.print("; ");
  Serial.print(valorHumedad2);
  Serial.print(";");
  Serial.print(analogRead(sensorH2));
  Serial.print("; ");
  Serial.print(valorHumedad3);
  Serial.print(";");
  i=i+1;
  Serial.print(analogRead(sensorH3));
  Serial.print(";");
  Serial.println(i);
  Serial.println(" ");

  Serial.print("Promedio T, H: "); 
  Serial.print(Prom_SensorT); 
  Serial.print("\t");
  Serial.println(Prom_SensorH);
 
  // Espera un minuto
  //delay(60000);

}

void regar(){
  //---Alimentación del sensor de flujo
  Serial.println("Entro");
  digitalWrite(alim_SF1,HIGH);
  digitalWrite(alim_SF2,HIGH);
 CicloRiego(); 
// if(volumen_f>=volumen_f_final&&volumen_a>=volumen_a_final){
//    band_R=0;
//    } 
}
