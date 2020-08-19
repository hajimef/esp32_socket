#include <WiFi.h>

static const char *ssid = "ESP32Server";
static const char *pass = "password";
static const int led_pin = 2;

IPAddress ip(192, 168, 11, 1);
WiFiServer server(12345);
 
void setup() {
  Serial.begin(115200);
  // ESP32をアクセスポイントとして動作させる
  WiFi.softAP(ssid, pass);
  WiFi.softAPConfig(ip, WiFi.gatewayIP(), WiFi.subnetMask());
  server.begin();
  // LEDの初期化
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.println("Server started");
}

void loop() {
  // クライアントからの通信を待つ
  WiFiClient client = server.available();
  if (client) {
    // 通信を始める
    Serial.println("New Client.");
    String line = "";
    // データがある間受信する
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read();
        // LFを受信したら受信を終える
        if (c == '\n')
          break;
        line.concat(c);
        Serial.print(c);
      }
    }
    // 受信した文字列をシリアルモニタに出力する
    Serial.print("\nReceived: ");
    Serial.println(line);
    // 受信した文字列が「On」なら
    // LEDを点灯して、クライアントに「On OK」と送信する
    if (line.equals("On")) {
      digitalWrite(led_pin, HIGH);
      client.print("On OK");
    }
    // 受信した文字列が「Off」なら
    // LEDを消灯して、クライアントに「Off OK」と送信する
    else if (line.equals("Off")) {
      digitalWrite(led_pin, LOW);
      client.print("Off OK");
    }
    client.print('\n');
    // 通信を終える
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
