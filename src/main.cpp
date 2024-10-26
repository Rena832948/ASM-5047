

// // 定义 DHT 传感器类型和连接的 GPIO 引脚
// #define DHTPIN 15        // 将 DHT22 数据引脚连接到 GPIO4
// #define DHTTYPE DHT22   // DHT 22 (AM2302) 型号

// // 初始化 DHT 传感器
// DHT dht(DHTPIN, DHTTYPE);

// void setup() {
//   // 初始化串口，用于调试输出
//   Serial.begin(115200);
//   Serial.println(F("TEST"));

//   // 启动 DHT 传感器
//   dht.begin();
// }

// void loop() {
//   // 等待 2 秒再读取下一次数据（DHT22 传感器的更新速率较慢）
//   delay(2000);

//   // 读取湿度值
//   float humidity = dht.readHumidity();
//   // 读取温度值（摄氏度）
//   float temperature = dht.readTemperature();
//   // 读取温度值（华氏度，如果需要的话）
//   // float temperatureF = dht.readTemperature(true);

//   // 检查读取是否成功
//   if (isnan(humidity) || isnan(temperature)) {
//     Serial.println(F("FAIL"));
//     return;
//   }

//   // 打印湿度和温度到串口
//   Serial.print(F("t: "));
//   Serial.print(humidity);
//   Serial.print(F("%  t: "));
//   Serial.print(temperature);
//   Serial.println(F("°C"));
// }

// // 定义 FSR 连接的 ADC 引脚（使用 GPIO34 作为模拟输入引脚）
// const int fsrPin = 34;  // ADC 引脚

// void setup() {
//   // 初始化串口监视器
//   Serial.begin(115200);
// }

// void loop() {
//   // 读取 ADC 值，ESP32 的 ADC 是 12 位分辨率，返回值范围为 0 到 4095
//   int fsrValue = analogRead(fsrPin);

//   // 将 ADC 值转换为电压值 (ESP32 的输入电压范围是 0 到 3.3V)
//   float voltage = fsrValue * (3.3 / 4095.0);

//   // 输出 FSR 传感器的原始 ADC 值和电压值到串口监视器
//   Serial.print("FSR ADC Value: ");
//   Serial.print(fsrValue);
//   Serial.print(" | Voltage: ");
//   Serial.print(voltage);
//   Serial.println(" V");

//   // 延迟100毫秒，避免频繁读取
//   delay(100);
// }

// #include "HardwareSerial.h"

// // 使用 UART2 与 DFPlayer Pro 通信
// HardwareSerial mySerial(2);  // ESP32 的 UART2 (RX2: GPIO16, TX2: GPIO17)
// DFRobotDFPlayerMini myDFPlayer;

// void setup() {
//   // 初始化串口，用于调试输出
//   Serial.begin(115200);
//   Serial.println(F("Initializing DFPlayer..."));

//   // 初始化 UART2 (9600 波特率)
//   mySerial.begin(9600, SERIAL_8N1, 25, 26); // RX2 = GPIO16, TX2 = GPIO17

//   // 初始化 DFPlayer Pro
//   if (!myDFPlayer.begin(mySerial)) {
//     Serial.println(F("DFPlayer Pro 初始化失败！"));
//     while (true);  // 停止程序
//   }

//   Serial.println(F("DFPlayer Pro 初始化成功！"));
  
//   // 设置音量（范围 0 - 30）
//   myDFPlayer.volume(20);  // 你可以调整音量大小
  
//   // 播放第一个音频文件（0001.mp3）
//   myDFPlayer.play(1);
// }

// void loop() {
//   // 在这里可以添加更多功能或控制代码
// }

// #include "HardwareSerial.h"

// HardwareSerial mySerial(2);  // 使用 UART2
// DFRobotDFPlayerMini myDFPlayer;

// void setup() {
//   Serial.begin(115200);  // 初始化调试串口
//   Serial.println("Initializing UART2...");
  
//   // 初始化UART2，波特率9600，TX: GPIO26，RX: GPIO25
//   mySerial.begin(9600, SERIAL_8N1, 25, 26);

