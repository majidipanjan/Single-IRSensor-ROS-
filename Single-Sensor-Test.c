#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/Bool.h>

int IR1 = D0;
const char* ssid = "ssid";
const char* password = "password";

IPAddress server(192, 168, 0, 190); // ip of your ROS server
IPAddress ip_address;
int status = WL_IDLE_STATUS;

WiFiClient client;


class WiFiHardware {
  public:
  WiFiHardware() {};
  void init() {
    client.connect(server, 11411);
  }
  int read() {
    return client.read();    
  }
  void write(uint8_t* data, int length) {
    for(int i=0; i<length; i++)
      client.write(data[i]);
  }
  unsigned long time() {
     return millis(); 
  }
};


std_msgs::Bool infrared;
ros::Publisher IRsensors("IRsensors", &infrared);
ros::NodeHandle_<WiFiHardware> nh;

void setupWiFi()
{
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  Serial.print("Ready! Use ");
  Serial.print(WiFi.localIP());
  Serial.println(" to access client");
}

void setup() {

  pinMode(IR1, INPUT);
  Serial.begin(115200);
  setupWiFi();
  delay(2000);
  nh.initNode();
  nh.advertise(IRsensors);
}

void loop() {
  int s1 = digitalRead(IR1);
  infrared.data = s1;
  IRsensors.publish(&infrared);
  nh.spinOnce();
  delay(500);
}
