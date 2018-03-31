#include <SoftwareSerial.h>

// LED 控制口Pin10
int ledPin = 10;
// 声明软串口rx=6, tx=7
SoftwareSerial SWSerial(6, 7);

void setup()
{
  
  // 软串口初始化
  SWSerial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  delay(10);
  // Serial 串口初始化
  Serial.begin(9600);
  delay(10);
  // Pin10初始化LOW
  digitalWrite(ledPin, LOW);
  //Serial.println("LED init:");
  Serial.println("LED OFF");
}

void loop()
{
  //　软串口读取
  if(SWSerial.available()){
  	String recv = SWreadLine();
    if(recv == "a"){
       digitalWrite(ledPin, HIGH);
	   Serial.println("LED ON");
	  }
    else if(recv == "b"){
       digitalWrite(ledPin, LOW);
	   Serial.println("LED OFF");
	  }
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

// 从Serial读入一行信息或一条信息, 不包含换行符'\n' 和 回车符'\r'
String SreadLine()
{
  String str;
  while(Serial.available())
  {
    char ch = Serial.read();
    if(ch != '\n'){
      str += char(ch);
      delay(2);
    }
    else break;
  }
  return str;
}

// 从软串口读入一行信息或一条信息, 不包含换行符'\n' 和 回车符'\r'
String SWreadLine()
{
  String str;
  while(SWSerial.available())
  {
    char ch = SWSerial.read();
    if(ch != '\n'){
      str += char(ch);
      delay(2);
    }
    else break;
  }
  return str;
}