//     // 初始化 DFPlayer Pro
//   if (!myDFPlayer.begin(mySerial)) {
//     Serial.println(F("DFPlayer Pro 初始化失败！"));
//     while (true);  // 停止程序
//   }
  
//   if (mySerial) {
//     Serial.println("UART2 initialized successfully on GPIO25(RX) and GPIO26(TX)");
//   } else {
//     Serial.println("Failed to initialize UART2");
//   }
// }

// void loop() {
//   // 向UART2发送数据
//   mySerial.println("Hello from UART2 on GPIO25(RX) and GPIO26(TX)");
  
//   // 读取UART2接收到的数据
//   if (mySerial.available()) {
//     String received = mySerial.readString();
//     Serial.print("Received: ");
//     Serial.println(received);
//     myDFPlayer.play(1);
//   }
// }

// #define LDR_PIN 4       // 光感电阻连接到 GPIO 34（ADC 引脚）
// #define LED_PIN 16        // LED 灯连接到 GPIO 5（数字输出引脚）
// #define LIGHT_THRESHOLD 1000 // 光强阈值，根据实际情况调整（0 - 4095）

// void setup() {
//   // 初始化串口通信
//   Serial.begin(115200);
  
//   // 设置 LED 引脚为输出模式
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//   // 读取光感电阻的模拟值
//   int ldrValue = analogRead(LDR_PIN);
  
//   // 将读取到的值打印到串口监视器
//   Serial.print("光感电阻的值: ");
//   Serial.println(ldrValue);

//   // 根据读取的值控制 LED 灯泡
//   if (ldrValue < LIGHT_THRESHOLD) {
//     // 当环境光较弱时，点亮 LED
//     digitalWrite(LED_PIN, HIGH);
//     Serial.println("环境光弱 -> 点亮 LED 灯泡");
//   } else {
//     // 当环境光较强时，熄灭 LED
//     digitalWrite(LED_PIN, LOW);
//     Serial.println("环境光强 -> 熄灭 LED 灯泡");
//   }

//   // 延时 500 毫秒，便于查看输出
//   delay(500);
// }

#include "DHT.h"
#include <Arduino.h>
#include <Wire.h>
#include <DFRobotDFPlayerMini.h>
#include <DHT.h>
#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <HTTPClient.h>  // 引入 HTTPClient 库以便进行 API 请求
#define LIGHT_SENSOR_PIN 34
#define PRESSURE_SENSOR_PIN 32
#define HUMIDITY_SENSOR_PIN 4
#define TEMPERATURE_SENSOR_PIN 4
#define SPEAKER_TX_PIN 17
#define SPEAKER_RX_PIN 16


#define RX1 16  // 你选择的 RX 引脚（可以是任意可用的 GPIO 引脚）
#define TX1 17  // 你选择的 TX 引脚（可以是任意可用的 GPIO 引脚）
#define LIGHT_SENSOR_PIN 39  //ok shaoyigedengpao 34ni
#define PRESSURE_SENSOR_PIN 34 //ok
#define HUMIDITY_SENSOR_PIN 15 //ok
#define TEMPERATURE_SENSOR_PIN 15 //ok
#define DHTTYPE DHT22
#define LED_PIN 13

DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);

void gameLightTransmission();
// void gameWeightDetection();
// void gameHumidityDetection();
// void gameTemperatureDetection();
// int getAISelectedGame();

// HardwareSerial mySerial(2);  // 使用 UART1
DFRobot_DF1201S DF1201S;
// 无用软连接
// SoftwareSerial mySerial(16, 17);  // RX = GPIO16, TX = GPIO17
// SoftwareSerial DF1201SSerial(RX, TX);  //RX  TX

HardwareSerial mySerial(2);  // 使用 UART1

// 设置 WiFi 凭据和设备名称
const char* ssid = "dick";  // TODO: 替换为您的 Wi-Fi SSID
const char* password = "dick8888";  // TODO: 替换为您的 Wi-Fi 密码
const char* device_name = "ESP32_Sensor";  // TODO: 替换为设备名称
WiFiServer wifi_server(80);  // 初始化 WiFi 服务器

