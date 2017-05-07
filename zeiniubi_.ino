#include <Servo.h>

#define x A0        
#define y A1
#define sw A2 
#define led1 3  //red
#define led2 4  //qing
#define led3 8  //yellow
#define led4 6  //blue


Servo myServo; 
Servo myServo_b; 
Servo myServo_c;
Servo myServo_d;
int select=0;
int select_t1=12;
int select_t2=11;
int select_t3=13;
int data;
int receive_r=A3;
int receive;

int a=0;
int b=0;
int c=1;
int xx;
int yy;

int ssww;

void setup() {
   
  pinMode(sw, INPUT);    //设置为输入模式  
  digitalWrite(sw,HIGH); 
  Serial.begin(9600);
    myServo_d.attach(10);
    myServo_c.attach(9);
     myServo_b.attach(7);
      myServo.attach(5);

      pinMode(select_t1,OUTPUT);
      pinMode(select_t2,OUTPUT);
      pinMode(select_t3,OUTPUT);
}

void loop() {
  xx= analogRead(x);
       yy= analogRead(y);
       ssww=analogRead(sw);
//      data=digitalRead(select_r);
      Serial.println(data);
      
      receive=digitalRead(receive_r);
      
        if(xx>750)
        a=1;
       else if(xx<250)
        a=0;
       if (yy>750)
       b=1;
       else if(yy<250)
        b=0;
  
       if(ssww>500)
        {
          c=1;
          digitalWrite(select_t3,1);
        }
       else
       { 
        c=0;
        digitalWrite(select_t3,0);
       }

         if ((a==0) && (b==0))
        {digitalWrite(led1,HIGH);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led4,LOW);
        select=1;
        digitalWrite(select_t1,0);
        digitalWrite(select_t2,0);
        }
     else if ((a==1) && (b==0))
        {
          digitalWrite(led1,LOW);
        digitalWrite(led2,HIGH);
        digitalWrite(led3,LOW);
        digitalWrite(led4,LOW);
        select=2;
        digitalWrite(select_t1,0);
        digitalWrite(select_t2,1);
        }
        else if ((a==0) &&( b==1))
        {
          digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,HIGH);
        digitalWrite(led4,LOW);
        select=3;
        digitalWrite(select_t1,1);
        digitalWrite(select_t2,0);
        }
        else if ((a==1) && (b==1))
        {
          digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led4,HIGH);
        select=4;
        digitalWrite(select_t1,1);
        digitalWrite(select_t2,1);
        }
        


        
 //  njbbiobhlbkbkbibbvjklfjkvnuruvrfrhiu

  // Serial.print(select);//////////////////////////////////////加回来
  
   if((select==1)&&(receive==1))
   { 
  myServo.writeMicroseconds(1300);
  delay(2000);
  myServo.writeMicroseconds(1500);
  delay(2000);
   }
  else if((select==3)&&(receive==1))
  { 
  myServo_c.writeMicroseconds(1300);
  delay(2000);
  myServo_c.writeMicroseconds(1500);
  delay(2000);
  }
  else if((select==4)&&(receive==1))
   { 
    myServo_d.writeMicroseconds(1300);
    delay(2000);
    myServo_d.writeMicroseconds(1500);
    delay(2000);
   }
   else if((select==2)&&(receive==1))
    {
    myServo_b.writeMicroseconds(1300);
    delay(2000);
    myServo_b.writeMicroseconds(1500);
    delay(2000);
    }
  

}
