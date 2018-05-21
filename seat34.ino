#include <SPI.h>
#include "WizFi250.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

LiquidCrystal_I2C lcd1(0x26, 16, 2);   //lcd주소 16칸 2줄 선언  26
LiquidCrystal_I2C lcd2(0x27, 16, 2);   //lcd주소 16칸 2줄 선언

//wifi 연결 변수
char ssid[] = "jm";   //wifi 이름
char pass[] = "11111111";  //wifi passwd
String apiKey = "8RUOBK0AK028O9AD";
char server[] = "waytech.kr";   //서버 주소 api.thingspeak.com  waytech.kr
int status = WL_IDLE_STATUS;
WiFiClient client;

volatile int state = LOW;

//적외선 거리센서
char ir1 = A0;   //사람 재실여부 확인하는 적외선 센서
char ir2 = A1;
int distance1;   //적외선 센서값 저장 변수
int distance2;
float cm1;   //적외선센서 측정값 cm으로 단위변환
float cm2;
#define VOLTS_PER_UNIT    .0049F        // (.0049 for 10 bit A-D)

int seatState1;
int seatState2;

int seatTime1;
int seatTime2;


int led1 = 7;   //장시간 이용시 점등하는 led
int led2 = 8;
int btn1 = 9;   //시간 초기화하는 버튼
int btn2 = 13;
unsigned long time1;   //시간 초기화하기 위한 변수
unsigned long time2;
unsigned long currentTime1;    //사용시간 측정 변수
unsigned long currentTime2;


unsigned long time0;    //서버로 보낼 시간 측정
unsigned long sendTime;

//한글설정 헤더파일로 만들고싶다
//사
byte newChar1[8] = {
        B01010,
        B01010,
        B10110,
        B10111,
        B10110,
        B10110,
        B10110,
        B10110
};
//용
byte newChar2[8] = {
        B01110,
        B10001,
        B01110,
        B01010,
        B11111,
        B01110,
        B10001,
        B01110
};
//중
byte newChar3[8] = {
        B11111,
        B00100,
        B01010,
        B11111,
        B00100,
        B01110,
        B10001,
        B01110
};

//비
byte newChar4[8] = {
        B10101,
        B10101,
        B10101,
        B11101,
        B10101,
        B10101,
        B10101,
        B11101
};
//어
byte newChar5[8] = {
        B01001,
        B10101,
        B10101,
        B10111,
        B10101,
        B10101,
        B10101,
        B01001
};
//있
byte newChar6[8] = {
        B01001,
        B10101,
        B10101,
        B01001,
        B00001,
        B01010,
        B10101,
        B10101
};
//음
byte newChar7[8] = {
        B01110,
        B10001,
        B01110,
        B11111,
        B00000,
        B11111,
        B10001,
        B11111
};


void wifiConnect();    //WIFI 연결 함수
void printWifiStatus();   //wifi 상태
void getTime();   //시간 측정 함수
void sensorDistance();  //거리 측정 함수(재실여부 확인)
void sendThingspeak();   //서버로 측정 값 전송
void sendAllData();
void ledOn();         //일정 시간이 되었을 때 Led 켜기
void btnPush();       //버튼이 눌렸을 때 led 끄기 및 시간 초기화
void seatState();     //칸 상태 0:비어있음 1:사용중 2:경고 3:위급
void korean(){
  lcd1.createChar(0, newChar1);
  lcd1.createChar(1, newChar2);
  lcd1.createChar(2, newChar3);
  lcd1.createChar(3, newChar4);
  lcd1.createChar(4, newChar5);
  lcd1.createChar(5, newChar6);
  lcd1.createChar(6, newChar7);

  lcd2.createChar(0, newChar1);
  lcd2.createChar(1, newChar2);
  lcd2.createChar(2, newChar3);
  lcd2.createChar(3, newChar4);
  lcd2.createChar(4, newChar5);
  lcd2.createChar(5, newChar6);
  lcd2.createChar(6, newChar7);
}
void changeTime(){
  seatTime1=currentTime1/1000;
  seatTime2=currentTime2/1000;

  Serial.println(seatTime1);
  Serial.println(seatTime2);
}
void lcd1UseChar(){

   lcd1.setCursor(4,0);
   lcd1.print(" ");
   lcd1.write(0);
   lcd1.print(" ");
   lcd1.write(1);
   lcd1.print(" ");
   lcd1.write(2);
   lcd1.print(" ");
  
}
void lcd1EptChar(){

   lcd1.setCursor(4,0);
   lcd1.write(3);
   lcd1.print(" ");
   lcd1.write(4);
   lcd1.print(" ");
   lcd1.write(5);
   lcd1.print(" ");
   lcd1.write(6);
}
void lcd2UseChar(){

   lcd2.setCursor(4,0);
   lcd2.print(" ");
   lcd2.write(0);
   lcd2.print(" ");
   lcd2.write(1);
   lcd2.print(" ");
   lcd2.write(2);
   lcd2.print(" ");
  
}
void lcd2EptChar(){

   lcd2.setCursor(4,0);
   lcd2.write(3);
   lcd2.print(" ");
   lcd2.write(4);
   lcd2.print(" ");
   lcd2.write(5);
   lcd2.print(" ");
   lcd2.write(6);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led1, OUTPUT);   //led핀 출력 설정
  pinMode(btn1, INPUT);    //btn핀 입력 설정
  pinMode(led2, OUTPUT);   //led핀 출력 설정
  pinMode(btn2, INPUT);    //btn핀 입력 설정

  korean();
  
  lcd1.init();
  lcd1.backlight();
  lcd1.setCursor(0,0);
  lcd1.print("Seat No.1");

  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(0,0);
  lcd2.print("Seat No.2");
 
  wifiConnect();
  Timer1.initialize(100000);

  Timer1.attachInterrupt(btnPush);
