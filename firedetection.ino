#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
SoftwareSerial mySerial(9, 10);

int sensor=A1;
float fire_read,fire_alert_val,fire_shut_val;
int sms_count=0,Fire_Set;


void setup()
{
  pinMode(sensor,INPUT);
  mySerial.begin(9600);   
  Serial.begin(9600);    
  lcd.begin(16,2);  
  delay(500);
}

void loop()
{
CheckFire();
CheckShutDown();
}

void CheckFire()
{
lcd.setCursor(0,0);
lcd.print("Fire Scan - ON");
fire_alert_val=CheckTemp();
if(fire_alert_val>50)
{

  Serial.print("FIRE DETECTED IN THE ROOM....!!!!!!!!!!!!!!!!!");
   SetAlert(); // Function to send SMS Alerts

  

   
}
}

float CheckTemp()
{
fire_read=analogRead(sensor); // reads the sensor output (Vout of LM358)

Serial.println(fire_read);
delay(1000);
return fire_read; // returns temperature value in degree celsius
}

void SetAlert()
{
while(sms_count<3) //Number of SMS Alerts to be sent
{  
SendTextMessage(); // Function to send AT Commands to GSM module
}
Fire_Set=1; 
lcd.setCursor(0,1);
lcd.print("Fire Alert! SMS Sent!");
}

void CheckShutDown()
{
if(Fire_Set==1)
{

fire_shut_val=CheckTemp();
if(fire_shut_val<28)
{
lcd.setCursor(0,1);
lcd.print("Fire Shut! SAFE NOW");
sms_count=0;
Fire_Set=0;
}}}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(2000);
  mySerial.println("AT+CMGS=\"+918712779270\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("Fire detected in the ROOM!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(5000);
   mySerial.println("AT+CMGS=\"+917416342496\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("Fire detected in the ROOM!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the message stopping character
  delay(5000);
  sms_count++;
}
