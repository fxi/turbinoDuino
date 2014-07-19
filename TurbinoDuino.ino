/* 
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <f@fxi.io> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

/*
 * ---------------------------     TurbinoDuino   -----------------------------
 * Minimal code for arduino based goldSprint for two opponents
 * This programm is not suitable for production GoldSprint :
 * this is an early proof of concept version.
 * However, it worked nicely during 8 hours in a GoldSprint contest.
 * ----------------------------------------------------------------------------
*/



#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int a0;
int a1;
int s =0 ;
int a =0;

int dist;

const int velo1=0;
const int velo2=1;
volatile byte c1;
volatile byte c2;
unsigned int rpm1;
unsigned int rpm2;
unsigned long timeold1;
unsigned long timeold2;
const int btn_select = 6;
int statut_c;
int statut_1 ;
int statut_2 ;
int t1;
int t2;
String score1;
String score2;

int temps;


void setup() {
 Serial.begin(9600); 
 lcd.begin(16, 2); 
 pinMode(velo1,INPUT);
 pinMode(velo2,INPUT);
 pinMode(btn_select,INPUT);
 reset();

}





///--- LOOP --- ///

void loop() {
 menu();
 score();
 reset();
 delay(100);
}

///--- SCORES --- ///


void menu(){
 //selector(digitalRead(btn_select));
 lcd.clear();
 lcd.print("TurbinoDuino");
 lcd.setCursor(0,1);
 lcd.print("Press Start");


 while(1){
  if(digitalRead(btn_select)==HIGH){
   course();
   break;
  };
  delay(100);
 };


}


///--- COURSE --- ///

void course(){
 for (int i=5; i>= 0; i--){ 
  lcd.clear();
  lcd.print("Depart dans "); 
  lcd.print(i);
  delay(1000);
 };

 attachInterrupt(0, compteur1, RISING);
 attachInterrupt(1, compteur2, RISING);

 temps = millis();

 while (statut_1 ==1 || statut_2==1){
  if(digitalRead(btn_select)==HIGH){
   delay(300);
   break;
  };
  lcd.clear();
  lcd.print("Bleu  = ");
  lcd.print(c1);
  lcd.setCursor(0,1);
  lcd.print("JAUNE = ");
  lcd.print(c2);
  delay(500);
 };

 detachInterrupt(0);
 detachInterrupt(1);
};


void score(){
 int t = 1;
 lcd.clear();
 lcd.print("SCORE B=");
 lcd.print(score1);
 lcd.setCursor(0,1);
 lcd.print("SCORE J=");
 lcd.print(score2);


 while(1){
  if(digitalRead(btn_select)==HIGH){
   delay(300);
   break;
  };
  delay(100);
 };     
};

///--- Conversion --- ///


String convert(int x, int y){
 int r;
 int ss;
 int cc;
 r = x-y;
 String sep = ":";
 ss= r/1000 ;
 cc=r-ss*1000 ;
 String res =  ss+sep+cc; 

 return(res);
}



///--- COMPTEURS --- ///

void compteur1()
{
 static unsigned long itold1 = 0;
 unsigned long it1 = millis();
 if (it1 - itold1 > 100)
 {
  c1++;

  if(c1>=dist){
   statut_1 = 0 ;
   t1=millis();
   detachInterrupt(0); 
   score1=convert(t1,temps);
   if(c2==0){statut_2 = 0 ;}

   Serial.print("Bleu");
   Serial.print(score1);
   Serial.print("\n");
   detachInterrupt(0);
  };
 };
 itold1 = it1;

} 

void compteur2()
{
 static unsigned long itold2 = 0;
 unsigned long it2 = millis();
 if (it2 - itold2 > 100)
 {
  c2++;

  if(c2>=dist){
   detachInterrupt(1);
   statut_2 = 0 ;
   t2=millis();
   score2=convert(t2,temps);
   if(c1==0){statut_1 = 0 ;}

   Serial.print("Jaune");
   Serial.print(score2);
   Serial.print("\n");
   detachInterrupt(1);
  };
 }
 itold2 = it2;

} 



void reset(){
 dist = 50;
 a0 = 0;
 a1 = 0;
 c1 = 0;
 c2 = 0;
 rpm1 = 0;
 rpm2 = 0;
 timeold1 = 0;
 timeold1 = 2;
 s=0;
 t1 =0;
 t2 =0;

 temps=0;

 statut_c=1;
 statut_1=1;
 statut_2=1;

 score1="";
 score2="";

 detachInterrupt(1);

 Serial.print("RESET OK");

}


