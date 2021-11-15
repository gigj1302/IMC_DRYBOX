

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include <string.h>
#include <function.hpp>

//Declaration de l'ecran
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel

// Declaration du capteur d'humidite et temperature
#define DHTPIN  2
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
unsigned long tepTimer ;

//VARIABLE POUR LE SENSOR
    int val;                               // variable to store the value coming from the analog pin
    double data;                           // variable to store the temperature value coming from the conversion formula
    double data_t;
    double data_h;


#define PIN_Button 0

volatile bool gate_on_;
volatile bool play;
volatile bool boolreadSensor;

volatile bool press_action;
volatile bool end_action;

volatile int valueButton;
int mode;

void updateButton()
{
    int value = analogRead(PIN_Button);
    int valuedecode = decodeButton(value);
    if (valueButton != valuedecode && value != 1023 && press_action == false){
      valueButton = valuedecode;
      press_action = true;
      //xTaskNotify(melodie, 0xFF,eNoAction);
      //xTaskNotifyWait(0,0xFFFFFFFF,NULL, portMAX_DELAY);
      //xTaskNotify(melodie, 0xFF,eNoAction);
      //declancher l<interruption
      
    
    }
    else if (end_action == true)
    {
      Serial.println("end");
      valueButton = 0;
      press_action = false;
      end_action = false;
    }

    else{

    }
    
    // Serial.println("Button value");
    // Serial.println(valueButton);
    // Serial.println("test");
    // Serial.println(valuedecode);

}


void taskReadButton(void *pvParameters)
{
    for (;;)
    {
        updateButton();
        vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    }
}



// void modeMenu(){
//     lcd.setCursor(0, 0);
//     lcd.print(" Choose a mode guigi ! ");
//     lcd.setCursor(0, 1);
//     lcd.print("1.Heat  2.Dry  ");

// }

// void mode_heat(){
//     lcd.setCursor(0, 0);
//     lcd.print(" Heating mode  ", "1.ON    2.Timer");
//     lcd.setCursor(0, 1);
//     lcd.print("1.Heat  2.Dry  ");

// }




void choseMode(){

    if (mode == 0)
    {
        //modeMenu();
          lcd.setCursor(0, 0);
          lcd.print(" Choose a mode ");
          lcd.setCursor(0, 1);
          lcd.print("1.Heatss  2.Dry  ");
        Serial.println("reussite");
        if (press_action == true && end_action == false)
        {
          if(valueButton == 1){
            mode = 10;
          }
          else if(valueButton == 2)
          {
            mode = 20;
          }
          end_action = true;
          //press_action = false;
        }


        //Serial.println("reussite");
    }
    else if (mode ==10){
      lcd.setCursor(0, 0);
      lcd.print(" Heating mode  ");
      lcd.setCursor(0, 1);
      lcd.print("1.ON    2.Timer");
      Serial.println("reussite _10");

        if (press_action == true && end_action == false)
        {
          if(valueButton == 1){
            mode = 20;
          }
          else if(valueButton == 2)
          {
            mode = 20;
          }
          
          //press_action = false;
          end_action = true;
        }
    }
    else if (mode ==20){
      lcd.setCursor(0, 0);
      lcd.print(" Heating mode  ");
      lcd.setCursor(0, 1);
      lcd.print("fermeture utilisateur");

    }

    else{
    lcd.setCursor(0, 0);
    lcd.print(" ERROR CRISS ");
    lcd.setCursor(0, 1);
    lcd.print(":)");
    }



}

void taskAffichageEcran(void *pvParameters)
{
    for (;;)
    {
        choseMode();
        vTaskDelay( 50 / portTICK_PERIOD_MS );   


    }

}

void readSensor(){
    //Lecture des donnees du capteur
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    // Compute heat index in Fahrenheit (the default)
    //float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    //float hic = dht.computeHeatIndex(t, h, false);

// Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) 
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    

//affichage des donnees sur l'echan a une frequence X
    data_t = t;
    data_h = h; 
    
}

void taskReadSensor(void *pvParameters)
{
    if (boolreadSensor == true)
    {
        readSensor();
        vTaskDelay( 1000 / portTICK_PERIOD_MS );

    }
    //Serial.println(data_t);
    //Serial.println(data_h);

}


void setup()
{
    pinMode(PIN_Button, INPUT);
    boolreadSensor = true;
    //boolreadSensor = false;
    
    lcd.begin(16, 2);                       // start the library
    Serial.begin(9600);
    dht.begin();
    press_action = false;
    end_action = false;
    mode = 0;

    //attachInterrupt(digitalPinToInterrupt(PIN_SW1), startMelodie, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(PIN_SW2), startInterruptionBouton2, FALLING);

    xTaskCreate(taskReadButton, (const portCHAR *) "UpdateButton", 128  , NULL , 2 , NULL);
    xTaskCreate(taskAffichageEcran, (const portCHAR *) "AffichageEcran", 128  , NULL , 3 , NULL);
    //xTaskCreate(taskReadSensor, (const portCHAR *) "ReadSensor", 128  , NULL , 1 , NULL);
}

void loop()
{    

}

