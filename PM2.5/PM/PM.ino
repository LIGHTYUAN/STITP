
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

void setup(){
  PM_init();
}

void loop(){
  PM_loop();
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