// 设置 AI API 密钥
const char* api_key = "AIzaSyB91H1x6yd80Oo604V2gA7gkX5J5uGGiks";  // TODO: 替换为您的实际 API 密钥
const char* api_endpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=AIzaSyB91H1x6yd80Oo604V2gA7gkX5J5uGGiks";  // TODO: 替换为实际的 API 端点
int gameSelection = 1;
// 生成 AI 响应的函数（使用 API）
// 生成 AI 响应的函数（使用 API）
String generateAIResponse(String input) {
    HTTPClient http;
  http.begin(api_endpoint);
  http.addHeader("Content-Type", "application/json");

  // 创建请求体
  String requestBody = "{\"contents\":[{\"parts\":[{\"text\":\"" + input + "\"}]}]}";
  int httpCode = http.POST(requestBody);

  if (httpCode != 200) {
      Serial.println("API 请求失败");
      Serial.print("HTTP 状态代码: ");
      Serial.println(httpCode);
      Serial.println("服务器返回错误: ");
      Serial.println(http.getString());
      http.end();
  }

  // 解析 API 响应
  String response = http.getString();
  Serial.println("API 返回内容: ");
  Serial.println(response);  // 打印完整的响应内容
  http.end();

  StaticJsonDocument<1024> jsonDoc;
  deserializeJson(jsonDoc, response);

  // 提取正确的字段路径
  String advice = jsonDoc["candidates"][0]["content"]["parts"][0]["text"].as<String>();
  return advice;
}

// 将文本转换为 MP3 文件并保存在本地（模拟功能）
void textToSpeech(String text, const char* filename) {
    // 模拟将文本转换为语音并存储为 MP3 文件
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (file) {
        file.print("模拟的MP3数据: " + text);
        Serial.println("MP3 文件已保存: " + String(filename));
        file.close();
    } else {
        Serial.println("无法打开文件进行写入");
    }
}

void connectWifi(){
// 连接到 Wi-Fi 网络
    WiFi.setHostname(device_name);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("正在连接到 Wi-Fi...");
    }
    Serial.println("已连接到 Wi-Fi");
    Serial.print("IP 地址: ");
    Serial.println(WiFi.localIP());

    // 启动 WiFi 服务器
    wifi_server.begin();
}

void sendAI(String userInput){
  if (WiFi.status() == WL_CONNECTED) {
        // 生成 AI 响应
        String aiResponse = generateAIResponse(userInput);
        Serial.println("AI 响应: " + aiResponse);

        // 将 AI 响应保存为 MP3 文件
        textToSpeech(aiResponse, "/feedback.mp3");
    }

    // 检查是否有客户端连接到 WiFi 服务器
    WiFiClient client = wifi_server.available();
    if (client && client.connected() && client.available()) {
        // 读取客户端请求
        String request = client.readStringUntil('\r');
        client.flush();

        // 发送请求到 Gemini API 并获取响应
        String geminiResponse = generateAIResponse(request);
        Serial.println("从客户端接收到的请求: " + request);
        Serial.println("AI 响应: " + geminiResponse);

        // 将响应发送回客户端
        client.print(geminiResponse);
        client.stop();
    }

}

