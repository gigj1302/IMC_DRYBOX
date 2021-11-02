#include <LiquidCrystal.h>

void modeMenu(){
    lcd.setCursor(0, 0);
    lcd.print(" Choose a mode ! ");
    lcd.setCursor(0, 1);
    lcd.print("1.Heat  2.Dry  ");

}