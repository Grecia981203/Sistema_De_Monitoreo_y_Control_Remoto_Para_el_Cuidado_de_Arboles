
//Librerias de comunicación Modbus
#include <ArduinoRS485.h> 
#include <ArduinoModbus.h>

//Librerias de reloj
#include <TimeLib.h>

//Librerias de sensor de Temperatura
#include <OneWire.h>
#include <DallasTemperature.h>

//Clases de OneWire y DallasTemperature
//OneWire oneWireObjeto(pinDatosT);
//DallasTemperature sensorDS18B20(&oneWireObjeto);

//Definicion de caracteristicas de esclavo
const int Id = 13;
const int Baud = 9600;

//Definicion de registros del SSF a SISMyC
const int SSF_NoConexion = 0x01;
const int SSF_EstadoAct = 0x02;
const int SSF_EstadoAnt = 0x03;
const int SSF_Modo = 0x04;
const int SSF_Confirmacion = 0x05;
const int SSF_Fechadd = 0x06;
const int SSF_Fechamm = 0x07;
const int SSF_Fechaaa = 0x08;
const int SSF_Horahh = 0x09;
const int SSF_Horamm = 0x0A;
const int SSF_Temperatura = 0x0B; //
const int SSF_Humedad = 0x0C;
const int SSF_NutrientesN = 0x0D; //
const int SSF_NutrientesP = 0x0E; //
const int SSF_NutrientesK = 0x0F; //
const int SSF_pH = 0x10;  //
//Copia de registros
int copy_SSF_NoConexion;
int copy_SSF_EstadoAct;
int copy_SSF_EstadoAnt;
int copy_SSF_Modo;
int copy_SSF_Confirmacion;
int copy_SSF_Fechadd;
int copy_SSF_Fechamm;
int copy_SSF_Fechaaa;
int copy_SSF_Horahh;
int copy_SSF_Horamm;
int copy_SSF_Temperatura; //
int copy_SSF_Humedad;
int copy_SSF_NutrientesN; //
int copy_SSF_NutrientesP; //
int copy_SSF_NutrientesK; //
int copy_SSF_pH;  //



//Definicion de registros del SISMyc a SSF
const int SISMyC_NoConexion = 0x11;
const int SISMyC_EstadoAct = 0x12;
const int SISMyC_EstadoAnt = 0x13;
const int SISMyC_Modo = 0x14;
const int SISMyC_Identificacion = 0x15;
const int SISMyC_Regar = 0x16;
const int SISMyC_Medir = 0x17;
const int SISMyC_Fertilizar = 0x18;
const int SISMyC_CantCiclosRiego = 0x19;
const int SISMyC_UnidadCiclosRiego = 0x1A;
const int SISMyC_CantCiclosMedicion = 0x1B;
const int SISMyC_UnidadCiclosMedicion = 0x1C;
const int SISMyC_CantAgua = 0x1D;
const int SISMyC_CantFertilizante = 0x1E;
const int SISMyC_RangoMinimoTemperatura = 0x1F;
const int SISMyC_RangoMaximoTemperatura = 0x20;
const int SISMyC_RangoMinimoHumedad = 0x21;
const int SISMyC_RangoMaximoHumedad = 0x22;
//Copias de Registros
int copy_SISMyC_NoConexion;
int copy_SISMyC_EstadoAct;
int copy_SISMyC_EstadoAnt;
int copy_SISMyC_Modo;
int copy_SISMyC_Identificacion;
int copy_SISMyC_Regar;
int copy_SISMyC_Medir;
int copy_SISMyC_Fertilizar;
int copy_SISMyC_CantCiclosRiego;
int copy_SISMyC_UnidadCiclosRiego;
int copy_SISMyC_CantCiclosMedicion;
int copy_SISMyC_UnidadCiclosMedicion;
int copy_SISMyC_CantAgua;
int copy_SISMyC_CantFertilizante;
int copy_SISMyC_RangoMinimoTemperatura;
int copy_SISMyC_RangoMaximoTemperatura;
int copy_SISMyC_RangoMinimoHumedad;
int copy_SISMyC_RangoMaximoHumedad;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(Baud);
  //setTime(20,0,0,19,3,2021);   //hr,mm,s,d,m,y
  if (!ModbusRTUServer.begin(Id, Baud)) { //Fallo de comunicacion
    while (1);
  }
  ModbusRTUServer.configureHoldingRegisters(SSF_NoConexion, 34);
