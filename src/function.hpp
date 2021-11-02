int decodeButton(int value)
{
    int numberButton = 0;
    if (value <= 60) {
       //lcd.print (" Right");
       numberButton = 5;
    }
    else if ((value <= 400)&&(value >= 61)){
       //lcd.print ("    Up");
       numberButton = 3;
    }
    else if ((value <= 600)&&(value >= 401)){
       //lcd.print ("  Down");
       numberButton = 4;
    }
    else if ((value <= 800)&&(value >= 601)){
       //lcd.print ("  Left");
       numberButton = 2;
    }
    else if ((value <= 1000)&&(value >= 801)){
       //lcd.print ("Select");
       numberButton = 1;
    }
    return numberButton;
}