// TODO 有效代码
void setup() {
  Serial.begin(115200);  // 启动串口监视器
  SPIFFS.begin(true);    // 初始化 SPIFFS 文件系统
  // 设置 LED 引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  connectWifi();
  //音频信号连接
  mySerial.begin(115200, SERIAL_8N1, 16, 17);  // GPIO16 RX, GPIO17 TX
  dht.begin();

    // 尝试初始化 DF1201S
  Serial.println("Serial is working.");

  // 初始化 mySerial（UART1 或 UART2）

  delay(1000);  // 确保串口初始化完成
  mySerial.println("AT");  // 发送 AT 命令
  delay(500);  // 等待响应

  if (mySerial.available()) {
    Serial.println("mySerial is working.");
  } else {
    Serial.println("No response from mySerial.");
  }
  delay(2000);  // 增加延迟，等待模块完成初始化

  // 尝试初始化 DF1201S
  while(!DF1201S.begin(mySerial)){
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
    if (mySerial.available()) {
      Serial.println("mySerial is working.");
    } else {
      Serial.println("No response from mySerial.");
    }
    delay(2000);  // 增加延迟，等待模块完成初始化
    } 

  // Game Selection (for simplicity, select via Serial input for testing)
  Serial.println("Select a game (1-4):");
  while (gameSelection < 1 || gameSelection > 4) {
    while (!Serial.available());
    gameSelection = Serial.parseInt();
    if (gameSelection < 1 || gameSelection > 4) {
      Serial.println("Invalid selection. Select a game (1-4):");
    }
  }

  // Play the audio instruction based on game selection
  DF1201S.playFileNum(gameSelection); // Play corresponding audio file for game rules

    
  // // mySerial.begin(9600);  // 设置波特率为 9600，确保与设备一致

  // delay(1000);
  // Serial.println("Serial is working.");

  // // 初始化 mySerial（UART1 或 UART2）

  // delay(1000);  // 确保串口初始化完成
  // mySerial.println("AT");  // 发送 AT 命令
  // delay(500);  // 等待响应

  // if (mySerial.available()) {
  //   Serial.println("mySerial is working.");
  // } else {
  //   Serial.println("No response from mySerial.");
  // }
  // delay(2000);  // 增加延迟，等待模块完成初始化

  // // 尝试初始化 DF1201S
  // while(!DF1201S.begin(mySerial)){
  //   Serial.println("Init failed, please check the wire connection!");
  //   delay(1000);
  //   if (mySerial.available()) {
  //     Serial.println("mySerial is working.");
  //   } else {
  //     Serial.println("No response from mySerial.");
  //   }
  //   delay(2000);  // 增加延迟，等待模块完成初始化
  //   } 

  // Serial.println("ok!");
  //   delay(1000);
    

  // /*Set volume to 20*/
  // DF1201S.setVol(/*VOL = */20);
  // Serial.print("VOL:");
  // /*Get volume*/
  // Serial.println(DF1201S.getVol());
  // /*Enter music mode*/
  // DF1201S.switchFunction(DF1201S.MUSIC);
  // /*Wait for the end of the prompt tone */
  // delay(2000);
  // /*Set playback mode to "repeat all"*/
  // DF1201S.setPlayMode(DF1201S.ALLCYCLE);
  // Serial.print("PlayMode:");
  // /*Get playback mode*/
  // Serial.println(DF1201S.getPlayMode());
    
}

void loop(){
  // sendAI("请分析当前环境下泥土和叶子的状况,限制在10个字以内");

  switch (gameSelection) {
    case 1:
      gameLightTransmission();
      break;
    case 2:
      //gameWeightDetection();
      break;
    case 3:
      //gameHumidityDetection();
      break;
    case 4:
      //gameTemperatureDetection();
      break;
    default:
      break;
  }
  
}

void gameLightTransmission() {
  Serial.println("Game 1: Light Transmission Test");
  digitalWrite(LED_PIN, HIGH);
  int lightValue;
  while (true) {
    delay(1000);
      digitalWrite(LED_PIN, HIGH);
    Serial.println("while true");
    if (analogRead(PRESSURE_SENSOR_PIN) > 500) { // Assuming pressure > threshold
      delay(3000); // Pressure sustained for 3 seconds
      //lightValue = analogRead(LIGHT_SENSOR_PIN);
      Serial.println(lightValue);
      if (lightValue < 500) {  // Check if light is blocked sufficiently
        // myDFPlayer.play(5);  // Play success audio
        delay(5000);
        break;
      }
    }else{
          Serial.println("LED 已经设置为常亮");
      digitalWrite(LED_PIN, HIGH);

      lightValue = analogRead(LIGHT_SENSOR_PIN);
      Serial.println(lightValue);
    }
    delay(100);
  }
}

