#include<SPI.h>

#define DATA 11
#define CLK 13
#define SS 10
#define SS_2 9
#define btnIn 2
#define btnDe 3
#define btnCount 4
#define buzzer 5
#define a  0b00000001
#define b  0b00000010
#define c  0b00000100
#define d  0b00001000
#define e  0b00010000
#define f  0b00100000
#define g  0b01000000
#define dp 0b10000000

#define num0 a | b | c | d | e | f
#define num1 b | c
#define num2 a | b | d | e | g
#define num3 a | b | c | d | g
#define num4 b | c | f | g
#define num5 a | c | d | f | g
#define num6 a | c | d | e | f | g
#define num7 a | b | c | f
#define num8 a | b | c | d | e | f | g 
#define num9 a | b | c | d | f | g

unsigned char num[10] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};
int count = 0, number=0,unit =0,counter=0,unit1,unit2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SS, OUTPUT);
  pinMode(SS_2, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  change_number();
  delay(500);
}
  
void spi_1(unsigned char cData){
  shiftOut(DATA, CLK, MSBFIRST, cData);
  digitalWrite(SS, 1);
  digitalWrite(SS, 0);
  digitalWrite(SS_2, 0);
}
void spi_2(unsigned char cData){
  shiftOut(DATA, CLK, MSBFIRST, cData);
  digitalWrite(SS, 0);
  digitalWrite(SS_2, 1);
  digitalWrite(SS_2, 0);
}
void change_number(){
   unit2 = num[count];
   unit = counter / 10;
   unit = floor(unit);
   unit1 = num[unit];
   if (unit <= 9 && count < 9){
    spi_1(unit1);
    spi_2(unit2);
    count++;
   }
   else if(count>=9){
    count = 0;
    spi_1(unit1);
    spi_2(unit2);
   }
   else{
    counter = 0;
   }
   counter++;
}
