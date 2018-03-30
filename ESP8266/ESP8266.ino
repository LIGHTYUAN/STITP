#include <ESP8266WiFi.h>

#define led 2
const char *ssid  = "DKY_FATHER";
const char *password = "139256478";
const char *host = "192.168.2.104";

WiFiClient client;
const int tcpPort = 1234;

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // 连接ＷＩＦＩ

  // 检测是否成功连接ＷＩＦＩ
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  // ＷＩＦ连接成功后的提示
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  while(!client.connected()) // TCP 数据流是否建立
  {
    if(!client.connect(host, tcpPort)){
      Serial.println("connecting....");
      delay(500);
    }else{
      Serial.println("TCP connected.");
    }
  }
  
  while(client.available()) // 是否可以获取数据
  {
    char val = client.read();
    if(val == 'a'){
       digitalWrite(led, LOW);
       Serial.println("LED is OFF");
    }
    else if(val == 'b')
    {
      digitalWrite(led, HIGH);
      Serial.println("LED is ON");
    }
  }
}