void gameWeightDetection() {
  Serial.println("Game 2: Weight Detection");
  int pressureValue;
  while (true) {
    pressureValue = analogRead(PRESSURE_SENSOR_PIN);
    Serial.print("Pressure: ");
    Serial.println(pressureValue);
    if (pressureValue > 800) { // Assuming 800 as target pressure value
      // myDFPlayer.play(6); // Play success audio
      delay(5000);
      break;
    }
    delay(100);
  }
}

void gameHumidityDetection() {
  Serial.println("Game 3: Humidity Detection");
  float humidity;
  while (true) {
    humidity = dht.readHumidity();
    Serial.print("Humidity: ");
    Serial.println(humidity);
    if (humidity >= 60) {  // Assuming 60% as target humidity level
      // myDFPlayer.play(7);  // Play success audio
      delay(5000);
      break;
    }
    delay(2000); // Check every 2 seconds
  }
}










// #define LIGHT_SENSOR_PIN 16  //ok shaoyigedengpao 34ni
// #define PRESSURE_SENSOR_PIN 34 //ok
// #define HUMIDITY_SENSOR_PIN 15 //ok
// #define TEMPERATURE_SENSOR_PIN 15 //ok
// #define SPEAKER_TX_PIN 35 //ok
// #define SPEAKER_RX_PIN 39  //ok
// #define DHTTYPE DHT22

// #if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
// #include <SoftwareSerial.h>
// SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
// #define FPSerial softSerial
// #else
// #define FPSerial Serial1
// #endif

// DFRobotDFPlayerMini myDFPlayer;


// DFRobot_DF1201S DF1201S;

// void setup(void){
//   Serial.begin(115200);
//   DF1201SSerial.begin(115200);
//   DF1201SSerial.println("AT");
//   if (DF1201SSerial.available()) {
//     while (DF1201SSerial.available()) {
//       char c = DF1201SSerial.read();
//       Serial.print("okokok");

//       Serial.print(c);
//       break;
//     }
//   } else {
//     Serial.println("No response from DF1201S");
//   }
//   while(!DF1201S.begin(DF1201SSerial)){
//     Serial.println("Init failed, please check the wire connection!");
//     delay(1000);
//   }
//   /*Set volume to 20*/
//   DF1201S.setVol(/*VOL = */20);
//   Serial.print("VOL:");
//   /*Get volume*/
//   Serial.println(DF1201S.getVol());
//   /*Enter music mode*/
//   DF1201S.switchFunction(DF1201S.MUSIC);
//   /*Wait for the end of the prompt tone */
//   delay(2000);
//   /*Set playback mode to "repeat all"*/
//   DF1201S.setPlayMode(DF1201S.ALLCYCLE);
//   Serial.print("PlayMode:");
//   /*Get playback mode*/
//   Serial.println(DF1201S.getPlayMode());

  //Set baud rate to 115200(Need to power off and restart, power-down save)
  //DF1201S.setBaudRate(115200);
  //Turn on indicator LED (Power-down save)
  //DF1201S.setLED(true);
  //Turn on the prompt tone (Power-down save) 
  //DF1201S.setPrompt(true);
  //Enable amplifier chip 
  //DF1201S.enableAMP();
  //Disable amplifier chip 
  //DF1201S.disableAMP();
// }









// DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);
// void printDetail(uint8_t type, int value);


// int gameSelection = 0;

// void setup()
// {
//     Serial.println(F("DFRobot DFPlayer Mini Demo"));

// // #if (defined ESP32)
//   FPSerial.begin(9600, SERIAL_8N1, 3, 1);//tx 1, rx 3
// // #else
// //   FPSerial.begin(9600);
// // #endif

//   Serial.begin(115200);

//   Serial.println();
//   Serial.println(F("DFRobot DFPlayer Mini Demo"));
//   Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
//   if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
//     Serial.println(F("Unable to begin:"));
//     Serial.println(F("1.Please recheck the connection!"));
//     Serial.println(F("2.Please insert the SD card!"));
//     while(true){
//       delay(0); // Code to compatible with ESP8266 watch dog.
//     }
//   }
//   Serial.println(F("DFPlayer Mini online."));
  
//   myDFPlayer.volume(10);  //Set volume value. From 0 to 30
//   myDFPlayer.play(1);  //Play the first mp3
// }