//  sensorDS18B20.begin();   
}

void loop() {
  // put your main code here, to run repeatedly:
  ModbusRTUServer.poll();
  delay(30);
  Configuraciones();
  delay(30);
//  Serial.print(copy_SISMyC_NoConexion);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_EstadoAct);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_EstadoAnt);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_Modo);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_Identificacion);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_Regar);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_Medir);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_Fertilizar);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_CantCiclosRiego);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_UnidadCiclosRiego);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_CantCiclosMedicion);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_UnidadCiclosMedicion);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_CantAgua);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_CantFertilizante);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_RangoMinimoTemperatura);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_RangoMaximoTemperatura);
//  Serial.print("\t");
//  Serial.print(copy_SISMyC_RangoMinimoHumedad);
//  Serial.print("\t");
//  Serial.println(copy_SISMyC_RangoMaximoHumedad);
  
  //delay(60);
  if(copy_SISMyC_NoConexion != 0){
    copy_SSF_NoConexion = Id; 
    copy_SSF_EstadoAct = 1;
    copy_SSF_EstadoAnt = 0;
    copy_SSF_Modo = 1; //0 - Semi; 1 - Automatico
    copy_SSF_Confirmacion = 0;
    copy_SSF_Fechadd = 11;
    copy_SSF_Fechamm = 12;
    copy_SSF_Fechaaa = 2021;
    copy_SSF_Horahh = 18;
    copy_SSF_Horamm = 28;
    copy_SSF_Temperatura = 30; //
    copy_SSF_Humedad = 50;
    copy_SSF_NutrientesN = 0; //
    copy_SSF_NutrientesP = 0;//
    copy_SSF_NutrientesK = 0; //
    copy_SSF_pH = 0;  //
  
    Envio_Datos_Sensores();
    Limpieza_Datos();  
  }
}

int Configuraciones() {  
    delay(110);
    copy_SISMyC_NoConexion = ModbusRTUServer.holdingRegisterRead(SISMyC_NoConexion);
    delay(110);
    copy_SISMyC_EstadoAct = ModbusRTUServer.holdingRegisterRead(SISMyC_EstadoAct);
    delay(110);
    copy_SISMyC_EstadoAnt = ModbusRTUServer.holdingRegisterRead(SISMyC_EstadoAnt);
    delay(110);
    copy_SISMyC_Modo = ModbusRTUServer.holdingRegisterRead(SISMyC_Modo);
    delay(110);
    copy_SISMyC_Identificacion = ModbusRTUServer.holdingRegisterRead(SISMyC_Identificacion);
    delay(110);
    copy_SISMyC_Regar = ModbusRTUServer.holdingRegisterRead(SISMyC_Regar);
    delay(110);
    copy_SISMyC_Medir = ModbusRTUServer.holdingRegisterRead(SISMyC_Medir);
    delay(110);
    copy_SISMyC_Fertilizar = ModbusRTUServer.holdingRegisterRead(SISMyC_Fertilizar );
    delay(110);
    copy_SISMyC_CantCiclosRiego = ModbusRTUServer.holdingRegisterRead(SISMyC_CantCiclosRiego);
    delay(110);
    copy_SISMyC_UnidadCiclosRiego = ModbusRTUServer.holdingRegisterRead(SISMyC_UnidadCiclosMedicion);
    delay(110);
    copy_SISMyC_CantCiclosMedicion = ModbusRTUServer.holdingRegisterRead(SISMyC_CantCiclosMedicion);
    delay(110);
    copy_SISMyC_UnidadCiclosMedicion = ModbusRTUServer.holdingRegisterRead(SISMyC_UnidadCiclosMedicion);
    delay(110);
    copy_SISMyC_CantAgua = ModbusRTUServer.holdingRegisterRead(SISMyC_CantAgua);
    delay(110);
    copy_SISMyC_CantFertilizante = ModbusRTUServer.holdingRegisterRead(SISMyC_CantFertilizante);
    delay(110);
    copy_SISMyC_RangoMinimoTemperatura = ModbusRTUServer.holdingRegisterRead(SISMyC_RangoMinimoTemperatura);
    delay(110);
    copy_SISMyC_RangoMaximoTemperatura = ModbusRTUServer.holdingRegisterRead(SISMyC_RangoMaximoTemperatura);
    delay(110);
    copy_SISMyC_RangoMinimoHumedad = ModbusRTUServer.holdingRegisterRead(SISMyC_RangoMinimoHumedad);
    delay(110);
    copy_SISMyC_RangoMaximoHumedad = ModbusRTUServer.holdingRegisterRead(SISMyC_RangoMaximoHumedad);
    delay(110);
}


