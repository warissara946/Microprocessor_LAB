#include <stdio.h>
#define MOTOR_D1_PIN 7
#define MOTOR_D2_PIN 8
#define MOTOR_PWM_PIN 6

int interruptA = 2;
int interruptB = 3;
int delay_count = 0;
int getvalue = 1;
int count = 0;
bool dir = 0; // 0 cw 1 ccw
int errord = 0;
float kp = 2.0;
float ki = 0;
float kd = 0;
float rpm_target = 0;
float rpm_now = 0;
float pid_i=0;
int speed;
int rpm = 0;
volatile float timer1_ovf;
String buff;
String setpointStr;
unsigned long seconds0 = 0, seconds1 = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(MOTOR_D1_PIN,OUTPUT); 
  pinMode(MOTOR_D2_PIN,OUTPUT); 
  pinMode(MOTOR_PWM_PIN,OUTPUT);
  
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(4, OUTPUT);
  
  pinMode(interruptA, INPUT_PULLUP);
  pinMode(interruptB, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interruptA), 
                  a_callback, RISING);
 

  TCCR1A = 0; // initialize timer1
  TCCR1B = 0; //mode 0

  TCCR1B |= (1 << CS12);  // prescaler 256
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  timer1_ovf = 65535 - (0.01 / 0.000016);
  TCNT1 = uint16_t(timer1_ovf); // preload timer
  
  sei(); //Global Interrupt
}

void loop(){
  getValue_function();
  if (rpm != 0){
    digitalWrite(4,1);
  }
  else{
    digitalWrite(4,0);
  }
  
  delay(500);
  
}
void getValue_function(){
  char inByte = Serial.read();
    if(inByte == 's'&& buff.length() == 0){buff += inByte;}
    if(inByte == '-'&& buff.length() == 1){buff += inByte;}
    if(('0'<= inByte) && (inByte <= '9')){buff += inByte;}
    if(inByte == 'n')
    {
      if(buff[0] == 's' && buff[1] != '-')
      {
        for(int i=1;i<buff.length();i++)
        {
         if(('0'<= buff[i]) && (buff[i] <= '9')){setpointStr += buff[i];} 
        }
      }
      if(buff[0] == 's' && buff[1] == '-')
      {
        for(int i=1;i<buff.length();i++)
        {
         if(buff[i] == '-' && setpointStr == ""){setpointStr += buff[i];} 
         if(('0'<= buff[i]) && (buff[i] <= '9')){setpointStr += buff[i];}
        }
      }
      
      int temp_speed = setpointStr.toInt();
      getvalue = map(temp_speed, -100, 100, -255, 255);
      
      if(getvalue > 255){
        getvalue = 255;
      }
      if(getvalue < -255){
        getvalue = -255;
      }
      buff = "";
      setpointStr = "";
      Serial.println(getvalue);
      
  }
}
void a_callback(){
  if (!digitalRead(interruptB)){count++;}
  else if(digitalRead(interruptB)){count--;}
}

void moveForward(int speed) {
    digitalWrite(MOTOR_D1_PIN,HIGH);
    digitalWrite(MOTOR_D2_PIN,LOW); 
    analogWrite(MOTOR_PWM_PIN,speed);
}

void moveBackward(int speed) {
    digitalWrite(MOTOR_D1_PIN,LOW);
    digitalWrite(MOTOR_D2_PIN,HIGH); 
    analogWrite(MOTOR_PWM_PIN,speed);
}
void setSpeed(int speed) { 
  if (speed > 0) {
    if (speed > 255) {
    	speed = 255;
    }
  	moveForward(speed);
  } else if (speed < 0) {  
  	speed = speed*(-1);
    
    if (speed > 255) {
    	speed = 255;
    }
    moveBackward(speed);
  }
  else {
    moveForward(1);
  }
}

ISR(TIMER1_OVF_vect)
{
  
  bool cw_read = digitalRead(9); // cw dir = 0
  bool ccw_read = digitalRead(10); // ccw dir =1
   
  if (!cw_read){getvalue = -1*abs(getvalue);}
  if (!ccw_read){getvalue = abs(getvalue);}
  //seconds1 = millis();
  rpm = (count*6000/226*(0.01));
  
  float error_now = getvalue - rpm; //  error
  pid_i += error_now*0.01;
  float pid_d = (error_now - errord)/0.01;
  int pid = (kp*error_now) + (ki*pid_i) + (kd*pid_d);
  if(pid > 255)
  {
    pid = 255;
  }
    if(pid < -255)
  {
    pid = -255;
  }
  
  setSpeed(pid); 
  
  //seconds0 = seconds1;
  errord = error_now;
  
  count = 0;
  Serial.print(getvalue);
  //Serial.println(rpm);
  //Serial.println(error_now);
  //Serial.println(pid_i);
  //Serial.println(errord);
  //Serial.println(pid_d);
  Serial.print(",");
  Serial.println(pid);
  //Serial.println("*******");

  TCNT1 = uint16_t(timer1_ovf);
}