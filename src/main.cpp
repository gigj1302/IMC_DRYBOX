#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include <string.h>
#include <function.hpp>
#include <Servo.h>

//declaration du servo
Servo myservo;

//Declaration de l'ecran
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel

// Declaration du capteur d'humidite et temperature
#define DHTPIN  2
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Declaration des valeurs des differents menues
#define Heating_Timer 10
#define Start 0
#define Drying_Timer 50
#define Heating 11
#define Drying 51
#define Home 1
#define Heating_ON 12
#define Drying_ON 52
#define Left_Heating 13
#define Left_Drying 53
#define error 9999

//Autre variables
int ON = false;
int timeDuration = 20;
volatile int text; 
unsigned long tepTimer ;

//VARIABLE POUR LE SENSOR
    int val;                               // variable to store the value coming from the analog pin
    double data;                           // variable to store the temperature value coming from the conversion formula
    volatile double data_t;
    volatile double data_h;


#define PIN_Button 0

volatile bool gate_on_;
volatile bool play;
volatile bool boolreadSensor;

volatile bool press_action;
volatile bool end_action;

volatile int valueButton;
int mode;
volatile int temps = 20;

struct returnTimer {
    int mode;
    int timer;
};

typedef struct returnTimer Struct;

void clickButton(bool on){
  myservo.write(30);
  delay(500);
  myservo.write(0);
  ON = on;
  Serial.print("ON in = ");
  Serial.println(ON);
}

void on(){
  Serial.println("START");
  clickButton(true);
}

void off(){
  Serial.println("STOP");
  clickButton(false);
}


int Back(int mode){

  if (press_action == true && end_action == false)
  {
    
          if (valueButton == 1){//Back
            if (mode == Left_Heating){
              return (Heating);
            }
            else if (mode == Left_Drying){
              return (Drying);
            }
            else if (mode == Heating_Timer){
              return (Heating);
            }
            else if (mode == Drying_Timer){
              return (Drying);
            }
            else if (mode == Heating_ON){
              return (Heating);
            }
            else if (mode == Drying_ON){
              return (Drying);
            }
            else if (mode == Heating){
              return(Home);
            }
            else if (mode == Drying){
              return(Home);
            }
          }
          end_action = true;
  }
}

int Stop(int mode){
  if (press_action == true && end_action == false)
  {
    if (valueButton == 1){
      if (mode == Left_Heating){
        off();
        return(Heating_Timer);
      }
      else if (mode == Left_Drying){
        off();
        return(Drying_Timer);
      }
      else if (mode == Heating_ON){
        off();
        return(Heating);
      }
      else if (mode == Drying_ON){
        off();
        return(Drying);
      }
      else{
        Serial.print("RIEN***************************************************************");
        Serial.print(error);
        return(error);
       }
    }
  }
}