// void setup() {
//   #if (defined ESP32)
//   FPSerial.begin(9600, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
//   #else
//   FPSerial.begin(9600);
//   #endif
//   Serial.begin(115200);
//   dht.begin();

//   // Setup DFPlayer
//   Serial2.begin(9600, SERIAL_8N1, SPEAKER_RX_PIN, SPEAKER_TX_PIN);
//   if (!myDFPlayer.begin(Serial2)) {
//     Serial.println("DFPlayer initialization failed!");
//     while (true);
//   }
//   myDFPlayer.volume(20);  // Set volume value (0~30).

//   // Game Selection (for simplicity, select via Serial input for testing)
//   Serial.println("Select a game (1-4):");
//   while (gameSelection < 1 || gameSelection > 4) {
//     while (!Serial.available());
//     gameSelection = Serial.parseInt();
//     if (gameSelection < 1 || gameSelection > 4) {
//       Serial.println("Invalid selection. Select a game (1-4):");
//     }
//   }

//   // Play the audio instruction based on game selection
//   myDFPlayer.play(gameSelection); // Play corresponding audio file for game rules
//   delay(5000);  // Delay to allow for audio completion
// }



// void gameTemperatureDetection() {
//   Serial.println("Game 4: Temperature Detection");
//   float initialTemperature = dht.readTemperature();
//   float currentTemperature;
//   while (true) {
//     currentTemperature = dht.readTemperature();
//     Serial.print("Temperature: ");
//     Serial.println(currentTemperature);
//     if (currentTemperature >= initialTemperature + 10) { // Check if temperature increased by 10 degrees
//       myDFPlayer.play(8);  // Play success audio
//       delay(5000);
//       break;
//     }
//     delay(2000); // Check every 2 seconds
//   }
// }



// void loop() {

//   switch (gameSelection) {
//     case 1:
//       gameLightTransmission();
//       break;
//     case 2:
//       gameWeightDetection();
//       break;
//     case 3:
//       // gameHumidityDetection();
//       break;
//     case 4:
//       // gameTemperatureDetection();
//       break;
//     default:
//       break;
//   }
// }


// 音频测试代码（loop）
// Serial.println("Start playing");
  // /*Start playing*/
  // DF1201S.start();
  // delay(3000);
  // Serial.println("Pause");
  // /*Pause*/
  // DF1201S.pause();
  // delay(3000);
  // Serial.println("Next");
  // /*Play the next song*/
  // DF1201S.next();
  // delay(3000);
  // Serial.println("Previous");
  // /*Play the previous song*/
  // DF1201S.last();
  // delay(3000);
  // Serial.println("Start playing");
  // //Fast forward 10S
  // DF1201S.fastForward(/*FF = */10);
  // //Fast Rewind 10S
  // //DF1201S.fastReverse(/*FR = */10);
  // //Start the song from the 10th second 
  // //DF1201S.setPlayTime(/*Play Time = */10);

  // delay(3000);

  // Serial.print("File number:");
  // //Get file number
  // Serial.println(DF1201S.getCurFileNumber());

  // Serial.print("The number of files available to play:");
  // //The number of files available to play
  // Serial.println(DF1201S.getTotalFile());

  // Serial.print("The time length the current song has played:");
  // //Get the time length the current song has played 
  // Serial.println(DF1201S.getCurTime());

  // Serial.print("The total length of the currently-playing song: ");
  // //Get the total length of the currently-playing song 
  // Serial.println(DF1201S.getTotalTime());
  // Serial.print("The name of the currently-playing file: ");
  // //Get the name of the playing file 
  // Serial.println(DF1201S.getFileName());
  // delay(3000);
  // //Play the file No.1, the numbers are arranged according to the sequence of the files copied into the U-disk 
  // DF1201S.playFileNum(/*File Number = */1);
  // //Play the test.mp3 file in test folder 
  // //DF1201S.playSpecFile("/test/test.mp3");

  // while(1);
  // /*Delete the currently-playing file */
  // DF1201S.delCurFile();