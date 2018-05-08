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

#define RST 13　// GSM 重启口
