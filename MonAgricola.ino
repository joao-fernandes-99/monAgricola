#include <dht.h>
#include <Dns.h>
#include <Ethernet.h>

dht DHT;//create a variable type of dht
const int DHT11_PIN= 7;// PORTA DE COMUNICAÇÃO DO SENSOR DE TEMRATURA HAMBIENTE

#define pinSensorA A0 // PORTA DE COMUNICAÇÃO DO SENSOR DE HUMIDADE DE SOLO
#define pinSensorUV A1 //PORTA DE COMUNICAÇÃO DO SENSOR DE RAIOS UV
#define DEBUG

int leituraUV=0; // VARIÁVEL PARA ARMAZENAR A LEITURA DA PORTA ANALÓGICA
byte indiceUV=0; // VARIÁVEL PARA ARMAZENAR A CONVERSÃO PARA INDICE UV

float temp_ambiente = 0;
float humi_solo = 0 ;
char data = "";
int raio_uv = 0;
char name_area = "area1";

static byte myip[] = { 192,168,1,30 };
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
EthernetClient cliente;

IPAddress server(192,168,1,105);


void setup() {
  Serial.begin(57600);
  Ethernet.begin(mymac, myip);
}


void loop() {
  int err =0;
  D: int chk = DHT.read11(DHT11_PIN);//read the value returned from sensor
  switch (chk)
  {
  case DHTLIB_OK:  
    //Serial.println("OK!");
    //Serial.print("Checksum error,\t");
    break;
  case DHTLIB_ERROR_TIMEOUT:
      goto D;
    //Serial.print("Time out error,\t");
    break;
  default:
     // goto D;
    //Serial.print("Unknown error,\t");
    break;
  }
  // DISPLAY DATA

  leituraUV = analogRead(pinSensorUV); // REALIZA A LEITURA DA PORTA ANALÓGICA
  indiceUV = map(leituraUV, 0,203,0,10) ; // CONVERTE A FAIXA DE SINAL DO SENSOR DE 0V A 1V PARA O INDICE UV DE 0 A 10.


  temp_ambiente = DHT.temperature;
  humi_solo = analogRead(pinSensorA);
  raio_uv = indiceUV;
 
 
 
  Serial.print("Temperatura: ");
  Serial.print(temp_ambiente,1); //print the temperature on lcd
  Serial.println("---------------");
 
  Serial.print("Umidade: ");
  Serial.print(humi_solo);
  Serial.println("---------------");

   #ifdef DEBUG
    Serial.print("Indice UV: ");
    Serial.print(raio_uv);
    Serial.print("---------------");
  #endif

  Ethernet.begin(mymac, myip);
  Serial.println("Conectando....");
  String string = "PUT /area/area1?temp_ambiente="+(String)temp_ambiente+"&humi_solo="+(String)humi_solo+"&raio_uv="+(String)raio_uv+"&name=area1 HTTP/1.1";

  if(cliente.connect(server,5000)){
    Serial.println("Enviando dados..");
    cliente.println(string);
    cliente.println(F("Host: 192.168.0.105:5000"));
    cliente.println(F("Content-Type: application/x-www-form-urlencoded"));
    cliente.println(F("Content-Length: 0"));
    cliente.println(F("Connection: close"));
    cliente.println();
   
   
    Serial.println("Passou");
    cliente.stop();
    }else{
      Serial.println("Nao conectou");
      Serial.println(cliente.connect(server,5000));
      }

   delay(300000);  

 
