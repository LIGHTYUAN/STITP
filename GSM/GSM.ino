#include <AltSoftSerial.h>
#define RST 13
// GSM软串口
AltSoftSerial GSMSerial;

void setup() {
  Serial.begin(9600);
  GSMSerial.begin(115200);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  isStartedA6();
  // delay(15000);
}

void loop() {
  if(GSMSerial.available()){
    String result = GSMreadLine();
    Serial.print(result);
  }
    
  if(Serial.available()){
    String str = SreadLine();
    if(str == "<"){
        sendMsg();
    }else
     GSMSerial.print(str);
  }
}

// 是否成功发送信息
bool isSuccess()
{
  String result = "";
  while(GSMSerial.available()){
    result = GSMreadLine();
    if(result.endsWith("OK\r\n"))
      return true;
    if(result.startsWith("\r\n\r\n+CME ERROR:"))
      break;
    if(result != "")
      Serial.print(result);
    result = "";
  }
  return false;
}

// 是否准备好
bool isReady()
{
  while(GSMSerial.available()){
    String result = GSMreadLine();
    if(result.endsWith("OK\r\n"))
      return true;
  }
  return false;
}




// 发送信息
void sendMsg()
{
  while(true){
    GSMSerial.print("AT\r\n");
    delay(200);
    if(isReady()){
      Serial.println("Ready");
      break;
    }
    else
      Serial.println("Not Ready");
    delay(1000);
  }
  delay(200);
  GSMSerial.print("AT+CMGS=\"13218992786\"\r");
  delay(200);
  // Serial.print(GSMreadLine());
  // delay(200);
  GSMSerial.print("Fire!");
  delay(200);
  GSMSerial.print("\x1a");
  // Serial.print(GSMreadLine());
  // delay(200);
  // Serial.print(GSMreadLine());
}



// 检测A6是否启动
void isStartedA6()
{
  String msg="";
  while (true){
    msg = GSMreadLine();
    if(msg.endsWith("+CREG: 1\r\n")){
      Serial.println("Started");
       break;     
    }
  }
}


// 执行AT指令
void execCmd(String cmd)
{
   cmd += "\r";
   String sub = cmd.substring(3, 7);
   if(sub == "CMGS"){
    GSMSerial.print(cmd);
    delay(500);
    GSMSerial.print("Hello, world.");
    GSMSerial.print("\x1a");
   }
   else
    GSMSerial.print(cmd);
   delay(1000);
}

// A6 重启
void rstA6()
{
     digitalWrite(RST, HIGH);
     delay(1000);
     digitalWrite(RST, LOW);
}


// GSM 串口读取
String GSMreadLine()
{
  String str=":";
  while(GSMSerial.available())
  {
    char ch = GSMSerial.read();
    str += char(ch);
    delay(2);
  }
  return str;
}

// Serial 串口读取
String SreadLine()
{
  String str;
  while(Serial.available())
  {
    char ch = Serial.read();
    str += char(ch);
  }
  return str;
}

