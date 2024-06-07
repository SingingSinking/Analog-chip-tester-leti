#include "Wire.h"
#include "DFRobot_MCP4725.h"
#include <stdlib.h>

#define  REF_VOLTAGE    5000

DFRobot_MCP4725 DAC; 

char dtaUart[] = "5000"; // Количество разрядов MCP
uint8_t dtaLen = 0; 
const int buttonPin = 6;
float OUTPUT_VOLTAGE = 0; // Напряжение которое получаем с микросхем.
uint16_t TEST_VOLTAGE;  // Напряжение которое задаём для микросхемы. (+0.1V; +0.5V; +2.5V) unit : mV 
bool Diod = false;


void setup(void) {
  DAC.init(MCP4725A0_IIC_Address1, REF_VOLTAGE);
  pinMode(4, OUTPUT); //Green LEd
  pinMode(5, OUTPUT); //Red LED
  pinMode(3, OUTPUT); //Mode Led 1
  pinMode(2, OUTPUT); //Mode Led 2
  pinMode(7, OUTPUT); //Mode Led 3
  pinMode(buttonPin, INPUT_PULLUP); //Кнопка
  Serial.begin(9600);
}

inline void clearBuf(char *str)
{
   while(*str)
     *str++ = 0;
}

int mode = 1;
int flag = 0;

