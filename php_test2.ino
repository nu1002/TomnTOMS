#include <SPI.h>
#include "WizFi250.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   //lcd주소 16칸 2줄 선언

//wifi 연결 변수
char ssid[] = "밴재";   //wifi 이름
char pass[] = "dmswo0922";  //wifi passwd
String apiKey = "8RUOBK0AK028O9AD";
char server[] = "waytech.kr";   //서버 주소
int status = WL_IDLE_STATUS;
WiFiClient client;


//적외선 거리센서
char ir = A0;   //사람 재실여부 확인하는 적외선 센서
int distance;   //적외선 센서값 저장 변수
float cm;   //적외선센서 측정값 cm으로 단위변환
#define VOLTS_PER_UNIT    .0049F        // (.0049 for 10 bit A-D)


int led = 12;   //장시간 이용시 점등하는 led
int btn = 9;   //시간 초기화하는 버튼
unsigned long time;   //시간 초기화하기 위한 변수
unsigned long currentTime;    //사용시간 측정 변수


void wifiConnect();    //WIFI 연결 함수
void printWifiStatus();   //wifi 상태
void getTime();   //시간 측정 함수
void sensorDistance();  //거리 측정 함수(재실여부 확인)
void sendThingspeak();   //서버로 측정 값 전송
void sendAllData();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);   //led핀 출력 설정
  pinMode(btn, INPUT);    //btn핀 입력 설정

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("eunjae babo");
  lcd.setCursor(0,1);
  lcd.print("nuri mungchung");
  wifiConnect();
  printWifiStatus();
}

void loop() {
  
  sensorDistance();
  if(cm > 45){
    getTime();
    lcd.setCursor(0,0);
    lcd.print("      use!      ");
    lcd.setCursor(0,1);
    lcd.print("CM= ");
    lcd.print(cm);
  }
  else{
    lcd.print("     empty!     ");
    lcd.setCursor(0,1);
    lcd.print(" junmin is King ");
  }
  sendThingspeak();
  // sendAllData();
  delay(5000);
 
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

  Serial.println("Connected to wifi");
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
  currentTime =  millis() - time;
  if(currentTime > 1000){
    digitalWrite(led,HIGH);
    Serial.print("time: ");
    Serial.println(time);
    Serial.print("millis: ");
    Serial.println(millis());
    Serial.print("currentTime: ");
    Serial.println(currentTime);
    Serial.print("millis()-time: ");
    Serial.println(millis()-time);
    delay(100);

    if(digitalRead(btn) == HIGH){
      time=millis();
      Serial.println("=============================");
      Serial.print("time: ");
      Serial.println(time);
      Serial.print("currentTime: ");
      Serial.println(currentTime);
      Serial.print("millis()-time: ");
      Serial.println(millis()-time);
      Serial.println("=============================");
      delay(100);
    }
  }
  else
  { 
    digitalWrite(led, LOW);
  } 

}


void sensorDistance(){
  distance = analogRead(ir);    //적외선센서 아날로그값 저장
  float volts = (float)distance * VOLTS_PER_UNIT;   //저장한 값을 volt 단위로 변환
  cm = 60.495 * pow(volts,-1.1904);   //cm단위로 변환

   delay(100);
}

void sendThingspeak(){
  if(client.connect(server, 80)){
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(cm);
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
    Serial.print(cm);

  }
  client.stop();
  Serial.println("Waiting…");
}

void sendAllData(){
  if (client.connect(server, 80)) {   // Http 요청 헤더
    Serial.println("Connected...");
    client.print("GET /iot.php?"); //읽을 PHP 파일
    client.print("cm= ");
    client.print(cm);
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
    Serial.println(cm);
    }
    else{
      Serial.println("Connection failed");  
    }
}


