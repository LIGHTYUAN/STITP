#include <ESP8266WiFi.h>
#define led 2
// const char *ssid  = "DKY_FATHER";
const char *ssid = "LIGHTYUAN";
const char *password = "139256478";
// const char *host = "192.168.2.104";
// const char *host = "172.96.226.237"; 
const char *host = "192.168.43.189";

WiFiClient client;
// const int tcpPort = 1234;
const int tcpPort = 80;

void setup()
{
  // ESP 启动串口通信
  Serial.begin(115200);
  
  delay(10);
  
  // 连接ＷＩＦＩ
  WiFi.begin(ssid, password); 
  
  
  // 检测是否成功连接ＷＩＦＩ
  while(WiFi.status() != WL_CONNECTED){
    //Serial.println("WIFI Connecting...");
    delay(1000);
  }
  //Serial.print("Success to connect WIFI: ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
}

void loop()
{
  // TCP 数据流是否建立
  while(!client.connected())
  {
    if(!client.connect(host, tcpPort)){
      //Serial.println("TCP Connecting...");
      delay(500);
    }else{
      //Serial.println("TCP connected.");
      Serial.println("");
      delay(20);
      Serial.print("OK");
    }
  }
  // 透传数据 Server - > Arduino
  if(client.available()) // TCP流连接是否可以获取数据
  {
    // TCP逐字节读取
    String recv = CreadLine();
    String sc = recv.substring(9,12);
    if(sc != "200"){
      Serial.println(sc);
      Serial.println(recv);
    }
    //Serial.print(recv);
    //uint8_t c = client.read();
    //Serial.write(c);
  }

  
  /*
  if(Serial.available()){
    size_t counti = Serial.available();
    uint8_t sbuf[counti];
    Serial.readBytes(sbuf, counti);
    client.write(sbuf, counti);
  }
  */
  // Arduino -> Server
  if(Serial.available()){
    String recv = SreadLine();

    String data = recv;  
    int length = data.length();  
        
    String postRequest =
          (String)("POST ") + "/POST.php HTTP/1.1\r\n" +  
          "Host: " + host + ":" + tcpPort + "\r\n" +               
          "Connection: keep-alive\r\n" +  
          "Content-Length: " + length + "\r\n" +  
          "Content-Type: application/x-www-form-urlencoded\r\n\r\n" +  
          data;  
    //Serial.println(postRequest); 
    
    //client.print(recv);
    client.print(postRequest);
  }
}

// 从Serial中读取一条信息
String SreadLine()
{
	String str = "";
	while(Serial.available()){
		char temp = Serial.read();		
		str += char(temp);
		delay(2);
	}
	return str;
}

// 从client中读取一条信息
String CreadLine()
{
  String str = "";
  while(client.available()){
    char temp = client.read();
    str += char(temp);
    delay(2);
  }
  return str;
}