void loop(void) {
   if(digitalRead(6)==HIGH&&flag==0)
     { 
        // Функция кнопки для переключения режима одним нажатием
        // плюс защита от "дребезга" 
       mode++;   
       flag=1; 
        
        if(mode>3)
        { 
          mode=1;
        } 
     } 
      if(digitalRead(6)==LOW&&flag==1)
     { 
          
        flag=0;
     } 
    if(mode==1)//первый режим 
    { 
      digitalWrite(3,HIGH);
      digitalWrite(2,LOW);
      digitalWrite(7,LOW);
      TEST_VOLTAGE = 100; dtaUart[dtaLen++] = TEST_VOLTAGE;  // +0.1V
    } 
    if(mode==2)//второй режим 
    { 
      digitalWrite(3,HIGH);
      digitalWrite(2,HIGH);
      digitalWrite(7,LOW);
      TEST_VOLTAGE = 500; dtaUart[dtaLen++] = TEST_VOLTAGE; // +0.5V
    } 
    if(mode==3)//третий режим 
    { 
      digitalWrite(3,HIGH);
      digitalWrite(2,HIGH);
      digitalWrite(7,HIGH);
      TEST_VOLTAGE = 2500; dtaUart[dtaLen++] = TEST_VOLTAGE; // +2.5V
    } 

      //AD8221 - A0
      //OPA2134 - A1
      //UA4721 - A2
      //AD633d- A3
      //AD620 - A6
      //AD633s - A7

      float val0 = analogRead(A0); 
      float val1 = analogRead(A1);
      float val2 = analogRead(A2);
      float val3 = analogRead(A3);
      float val6 = analogRead(A6);
      float val7 = analogRead(A7);

    if(dtaLen > 0){
      
      Serial.println("¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯");
      Serial.print("Напряжение для Теста: ");
      Serial.print(TEST_VOLTAGE); 
      Serial.println(" mV");
      dtaLen = 0 ;
      DAC.outputVoltage(TEST_VOLTAGE);
      
      Serial.print("AD8221: ");
      Serial.println(val0 * 0.0048828125);
      Serial.print("OPA2134: ");
      Serial.println(val1 * 0.0048828125);
      Serial.print("UA4721: ");
      Serial.println(val2 * 0.0048828125);
      Serial.print("AD633d: ");
      Serial.println(val3 * 0.0048828125);
      Serial.print("AD620: ");
      Serial.println(val6 * 0.0048828125);
      Serial.print("AD633s: ");
      Serial.println(val7 * 0.0048828125);
      Serial.println("");

      float FLOAT_TEST_VOLTAGE = TEST_VOLTAGE * 0.001; //Перевод Напряжения в нужную величину 
      float a = 0; 

      if (val0 > val1 && val0 > val1 && val0 > val3 && val0 > val6 && val0 > val7) { //AD8221 для пина A0

          Serial.println("/AD8221/");
      
        // Среднее значение 20 значений в промежутке 1 секунды
        for(int i = 0; i < 20; i++) {
          OUTPUT_VOLTAGE = analogRead(A0);
          OUTPUT_VOLTAGE = OUTPUT_VOLTAGE * 0.0048828125;
          a += OUTPUT_VOLTAGE;
          delay(50);
        } 
        float SR_V = a / 20;

        Serial.print("Напряжение после Теста: ");
        Serial.println(OUTPUT_VOLTAGE,4); 
        Serial.print("Ср напряжение после Теста: ");
        Serial.println(SR_V,4);

        //Сравнение значений напряжения до и после.
        if (abs(FLOAT_TEST_VOLTAGE - SR_V) < 0.1) {
            Serial.println("True");
            Diod = true;
        } else{
            Serial.println("False");
            Diod = false;

          }

      } else if (val1 > val0 && val1 > val2 && val1 > val3 && val1 > val6 && val1 > val7) { // OPA2134 для пина A1

        Serial.println("/OPA2134/");
        
        for(int i = 0; i < 20; i++) {
          OUTPUT_VOLTAGE = analogRead(A1);
          OUTPUT_VOLTAGE = OUTPUT_VOLTAGE * 0.0048828125;
          a += OUTPUT_VOLTAGE;
          delay(50);
        } 
        float SR_V = a / 20;

        Serial.print("Напряжение после Теста : ");
        Serial.println(OUTPUT_VOLTAGE,4); 
        Serial.print("Ср напряжение после Теста: ");
        Serial.println(SR_V,4);
        Serial.print("Test_Voltage: ");
        Serial.println(FLOAT_TEST_VOLTAGE,4);

        if (abs(FLOAT_TEST_VOLTAGE - SR_V) < 0.1) {
            Serial.println("True");
            Diod = true;
        } else{
            Serial.println("False");
            Diod = false;

          } 

      } else if (val2 > val0 && val2 > val1 && val2 > val3 && val2 > val6 && val2 > val7) { // UA4721 для пина A2

        Serial.println("/UA4721/");
        
        for(int i = 0; i < 20; i++) {
          OUTPUT_VOLTAGE = analogRead(A2);
          OUTPUT_VOLTAGE = OUTPUT_VOLTAGE * 0.0048828125;
          a += OUTPUT_VOLTAGE;
          delay(50);
        } 
        float SR_V = a / 20;

        Serial.print("Напряжение после Теста : ");
        Serial.println(OUTPUT_VOLTAGE,4); 
        Serial.print("Ср напряжение после Теста: ");
        Serial.println(SR_V,4);
        

        if (abs(FLOAT_TEST_VOLTAGE - SR_V) < 0.1) {
            Serial.println("True");
            Diod = true;
        } else{
            Serial.println("False");
            Diod = false;

          } 

      } else if (val3 > val0 && val3 > val2 && val3 > val2 && val3 > val6 && val3 > val7) { // AD633D для пина A3

        Serial.println("/AD633d/");

        for(int i = 0; i < 20; i++) {
          OUTPUT_VOLTAGE = analogRead(A3);
          OUTPUT_VOLTAGE = OUTPUT_VOLTAGE * 0.0048828125;
          a += OUTPUT_VOLTAGE;
          delay(50);
        } 
        
        float SR_V = a / 20;
        float Formula_AD633 = FLOAT_TEST_VOLTAGE * FLOAT_TEST_VOLTAGE / 10 + FLOAT_TEST_VOLTAGE;

        Serial.print("Напряжение по форимуле AD633: ");
        Serial.println(Formula_AD633,4);
        Serial.print("Напряжение после Теста : ");
        Serial.println(OUTPUT_VOLTAGE,4); 
        Serial.print("Ср напряжение после Теста: ");
        Serial.println(SR_V,4);

        if (abs(Formula_AD633 - SR_V) < 0.2) {
          Serial.println("True");
          Diod = true;
        } else {
          Serial.println("False");
          Diod = false;
        }

      } else if(val6 > val1 && val6 > val2 && val6 > val3 && val6 > val0 && val6 > val7) {  // AD620 для пина A6

        Serial.println("/AD620/");

        for(int i = 0; i < 20; i++) {
          OUTPUT_VOLTAGE = analogRead(A6);
          OUTPUT_VOLTAGE = OUTPUT_VOLTAGE *  0.0048828125;
          a += OUTPUT_VOLTAGE;
          delay(50);
        } 
        float SR_V = a / 20;
        float Formula_AD620 = (((24.7 * 2)/10 ) + 1) * FLOAT_TEST_VOLTAGE;

        Serial.print("Напряжение по форимуле AD620: ");
        Serial.println(Formula_AD620,4);
        Serial.print("Напряжение после Теста : ");
        Serial.println(OUTPUT_VOLTAGE,4); 
        Serial.print("Ср напряжение после Теста: ");
        Serial.println(SR_V,4);

        if (abs(Formula_AD620 - SR_V) < 0.2) {
          Serial.println("True");
          Diod = true;
        } else {
          Serial.println("False");
          Diod = false;
        }

      } else {  // AD633s для пина A7

        Serial.println("/AD633s/");

        for(int i = 0; i < 20; i++) {
          OUTPUT_VOLTAGE = analogRead(A7);
          OUTPUT_VOLTAGE = OUTPUT_VOLTAGE * 0.0048828125;
          a += OUTPUT_VOLTAGE;
          delay(50);
        } 
        
        float SR_V = a / 20;
        float Formula_AD633s = FLOAT_TEST_VOLTAGE * FLOAT_TEST_VOLTAGE / 10 + FLOAT_TEST_VOLTAGE;

        Serial.print("Напряжение по форимуле AD633: ");
        Serial.println(Formula_AD633s,4);
        Serial.print("Напряжение после Теста : ");
        Serial.println(OUTPUT_VOLTAGE,4); 
        Serial.print("Ср напряжение после Теста: ");
        Serial.println(SR_V,4);

        if (abs(Formula_AD633s - SR_V) < 0.2) {
          Serial.println("True");
          Diod = true;
        } else {
          Serial.println("False");
          Diod = false;
        }
      }   

    }
      if (Diod == true) {
        digitalWrite(4, HIGH);
        digitalWrite(5, LOW);
      } else {
        digitalWrite(5, HIGH);
        digitalWrite(4, LOW);
    } 
    Serial.println("______________________________");
    Serial.println("//////////////////////////////");
    delay(500);
}