int Envio_Datos_Sensores() {   
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_NoConexion,copy_SSF_NoConexion);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_EstadoAct,copy_SSF_EstadoAct);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_EstadoAnt,copy_SSF_EstadoAnt);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Modo,copy_SSF_Modo);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Confirmacion,copy_SSF_Confirmacion);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Fechadd,copy_SSF_Fechadd);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Fechamm,copy_SSF_Fechamm);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Fechaaa,copy_SSF_Fechaaa);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Horahh,copy_SSF_Horahh);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Horamm,copy_SSF_Horamm);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Temperatura,copy_SSF_Temperatura);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_Humedad,copy_SSF_Humedad);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_NutrientesN,copy_SSF_NutrientesN);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_NutrientesP,copy_SSF_NutrientesP);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_NutrientesK,copy_SSF_NutrientesK);
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_pH,copy_SSF_pH);
    delay(10);    
}


int Limpieza_Datos(){  
    delay(110);
    copy_SSF_NoConexion = ModbusRTUServer.holdingRegisterRead(SSF_NoConexion);
    delay(110);
    copy_SSF_EstadoAct = ModbusRTUServer.holdingRegisterRead(SSF_EstadoAct);
    delay(110);
    copy_SSF_EstadoAct = ModbusRTUServer.holdingRegisterRead(SSF_EstadoAnt);
    delay(110);
    copy_SSF_Modo = ModbusRTUServer.holdingRegisterRead(SSF_Modo);
    delay(110);
    copy_SSF_Confirmacion = ModbusRTUServer.holdingRegisterRead(SSF_Confirmacion);
    delay(110);
    copy_SSF_Fechadd = ModbusRTUServer.holdingRegisterRead(SSF_Fechadd);
    delay(110);
    copy_SSF_Fechamm = ModbusRTUServer.holdingRegisterRead(SSF_Fechamm);
    delay(110);
    copy_SSF_Fechaaa = ModbusRTUServer.holdingRegisterRead(SSF_Fechaaa);
    delay(110);
    copy_SSF_Horahh = ModbusRTUServer.holdingRegisterRead(SSF_Horahh);
    delay(110);
    copy_SSF_Horamm = ModbusRTUServer.holdingRegisterRead(SSF_Horamm);
    delay(110);
    copy_SSF_Temperatura = ModbusRTUServer.holdingRegisterRead(SSF_Temperatura);
    delay(110);
    copy_SSF_Humedad = ModbusRTUServer.holdingRegisterRead(SSF_Humedad);
    delay(110);
    copy_SSF_NutrientesN = ModbusRTUServer.holdingRegisterRead(SSF_NutrientesN);
    delay(110);
    copy_SSF_NutrientesP = ModbusRTUServer.holdingRegisterRead(SSF_NutrientesP);
    delay(110);
    copy_SSF_NutrientesK = ModbusRTUServer.holdingRegisterRead(SSF_NutrientesK);
    delay(110);
    copy_SSF_pH = ModbusRTUServer.holdingRegisterRead(SSF_pH);
    delay(110);
}
