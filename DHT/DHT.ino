#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2 // 数据接受端口
#define DHTTYPE DHT22 // 定义传感器型号

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  dht_init();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // put your main code here, to run repeatedly:

}

void dht_init()
{
  dht.begin();
}

