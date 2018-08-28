#include "HCuOLED.h"
#include "SPI.h"

/* Digital pin number for the displays chip select pin */
#define CS_DI 10
/* Digital pin number for the displays data/command pin */
#define DC_DI 9
/* Digital pin number for the displays reset pin */
#define RST_DI 8
HCuOLED HCuOLED(SH1106, CS_DI, DC_DI, RST_DI);
const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;  
const int MAX_ADC_VALUE = 1023;
void setup()
{
  HCuOLED.Reset();
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Cursor(4,10);
  HCuOLED.Print("capacitance");
 
   HCuOLED.Rect(103,5,125,27, OUTLINE);
  

 
  while(1)
  {
    /* Clear the area where the temperature will be displayed */
    HCuOLED.Erase(0,30,127,63);
    HCuOLED.Erase(104,6,124,26);
    /* Set the text cursor to that location */
   
    
    /* Read the current temperature from the LM35 sensor */

    /* Write the temperature to the display buffer */
   
  
  
    pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);

    if (val < 1000)
    {
      pinMode(IN_PIN, OUTPUT);

      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

      Serial.print(F("Capacitance Value = "));
      Serial.print(capacitance, 3);
      Serial.print(F(" pF ("));
      Serial.print(val);
      Serial.println(F(") "));
      HCuOLED.SetFont(LCDLarge_24pt);
       HCuOLED.Cursor(10,30);
      HCuOLED.Print(capacitance, 4, 2);  
      HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Cursor(105,10);
  HCuOLED.Print("pF");
 
      
     
       
    /* Output the display buffer to the screen */
    HCuOLED.Refresh();
    }
    else
    {
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;

      do
      {
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } while ((digVal < 1) && (t < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);

      float capacitance = -(float)t / R_PULLUP
                              / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

      Serial.print(F("Capacitance Value = "));
      if (capacitance > 1000.0)
      {
        Serial.print(capacitance / 1000.0, 2);
        Serial.print(F(" uF"));
         HCuOLED.SetFont(LCDLarge_24pt);
       HCuOLED.Cursor(10,30);
      HCuOLED.Print((capacitance/1000.0), 4, 2);  
      HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Cursor(105,10);
  HCuOLED.Print("uF");
  HCuOLED.Refresh();
      }
      else
      {
        Serial.print(capacitance, 2);
        Serial.print(F(" nF"));
         HCuOLED.SetFont(LCDLarge_24pt);
       HCuOLED.Cursor(10,30);
      HCuOLED.Print(capacitance, 4, 2);  
      HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Cursor(105,10);
  HCuOLED.Print("nF");
  HCuOLED.Refresh();
      }

      Serial.print(F(" ("));
      Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
      Serial.print(F(", t= "));
      Serial.print(t);
      Serial.print(F(" us, ADC= "));
      Serial.print(val);
      Serial.println(F(")"));
    }
    while (millis() % 1000 != 0);    
  }
}
