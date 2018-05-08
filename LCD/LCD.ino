#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <AltSoftSerial.h>
#define NOP do{__asm__ __volatile__("nop");}while(0)
// G203
int buttonpin = 2;
int LED = LED_BUILTIN;
int val;

// lcd 显示函数声明
void lcd_disp(String msg, const int col=0, const int raw=0);

//GSM
// GSM软串口
AltSoftSerial GSMSerial;

// 光感
BH1750 lightMeter;
uint16_t lux = 0;

// DHT
#define DHTPIN 7 // 数据接受端口
#define DHTTYPE DHT22 // 定义传感器型号

// 温度　湿度
DHT dht(DHTPIN, DHTTYPE);
float h = 0.0;
float t1 = 0.0;
String olddata = "";

// 大气压
Adafruit_BMP085 bmp;
float pre = 0.0;
float alt = 0.0;
float t2 = 0.0;

// PM2.5
// PM端口设置
int measurePin = A0;            
int ledPower = 12;              

// PM检测参数设置
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

// PM结果处理
float voMeasured = 0.0;
float Air = 0.0;

// LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// ESP
SoftwareSerial SWSerial(5, 6);

// timer1
int timer1_counter;  

#define RST 13　// GSM 重启端口
void setup() {
  lcd_init(); // lcd 初始化
  lcd.clear();
  lcd_disp("Hello, world");
  lcd_disp("INITING...", 0, 1);

  Serial.begin(9600); // 测试输出串口初始化 
  SWSerial.begin(115200); // Esp 串口初始化
  GSMSerial.begin(115200); // GSM 串口初始化

  dht.begin(); // dht 初始化
  bmp.begin(); // bmp 初始化
  lightMeter.begin(); // 光强传感器初始化
  PM_init(); // PM2.5 初始化
   // pinMode(13, OUTPUT);
  //digitalWrite(13, LOW);
  
  // G203初始化
  pinMode(LED, OUTPUT);
  pinMode(buttonpin, INPUT);

  isStartedA6();  // GSM 初始化

/*
  // timer1初始化
  noInterrupts();  
  TCCR1A = 0;  
  TCCR1B = 0;  
  //timer1_counter = 65536-(62500*0.5);  //0.5s 定时器中断  
  timer1_counter = 34286;  //预加载timer1 65536-16Mhz/256/2Hz  
  TCNT1 = timer1_counter;  
  TCCR1B |= (1 << CS12);  
  TIMSK1 |= (1<< TOIE1);  
  interrupts();
*/
}

void loop() {    
  // 显示ＤＨＴ
  delay(1000);
  detect(); 
  // 获取温度　湿度
  h = dht.readHumidity();
  t1 = dht.readTemperature();
  lcd.clear();
  // 显示温度　湿度
  lcd_disp((String)("Humi: ") + h +"%", 0, 0);
  lcd_disp((String)("Temp: ") + t1 +" *C", 0, 1);

  // 获取光强
  lux = lightMeter.readLightLevel();
  // 获取PM2.5
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);
  delay(1000);
    detect(); 
  if(voMeasured > 36.455){
    Air = (float(voMeasured/1024)-0.0356)*120000*0.035;
  }
  delay(1000);
    detect();
  lcd.clear();
  // 显示PM 光强
  lcd_disp((String)("PM2.5: ") + Air);
  lcd_disp((String)("Light: ") + lux + " lx", 0, 1);

  // 获取气压　海拔 温度
  pre = bmp.readPressure();
  alt = bmp.readAltitude();
  t2 = bmp.readTemperature();
  delay(1000);
    detect();
  delay(1000);
    detect();
  lcd.clear();
  // 显示气压　海拔
  lcd_disp((String)("Alt: ") + alt +" m");
  lcd_disp((String)("Pre: ") + pre + " Pa", 0, 1);

  // esp 发送实时数据
  esp_loop();
    detect(); 
}

// 初始化
void lcd_init()
{
  lcd.init();
  lcd.backlight();
}

// 显示一行
void lcd_disp(String msg, const int col=0, const int raw=0)
{
  lcd.setCursor(col, raw);
  lcd.print(msg);
}



// PM初始化
void PM_init()
{
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  pinMode(measurePin, INPUT);
}

// PM循环函数
void PM_loop()
{
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);
  delay(1000);
  if(voMeasured > 36.455){
    Air = (float(voMeasured/1024)-0.0356)*120000*0.035;
    Serial.println(Air);
  }
}

void bmp_init()
{
  if (!bmp.begin()) {
  while (1) {}
  }
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

// ESP
// 温度 t1 t2
// 湿度 h
// PM2.5 Air
// 光强 lux
// 海拔 alt
// 气压 pre
void esp_loop()
{
  // 构造报文
  String data = "";
  data = (String)"temperature=" + t1 + "&" + "humidity=" + h; // 温度 湿度
  data += (String)"&PM2_5=" + Air + "&" + "light=" + lux; // PM2.5 光强
  data += (String)"&altitude=" + alt + "&" + "pressure=" + pre; // 海拔　气压
  SWSerial.print(data);

  delay(1000);

  if(SWSerial.available()){
    Serial.println(SWreadLine());
  }
}

// 完成GSM模块的启动
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

void Blink()
{
  for(int i=0; i<5; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);   
  }
}

void detect()
{
  val = digitalRead(buttonpin);
  if(val == LOW){   
    // Blink();
    delay(3000);
    sendMsg();
  }else{
    digitalWrite(LED, LOW);
  }
}

/*
ISR(TIMER1_OVF_vect) {  
  // TCNT1 = timer1_counter; 
  // sendMsg();
  detect();
  TCNT1 = timer1_counter; 
}
*/

void delay2(int ms)
{
  for(int i=0; i<ms; i++){
    for(unsigned long  j = 0; j<1085; j++) NOP;
  }
}

