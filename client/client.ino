#include <WiFi.h>

static const char *ssid = "ESP32Server";
static const char *pass = "password";

IPAddress ip(192, 168, 11, 2);
IPAddress host(192, 168, 11, 12);
const int sw_pin = 2;
const int port = 12345;

void setup() {
  Serial.begin(115200);
  // サーバーに接続する
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // スイッチを初期化する
  pinMode(sw_pin, INPUT_PULLUP);
}

void loop() {
  WiFiClient client;
  static bool status = false;
  static int sw_stat = HIGH;

  // スイッチが押された場合
  if (digitalRead(sw_pin) == LOW && sw_stat == HIGH) {
    sw_stat = LOW;
    // サーバーに接続する
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    }
    // LEDの点灯／消灯状態を入れ替える
    status = !status;
    // 点灯させる場合はサーバーに「On」の文字列を送信する
    if (status) {
      Serial.println("Send On");
      client.print("On\n");
    }
    // 消灯させる場合はサーバーに「Off」の文字列を送信する
    else {
      Serial.println("Send Off");
      client.print("Off\n");
    }
    //client.print('\n');
    // サーバーからのレスポンスを待つ
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Timeout!");
        client.stop();
        return;
      }
    }
    // サーバーからのレスポンスを受信する
    while(client.available()) {
      String line = client.readStringUntil('\n');
      Serial.print(line);
    }
    Serial.print('\n');
    delay(200);
  }
  else {
    sw_stat = HIGH;
  }
}