//  Timer1.attachInterrupt(ledOn);
  
  //printWifiStatus();

}

void loop() {  
  
  sendTime = millis() - time0;  //서버로 측정값 시간초기화

  sensorDistance();   //거리측정 및 측정거리 cm으로 변환

  if(cm1 < 40){
      getTime();
      lcd1.setCursor(0,0);
    lcd1.print("    ");
        lcd1.setCursor(4,0);
   lcd1.print(" ");
   lcd1.write(0);
   lcd1.print(" ");
   lcd1.write(1);
   lcd1.print(" ");
   lcd1.write(2);
   lcd1.print(" ");
  }
  else{
    time1=millis();
    currentTime1=0;
    digitalWrite(led1, LOW);
    lcd1.setCursor(0,0);
    lcd1.print("    ");
    lcd1.setCursor(4,0);
   lcd1.write(3);
   lcd1.print(" ");
   lcd1.write(4);
   lcd1.print(" ");
   lcd1.write(5);
   lcd1.print(" ");
   lcd1.write(6);

  }
  
  
  if(cm2 < 40){
      getTime();
      lcd2UseChar();
  }
  else{
    time2=millis();
    currentTime2=0;
    digitalWrite(led2, LOW);
    lcd2EptChar();

  }
  
  if(sendTime > 20000){   //일정 시간이 되면 서버로 값 전송

   // changeTime();
    seatState();
    sendAllData();
//    sendThingspeak();
    time0 = millis();
    
    
  }
}

void wifiConnect(){
  WiFi.init();
  //WIFI 쉴드 없을 때
  if(WiFi.status() == WL_NO_SHIELD){
    Serial.println("WiFi shield not present");
    while(true);
  }
  //WIFI 연결 안 되어 있을 때
  while(status != WL_CONNECTED){
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid,pass);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi); 
  Serial.println(" dBm");
}

void getTime(){ 
  currentTime1 =  millis() - time1;
  currentTime2 =  millis() - time2;
  ledOn();

}


void sensorDistance(){
  distance1 = analogRead(ir1);    //적외선센서 아날로그값 저장
  distance2 = analogRead(ir2);
  float volts1 = (float)distance1 * VOLTS_PER_UNIT;   //저장한 값을 volt 단위로 변환
  float volts2 = (float)distance2 * VOLTS_PER_UNIT;
  cm1 = 60.495 * pow(volts1,-1.1904);   //cm단위로 변환
  cm2 = 60.495 * pow(volts2,-1.1904);
}

void ledOn(){
  if(currentTime1 > 5000){
    digitalWrite(led1, HIGH);
    btnPush();

  }
  else
  { 
    digitalWrite(led1, LOW);
  } 


  if(currentTime2 > 5000){
    digitalWrite(led2, HIGH);
    btnPush();

  }
  else
  { 
    digitalWrite(led2, LOW);
  } 
}

void btnPush(){
  
    if(digitalRead(btn1) == HIGH){
      time1=millis();
      Serial.println("=============================");
//      Serial.print("time: ");
//      Serial.println(time);
      Serial.print("currentTime1: ");
      Serial.println(currentTime1);
//      Serial.print("millis()-time: ");
//      Serial.println(millis()-time);
      Serial.println("=============================");
      delay(100);
    }
    else if(digitalRead(btn2) == HIGH){
      time2=millis();
      Serial.println("=============================");
      Serial.print("currentTime2: ");
      Serial.println(currentTime2);
      Serial.println("=============================");
     delay(100);
    }
}

void seatState(){
  if(currentTime1<500){
      seatState1 = 0;
  }
  else if(currentTime1<5000){
    seatState1 = 1;
  }
  else if(currentTime1<15000){
    seatState1 = 2;
  }
  else{
    seatState1 = 3;
  }

  if(currentTime2<500){
      seatState2 = 0;
  }
  else if(currentTime2<5000){
    seatState2 = 1;
  }
  else if(currentTime2<15000){
    seatState2 = 2;
  }
  else{
    seatState2 = 3;
  }
}

void sendThingspeak(){
if(client.connect(server, 80)){
    Serial.println("Connected to server!!");

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(cm1);
    postStr += "&field2=";
    postStr += String(cm2);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println(cm1);
    Serial.println(cm2);
  }
  //client.stop();
 //Serial.println("Waiting…");

}

void sendAllData(){
  if (client.connect(server, 80)) {
    client.print("GET /toms/arduino2.php?"); //읽을 PHP 파일
    client.print("cm3=");
    client.print(cm1);
    client.print("&&");
    client.print("cm4=");
    client.print(cm2);
    client.print("&&");
    client.print("seatState3=");
    client.print(seatState1);
    client.print("&&");
    client.print("seatState4=");
    client.print(seatState2);
    /* 거리센서 말고 보낼데이터 또 있으면 추가
    client.print("&&");
    client.print("temperature=");   
    client.print(temperature);
    */
    client.println(" HTTP/1.1");
    client.println("Host: waytech.kr" ); //요청을 보낼 서버의 주소
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println("Connection: close" );
    client.print("\r\n\r\n"); 
    client.stop();
    }
    else{
      Serial.println("Connection failed");  
    }
}


