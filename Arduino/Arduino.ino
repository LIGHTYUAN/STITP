#include "Arduino1.h"
#include "Arduino2.h"
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

