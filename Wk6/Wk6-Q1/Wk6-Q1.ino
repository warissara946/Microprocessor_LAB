#include <ThreeWire.h>  
#include <RtcDS1302.h>
#define led 6
ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire>Rtc(myWire);
int sectime =0;
//void printDateTime(const RtcDateTime& dt){
//  Serial.print(dt.Year(), DEC);
//  Serial.print('/');
//  Serial.print(dt.Month(), DEC);
//  Serial.print('/');
//  Serial.print(dt.Day(), DEC);
//  Serial.print(' ');
//  Serial.print(dt.Hour(), DEC);
//  Serial.print(':');
//  Serial.print(dt.Minute(), DEC);
//  Serial.print(':');
//  Serial.print(dt.Second(), DEC);
//  Serial.println();
//  Serial.print("Total Seconds since 1/1/2000 : ");
//  Serial.print(dt.TotalSeconds());
//  Serial.println();
//  }
void setup() {
  Serial.begin(9600);
  Rtc.Begin();
  RtcDateTime setting_time(2020,2,2,23,31,10);// year, month, dayOfMonth, hour, minute, second
//  printDateTime(setting_time);//Try to comment This line
  Rtc.SetDateTime(setting_time);
  pinMode(led, OUTPUT);
  }
  void loop() {
    toggle();
 
    delay(500);
}
void toggle(){
  RtcDateTime now = Rtc.GetDateTime();
  Serial.println(sectime);
  sectime = (now.Hour() * 60 * 100) + (now.Minute() * 100 ) +(now.Second());
  if (sectime % 2 == 0){
    digitalWrite(led,1);
  }
  else{
    digitalWrite(led,0);
  }
}
