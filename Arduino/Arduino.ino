#include <DHT.h>
#include <DHT_U.h>

#include <SoftwareSerial.h>

#define DHTPIN 2
#define DHTTYPE DHT22

// LED 控制口Pin10
// int ledPin = 10;
// 声明软串口rx=6, tx=7
SoftwareSerial SWSerial(6, 7);
// 声明DHT对象
DHT dht(DHTPIN, DHTTYPE);
// olddata
String olddata = "";

void setup()
{
  
  // 软串口初始化
  SWSerial.begin(115200);
  delay(10);
  // pinMode(ledPin, OUTPUT);
  delay(10);
  // Serial 串口初始化
  Serial.begin(9600);
  delay(10);
  // Pin10初始化LOW
  //digitalWrite(ledPin, LOW);
  //Serial.println("LED init:");
  //Serial.println("LED OFF");

  // dht开始运行
  dht.begin();
}

void loop()
{
  // 测量前等待6s
  for(int i = 3; i > 0; i--)
    delay(2000);
  // 湿度
  float h = dht.readHumidity();
  // 温度
  float t = dht.readTemperature();
  String data = "";
  data = (String)"temperature=" + t + "&" + "humidity=" + h;
  if(data != olddata){
    SWSerial.print(data);
    Serial.println(data);
    Serial.println(olddata);
    olddata = data;
  }

  //　软串口读取
  if(SWSerial.available()){
    Serial.println(SWreadLine());
        /*
  	String recv = SWreadLine();
    Serial.print(recv + " : ");

    if(recv == "a"){
       digitalWrite(ledPin, HIGH);
	     Serial.println("LED ON");
	  }
    else if(recv == "b"){
       digitalWrite(ledPin, LOW);
	     Serial.println("LED OFF");
	  }
    else{
       Serial.println("Nothing");
        
    }
   */
  /*
	// 接受第一个字符
   // Serial.println("your command is:");
    size_t counti = SWSerial.available();
    char ch = SWSerial.read();
    counti--;
    //Serial.write(ch);
    
	// 丢弃其余字符
    while(counti > 0){
      SWSerial.read();
      //Serial.write(SWSerial.read());
      counti--;
    }
    //Serial.write('\n');

    
	// 判断字符　控制LED灯
    if(ch == 'a'){
       digitalWrite(ledPin, HIGH);
	   Serial.println("LED ON");
	  }
    else if(ch == 'b'){
       digitalWrite(ledPin, LOW);
	   Serial.println("LED OFF");
	  }
	  */
  }

  if(Serial.available())
    SWSerial.print(SreadLine());
}

// 从Serial读入一条信息
String SreadLine()
{
  String str;
  while(Serial.available())
  {
    char ch = Serial.read();
    str += char(ch);
    delay(2);
  }
  return str;
}

// 从软串口读入一条信息
String SWreadLine()
{
  String str;
  while(SWSerial.available())
  {
    char ch = SWSerial.read();
    str += char(ch);
    delay(2);
  }
  return str;
}
