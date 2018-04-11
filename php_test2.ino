#include <SPI.h>
#include "WizFi250.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);   //lcd주소 16칸 2줄 선언

//wifi 연결 변수
char ssid[] = "jm";   //wifi 이름
char pass[] = "11111111";  //wifi passwd
String apiKey = "8RUOBK0AK028O9AD";
char server[] = "waytech.kr";   //서버 주소
int status = WL_IDLE_STATUS;
WiFiClient client;

volatile int state = LOW;

//적외선 거리센서
char ir1 = A0;   //사람 재실여부 확인하는 적외선 센서
int distance1;   //적외선 센서값 저장 변수
float cm1;   //적외선센서 측정값 cm으로 단위변환
#define VOLTS_PER_UNIT    .0049F        // (.0049 for 10 bit A-D)


int led1 = 7;   //장시간 이용시 점등하는 led
int btn1 = 13;   //시간 초기화하는 버튼
unsigned long time1;   //시간 초기화하기 위한 변수
unsigned long currentTime1;    //사용시간 측정 변수

unsigned long time0;
unsigned long sendTime;


void wifiConnect();    //WIFI 연결 함수
void printWifiStatus();   //wifi 상태
void getTime();   //시간 측정 함수
void sensorDistance();  //거리 측정 함수(재실여부 확인)
void sendThingspeak();   //서버로 측정 값 전송
void sendAllData();
void ledOn(){
  if(currentTime1 > 3000){
    digitalWrite(led1,HIGH);
//    Serial.print("time: ");
//    Serial.println(time);
//    Serial.print("millis: ");
//    Serial.println(millis());
//    Serial.print("currentTime: ");
//    Serial.println(currentTime1);
//    Serial.print("millis()-time: ");
//    Serial.println(millis()-time);
    btnPush();

  }
  else
  { 
    digitalWrite(led1, LOW);
  } 
}

void btnPush(){
  
    if(digitalRead(btn1) == HIGH){
      time1=millis();
      Serial.println("=============================");
//      Serial.print("time: ");
//      Serial.println(time);
      Serial.print("currentTime: ");
      Serial.println(currentTime1);
//      Serial.print("millis()-time: ");
//      Serial.println(millis()-time);
      Serial.println("=============================");
      delay(100);
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led1, OUTPUT);   //led핀 출력 설정
  pinMode(btn1, INPUT);    //btn핀 입력 설정

  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("hello");
 
  wifiConnect();
  Timer1.initialize(100000);

  Timer1.attachInterrupt(btnPush);
//  Timer1.attachInterrupt(ledOn);
  
  //printWifiStatus();

}

void loop() {  
  sendTime = millis() - time0;

  sensorDistance();

  if(cm1> 40){
      getTime();
      lcd.setCursor(0,0);
      lcd.print("      use!      ");
//    lcd.setCursor(0,1);
//    lcd.print("CM= ");
//    lcd.print(cm);
  }
  else{
    time1=millis();
    digitalWrite(led1, LOW);
    lcd.setCursor(0,0);
    lcd.print("     empty!     ");

  }

  if(sendTime > 20000){
    Serial.println(sendTime);
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
  ledOn();

}


void sensorDistance(){
  distance1 = analogRead(ir1);    //적외선센서 아날로그값 저장
  float volts = (float)distance1 * VOLTS_PER_UNIT;   //저장한 값을 volt 단위로 변환
  cm1 = 60.495 * pow(volts,-1.1904);   //cm단위로 변환

}

void sendThingspeak(){
if(client.connect(server, 80)){
    Serial.println("Connected to server!!");

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(cm1);
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

    Serial.print("distance: ");
    Serial.println(cm1);
  }
  //client.stop();
 //Serial.println("Waiting…");

}

void sendAllData(){
  if (client.connect(server, 80)) {   // Http 요청 헤더
    Serial.println("Connected...");
    client.print("GET /iot.php?"); //읽을 PHP 파일
    client.print("cm= ");
    client.print(cm1);
    /* 거리센서 말고 보낼데이터 또 있으면 추가
    client.print("&&");
    client.print("temperature=");   
    client.print(temperature);
    */
    client.println(" HTTP/1.1");
    client.print("Host: " ); //요청을 보낼 서버의 주소
    client.println(server);   //안되면 걍 바로 때려박기
    client.println("Connection: close" );
    client.println();
    client.stop();

    Serial.print("cm= ");
    Serial.println(cm1);
    }
    else{
      Serial.println("Connection failed");  
    }
}


