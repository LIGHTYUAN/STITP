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