Struct timer(int mode){
  valueButton = 0;
  press_action = false;
  int i = 1;
  int valuemax = 400;
    while (true) {
      int value = analogRead(PIN_Button);
      int valuedecode = decodeButton(value);
      if (valueButton != 2 && press_action == false && i%valuemax == 0){
        valueButton = valuedecode;
        if (valueButton != 0 )
        {
          if(valueButton != 2)
          {
            press_action = true;
          }
        }
      }
      if(i == (valuemax +5 )){
        i = 1;
      }
      if (valueButton == 2)
      {
        valueButton = 0;
      }

      Struct timer;
      if (valueButton == 5){//Back
        if (mode == Heating){
          //setLines(" Heating mode  ", "1.ON    2.Timer");
          lcd.setCursor(0, 0);
          lcd.print(" Heating mode  ");
          lcd.setCursor(0, 1);
          lcd.print("1.ON    2.Timer");
          timer.mode = mode;
          timer.timer = 20;
          return(timer);
          break;
        }
              else if (mode == Drying){
                //setLines("  Drying mode  ", "1.ON    2.Timer");
                lcd.setCursor(0, 0);
                lcd.print("  Drying mode  ");
                lcd.setCursor(0, 1);
                lcd.print("1.ON    2.Timer");
                timer.mode = mode;
                timer.timer = 20;
                return(timer);
                break;
              }
            }
            else if (valueButton == 3){//Up
            if (temps < 90){
              temps = temps + 10;
              //setLines("Timer duration ", "    "+String(temps)+" min     ");
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Timer duration ");
                lcd.setCursor(6, 1);
                lcd.setCursor(4, 1);
                lcd.print(temps);
                lcd.setCursor(7, 1);
                lcd.print(" min ");
                valueButton = 0;
                press_action = false;
                Serial.println("OK!");
            }
            }
            else if (valueButton == 4){//Down
            if (temps >= 30){
              temps = temps - 10;
              //setLines("Timer duration ", "    "+String(temps)+" min     ");
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Timer duration ");
                lcd.setCursor(6, 1);
                lcd.setCursor(4, 1);
                lcd.print(temps);
                lcd.setCursor(7, 1);
                lcd.print(" min ");
                valueButton = 0;
                press_action = false;
                Serial.println("OK");
            }
            }
            else if (valueButton == 1){//Start
              //setLines("    Time left   ", "    "+String(temps)+" min     ");
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("    Time left   ");
                lcd.setCursor(4, 1);
                lcd.print(temps);
                lcd.setCursor(7, 1);
                lcd.print(" min ");
                
              if (mode == Heating){
                timer.mode = Left_Heating;
              }else{
                timer.mode = Left_Drying;
              }
              timer.timer = temps;
              return(timer);
              break;
            }
      i +=1;
    }
    end_action = true;
}

int LeftTime(int temps){
  int compteur = 0;
  valueButton = 0;
  press_action = false;
  int i = 1;
  int valuemax = 400;
  while (temps > 0) {
    Serial.println(temps);
      int value = analogRead(PIN_Button);
      int valuedecode = decodeButton(value);
      if (valueButton != 2 && press_action == false && i%valuemax ==0){
        valueButton = valuedecode;
        if (valueButton != 0 )
        {
          if(valueButton != 2)
          {
            press_action = true;
          }
        }
      }
      if(i == (valuemax +5 )){
        i = 1;
      }
      if (valueButton == 2)
      {
        valueButton = 0;
      }

      if ((valueButton == 1)){
        off();
        return temps;
      }
      compteur = compteur + 100;
      if (compteur == 1000){ //suppose etre en minutes, live en seconde (60 000 pour en minutes)
        temps = temps -1;
        compteur = 0;
        if (temps == 0){
          //setLines("    Time left   ", "    "+String(temps)+" min     ");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("    Time left   ");
          lcd.setCursor(4, 1);
          lcd.print(temps);
          lcd.setCursor(7, 1);
          lcd.print(" min ");
          off();
          return temps;
        }
        else{
          //setLines("    Time left   ", "    "+String(temps)+" min     ");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("    Time left   ");
          lcd.setCursor(4, 1);
          lcd.print(temps);
          lcd.setCursor(7, 1);
          lcd.print(" min ");
        }
      }
      i +=1;
  }
  end_action = true;
}



