#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
void lcd_disp(String msg, const int col=0, const int raw=0);

// 光感
BH1750 lightMeter;
uint16_t lux = 0;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SWSerial.begin(115200);
  lcd_init();
  dht.begin();
  bmp.begin();
  lightMeter.begin();
  lcd.clear();
  PM_init();
  lcd_disp("Hello, world");
  delay(3000);
}

void loop() {
  // 显示ＤＨＴ
  delay(3000);
  
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
  if(voMeasured > 36.455){
    Air = (float(voMeasured/1024)-0.0356)*120000*0.035;
  }
  delay(3000);
  lcd.clear();
  // 显示PM 光强
  lcd_disp((String)("PM2.5: ") + Air);
  lcd_disp((String)("Light: ") + lux + " lx", 0, 1);

  // 获取气压　海拔 温度
  pre = bmp.readPressure();
  alt = bmp.readAltitude();
  t2 = bmp.readTemperature();
  delay(3000);
  lcd.clear();
  // 显示气压　海拔
  lcd_disp((String)("Alt: ") + alt +" m");
  lcd_disp((String)("Pre: ") + pre + " Pa", 0, 1);

  
  // esp 发送实时数据
  esp_loop();
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

