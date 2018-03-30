#include <ESP8266WiFi.h>

#define led 2
const char *ssid  = "DKY_FATHER";
const char *password = "139256478";
const char *host = "192.168.2.104";

WiFiClient client;
const int tcpPort = 1234;

void setup()
{
  // ESP 启动串口通信
  Serial.begin(115200);
  
  delay(10);
  
  // 连接ＷＩＦＩ
  WiFi.begin(ssid, password); 
  
  
  // 检测是否成功连接ＷＩＦＩ
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
  }
}

void loop()
{
  // TCP 数据流是否建立
  while(!client.connected()) 
  {
    if(!client.connect(host, tcpPort)){
      delay(500);
    }
  }

  // 透传数据
  while(client.available()) // TCP流连接是否可以获取数据
  {
    // TCP逐字节读取
    uint8_t c = client.read();
    Serial.write(c);
  }

  if(Serial.available()){
    size_t counti = Serial.available();
    uint8_t sbuf[counti];
    Serial.readBytes(sbuf, counti);
    client.write(sbuf, counti);
  }
}