void updateButton()
{
    int value = analogRead(PIN_Button);
    int valuedecode = decodeButton(value);
    if (valueButton != valuedecode && value != 1023 && press_action == false){
      valueButton = valuedecode;
      press_action = true;
      Serial.println("save_value");
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






void choseMode(){
    /*
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

    */
    //delay(200);
    //int x = analogRead (0);
    //Serial.println(x);




    if (text == Start){
      Serial.println("start");
      //setLines(" Choose a mode ", "1.Heat  2.Dry  ");
         lcd.setCursor(0, 0);
         lcd.print(" Choose a mode ");
         lcd.setCursor(0, 1);
         lcd.print("1.Heat  2.Dry  ");
      text = Home;

    }

    else if (text == Heating_Timer && press_action == true){
      Serial.println("in Heating Timer");
      //setLines("Timer duration ", "    20 min     ");
         lcd.setCursor(0, 0);
         lcd.print("Timer duration ");
         lcd.setCursor(4, 1);
          lcd.print(temps);
          lcd.setCursor(7, 1);
          lcd.print(" min ");

         Serial.println("in Heating Timer 1");
         end_action = true;
         //vTaskDelay( 200 / portTICK_PERIOD_MS );
      Struct result;
      Serial.println("in Heating Timer 2");
      result = timer(Heating);
      Serial.println("in Heating Timer 3");
      text = result.mode;
      Serial.println("in Heating Timer 4");
      timeDuration = result.timer;
      Serial.println("in Heating Timer 5");
    }
    
    else if (text == Drying_Timer && press_action == true){
      Serial.println("in Drying Timer");
      //setLines("Timer duration ", "    20 min     ");
        lcd.setCursor(0, 0);
        lcd.print("Timer duration ");
        lcd.setCursor(0, 1);
        lcd.print("    20  min     ");
      Struct result;
      result = timer(Drying);
      text = result.mode;
      timeDuration = result.timer;
    }

    else if (text == Heating && press_action == true){
      Serial.println("in Heating");
      //setLines(" Heating mode  ", "1.ON    2.Timer");
        lcd.setCursor(0, 0);
        lcd.print(" Heating mode  ");
        lcd.setCursor(0, 1);
        lcd.print("1.ON    2.Timer");
      
      //String mode[2] = {"  Heating ON   ", "Timer duration "};
      //String option[2] = {"                ", "    20 min     "};
      int title[2] = {Heating_ON, Heating_Timer};
      //text = menu(mode, option, title, text);
            // ajout du menu a la main
        if (press_action == true && end_action == false)
        {
          if (valueButton == 5){ //Back
          text = Back(text);
          }
          else if (valueButton == 2){//2
          //setLines("Timer duration ", "    20 min     ");
              lcd.setCursor(0, 0);
              lcd.print("Timer duration ");
              lcd.setCursor(0, 1);
              lcd.print("    20  min     ");
          text = title[1];
          }
          else if (valueButton == 1){//1
          //setLines("  Heating ON   ","                ");
              lcd.setCursor(0, 0);
              lcd.print("  Heating ON   ");
              lcd.setCursor(0, 1);
              lcd.print("                ");
          text = title[0];
          }
          end_action = true;
        }

      // fin de l'ajout du menu a la main


      
    }
    else if (text == Drying && press_action == true){
      Serial.println("in Drying");
      //setLines("  Drying mode  ", "1.ON    2.Timer");
        lcd.setCursor(0, 0);
        lcd.print("  Drying mode  ");
        lcd.setCursor(0, 1);
        lcd.print("1.ON    2.Timer");
      //String mode[2] = {"    Drying ON   ", "Timer duration "};
      //String option[2] = {"                ", "    20 min     "};
      int title[2] = {Drying_ON, Drying_Timer};
      //text = menu(mode, option, title, text);
            // ajout du menu a la main
        //x = analogRead (0);
        if (press_action == true && end_action == false)
        {
          if (valueButton == 5){ //Back
            text = Back(text);
            }
            else if (valueButton == 2){//2
            //setLines("Timer duration ", "    20 min     ");
                lcd.setCursor(0, 0);
                lcd.print("Timer duration ");
                lcd.setCursor(0, 1);
                lcd.print("    20  min     ");
            text = title[1];
            }
            else if (valueButton == 1){//1
            //setLines("    Drying ON   ", "                ");
                lcd.setCursor(0, 0);
                lcd.print("    Drying ON   ");
                lcd.setCursor(0, 1);
                lcd.print("                ");
            text = title[0];
            }
          end_action = true;
        }

          // fin de l'ajout du menu a la main

      
    }
    
    else if (text == Home){
      Serial.println("home");
      //setLines(" Choose a mode ", "1.Heat  2.Dry  ");
        lcd.setCursor(0, 0);
        lcd.print(" Choose a mode ");
        lcd.setCursor(0, 1);
        lcd.print("1.Heat  2.Dry  ");
      //String mode[2] = {" Heating mode  ", "  Drying mode  "};
      //String option[2] = {"1.ON    2.Timer", "1.ON    2.Timer"};
      int title[2] = {Heating, Drying};
      //text = menu(mode, option, title, text);

      // ajout du menu a la main
        if (press_action == true && end_action == false)
        {
          if (valueButton == 5){ //Back
            text = Back(text);
            }
            else if (valueButton == 2){//2
            //setLines("  Drying mode  ", "1.ON    2.Timer");
                lcd.setCursor(0, 0);
                lcd.print("  Drying mode  ");
                lcd.setCursor(0, 1);
                lcd.print("1.ON    2.Timer");
            text = title[1];
            }
            else if (valueButton == 1){//1
            //setLines(" Heating mode  ", "1.ON    2.Timer");
                lcd.setCursor(0, 0);
                lcd.print(" Heating mode  ");
                lcd.setCursor(0, 1);
                lcd.print("1.ON    2.Timer");
            text = title[0];
            }
          end_action = true;
        }
      // fin de l'ajout du menu a la main
    }

    else if (text == Heating_ON && press_action == true){
      Serial.println("in Heating ON");
      on();
      //setLines("  Heating ON   ", "                ");
        lcd.setCursor(0, 0);
        lcd.print("  Heating ON   ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
      text = Stop(Heating_ON);
    }
    else if (text == Drying_ON && press_action == true){
      Serial.println("in Drying ON");
      on();
      //setLines("    Drying ON   ", "                ");
        lcd.setCursor(0, 0);
        lcd.print("    Drying ON   ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
      text = Stop(Drying_ON);
    }

    else if (text == Left_Heating && press_action == true){
      Serial.println("in Left Heating");
      on();
      timeDuration = LeftTime(timeDuration);
      if (timeDuration == 0){
        text = Back(Left_Heating);
      }
      else {
        if (press_action == true && end_action == false)
        {
          if (valueButton == 1){
            text = Left_Heating;
          }
          else if(valueButton == 5){
            text = Heating;
          }
          end_action = true;
        }
      
      }
    }

    else if (text == Left_Drying && press_action == true){
      Serial.println("in Left Drying");
      on();
      LeftTime(timeDuration);
      timeDuration = LeftTime(timeDuration);
      if (timeDuration == 0){
        text = Back(Left_Drying);
      }
      else {
        if (press_action == true && end_action == false)
        {
          if (valueButton == 1){
            text = Left_Drying;
          }
          else if(valueButton == 5){
            text = Drying;
          }
          end_action = true;
        }
      }
      
    }
}




void taskAffichageEcran(void *pvParameters)
{
    for (;;)
    {
        choseMode();
        Serial.println(valueButton);
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
    //float f = dht.readTemperature(true);
    // Compute heat index in Fahrenheit (the default)
    //float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    //float hic = dht.computeHeatIndex(t, h, false);

// Check if any reads failed and exit early (to try again).
    // if (isnan(h) || isnan(t) || isnan(f)) 
    // {
    //   Serial.println(F("Failed to read from DHT sensor!"));
    //   return;
    // }
    
    if (isnan(h) || isnan(t)) 
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
        vTaskDelay( 400 / portTICK_PERIOD_MS );

    }
    Serial.println(data_t);
    Serial.println(data_h);

}


void setup()
{
    pinMode(PIN_Button, INPUT);
    boolreadSensor = true;
    boolreadSensor = false;
    
    lcd.begin(16, 2);                       // start the library
    Serial.begin(9600);
    dht.begin();
    press_action = false;
    end_action = false;
    mode = 0;
    myservo.attach(A5);  // attaches the servo on pin 9 to the servo object 
    myservo.write(0);
    text = Start;

    //attachInterrupt(digitalPinToInterrupt(PIN_SW1), startMelodie, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(PIN_SW2), startInterruptionBouton2, FALLING);

    xTaskCreate(taskReadButton, (const portCHAR *) "UpdateButton", 128  , NULL , 2 , NULL);
    xTaskCreate(taskAffichageEcran, (const portCHAR *) "AffichageEcran", 128  , NULL , 3 , NULL);
    //xTaskCreate(taskReadSensor, (const portCHAR *) "ReadSensor", 128  , NULL , 1 , NULL);
}

void loop()
{    

}

