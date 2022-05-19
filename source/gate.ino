#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
Servo myServoMotor;
#define buzzer 12
#define myservo 3
#define ledRGB 7
#define ledInterface 5
LiquidCrystal lcd(4,2,9,10,11,8);
char keys[4][4]=
{
  {'1','2','3','/'},
  {'4','5','6','*'},
  {'7','8','9','-'},
  {'.','0','=','+'}
};
byte rowPins[4]={1,A5,0,A4};//key pad rows
byte colPins[4]={A0,A2,A1,A3};//key pad cols
Keypad keypad1=Keypad(makeKeymap(keys),rowPins,colPins,4,4);



void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  pinMode(buzzer,OUTPUT);
  pinMode(ledInterface,OUTPUT);
  digitalWrite(ledInterface,LOW);
  EEPROM.write(0,0);
  myServoMotor.attach(myservo);
  myServoMotor.write(70);
  pinMode(ledRGB,OUTPUT);
  digitalWrite(ledRGB,HIGH);
  String baka="Baka Project";
  for (int i=0;i<12;i++)
  {
    lcd.print(baka[i]);
    delay(500);
    
  }
  lcd.setCursor(0,1);
  lcd.print("Welcome");
  delay(1500);
  lcd.clear();
  delay(800);
}


int t=0;
void loop()
{
  if (EEPROM.read(0)<2)
  {
    String vv="New Password";
    if(t==0)
      lcd.print(vv);
    else if (t==1)
    {
      vv="Confirm";
      lcd.clear();
      lcd.print(vv);
    }
    
    lcd.setCursor(0,1);
    lcd.print("=>");
    
    String password="";
    
    while (true)
    {
      char key =keypad1.getKey();
      if (key == '=')
      {
        if(t!=0)
        {
          if(!(ReadFromEEPROM(2).equals( password)))
          {
            EEPROM.write(0,0);
            t=0;
            lcd.clear();
            break;
          }
        }
        
        WriteToEEPROM(2,password);
        lcd.clear();
        lcd.print("password set");
        lcd.setCursor(0,1);
        lcd.print("succeeded");
        t++;
        EEPROM.write(0,t);
        EEPROM.write(1,0);
        delay(200);
        lcd.clear();
        break;
      }
      else if (key)
      {
        password+=String(key);
        lcd.print("*");
      }
    }
    
  }
  else 
  {
    label:
    delay(800);
    String password=ReadFromEEPROM(2);
    bool IsCorrect=false;
    lcd.clear();
    lcd.print("Enter password");
    lcd.setCursor(0,1);
    lcd.print("=>");
    int noTrials=EEPROM.read(1);
    for (int i=noTrials;i<3;i++)
    {
      String  userpass="";
      while (true)
      {
        char key=keypad1.getKey();
        if (key == '=')
        {
          lcd.clear();
          if(userpass.equals(password))
          {
            lcd.clear();
            lcd.print("Correct");
            IsCorrect=true;
          }
          else 
          {
            lcd.clear();
            lcd.print("InCorrect");
            delay(1000);
            lcd.clear();
            IsCorrect=false;
            userpass="";
            lcd.print("Enter Password");
            lcd.setCursor(0,1);
            lcd.print("=>");
            
          }
          EEPROM.write(1,noTrials+1);
          break;
        }
        else if (key)
        {
          userpass+=String (key);
          lcd.print("*");
        }
        
      }
      if (IsCorrect)
        {
          break;
        }
    }

    if (IsCorrect)
    {
      while (true)
      {
        EEPROM.write(1,0);
        //write code here
        delay(300);
        digitalWrite(ledRGB,LOW);
        myServoMotor.write(160);
        delay(5000);
        myServoMotor.write(70);
        delay(7000);
        digitalWrite(ledRGB,HIGH);
        
        goto label;
      }
    }
    else 
    {
      lcd.clear();
      lcd.print("Alarm");
      digitalWrite(buzzer,HIGH);
      while(true);
    }

    
  }


  
}









void WriteToEEPROM(int pos,const String &str)
{
  byte len =str.length();
  EEPROM.write(pos,len);
  for (int i=0;i<len;i++)
  {
    EEPROM.write(pos+1+i,str[i]);
  }
  
}


String ReadFromEEPROM(int pos)
{
  byte len =EEPROM.read(pos);
  char string[len+1];
  for (int i=0;i<len;i++)
  {
    string[i]=EEPROM.read(pos+i+1);
  }
  string[len]='\0';
  return String (string);
}
