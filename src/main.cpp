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
#include <cstdlib>


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
#define DHTTYPE DHT11
#define LED_PIN 22

DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);
int pwmChannel = 0;    // PWM 通道
int pwmFreq = 5000;    // PWM 频率
int pwmResolution = 8; // PWM 分辨率为 8 位

void gameLightTransmission();
void gameWeightDetection();
void gameHumidityDetection();
void gameTemperatureDetection();
String generateAIResponse(String input);
void textToSpeech(String text, const char* filename);
void connectWifi();
void sendAI(String userInput);
void sendCommandAndWait(String command);
int readAverageLightValue(int pin, int sampleCount);


DFRobot_DF1201S DF1201S;
// 无用软连接
// SoftwareSerial mySerial(16, 17);  // RX = GPIO16, TX = GPIO17
// SoftwareSerial DF1201SSerial(RX, TX);  //RX  TX
HardwareSerial mySerial(2);  // 使用 UART1

// 设置 WiFi 凭据和设备名称
const char* ssid = "HW";  // TODO: 替换为您的 Wi-Fi SSID
const char* password = "92316885";  // TODO: 替换为您的 Wi-Fi 密码
const char* device_name = "ESP32_Sensor";  // TODO: 替换为设备名称
WiFiServer wifi_server(80);  // 初始化 WiFi 服务器

// 设置 AI API 密钥
const char* api_key = "AIzaSyB91H1x6yd80Oo604V2gA7gkX5J5uGGiks";  // TODO: 替换为您的实际 API 密钥
const char* api_endpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=AIzaSyB91H1x6yd80Oo604V2gA7gkX5J5uGGiks";  // TODO: 替换为实际的 API 端点
int gameSelection = 1;
// Wi-Fi 网络参数
const char* computerIP = "192.168.43.93";  // 替换为电脑的IP地址
const uint16_t port = 12345;




// TODO 有效代码
void setup() {
  Serial.begin(115200);  // 启动串口监视器
  SPIFFS.begin(true);    // 初始化 SPIFFS 文件系统
  // 设置 LED 引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  ledcSetup(pwmChannel, pwmFreq, pwmResolution); // 配置 PWM 通道
  ledcAttachPin(LED_PIN, pwmChannel); // 将 LED 引脚附加到 PWM 通道
  connectWifi();
  //音频信号连接
  // mySerial.begin(115200, SERIAL_8N1, 16, 17);  // GPIO16 RX, GPIO17 TX
  dht.begin();

    // 尝试初始化 DF1201S
  Serial.println("Serial is working.");
}

void loop(){ 
  // 播放游戏规则
    String command;
  switch (gameSelection) {
    case 0:
      command = "PLAY+rule.mp3";
      sendCommandAndWait(command);
      gameSelection += 1;
    case 1:
      gameLightTransmission();
      gameSelection += 1;
      break;
    case 2:
      gameWeightDetection();
      gameSelection += 1;
      break;
    case 3:
      gameHumidityDetection();
      gameSelection += 1;
      break;
    case 4:
      gameTemperatureDetection();
      gameSelection += 1;
      break;
    case 5:
      command = "PLAY+finish.mp3";
      sendCommandAndWait(command);
    default:
      break;
  }
  
}

void gameLightTransmission() {
  // 初始化
  Serial.println("Game 1: Light Transmission Test");

  // 发出播放命令，播放 T1.mp3
  String command = "PLAY+T1.mp3";
  sendCommandAndWait(command);

  int lightValue;
  unsigned long fsrStartTime = 0;
  bool fsrTriggered = false;
  unsigned long lightStartTime = 0;
  bool lightInRange = false;

  // 初始化前 10 次光线值数组
  int initialLightValues[10] = {0};
  int lightValueCount = 0;
  int minLightValueInRange = 4096; // 初始化一个较大的值

  while (true) {
    delay(500);
    
    // 读取压力传感器的数值
    int fsrValue = analogRead(PRESSURE_SENSOR_PIN);
    Serial.print("Pressure Sensor Value: ");
    Serial.println(fsrValue);
    int  AverageLightValue = readAverageLightValue(LIGHT_SENSOR_PIN,30);
    
    // 检测 fsrValue 是否大于 1000 并持续三秒
    if (fsrValue > 1000) {
      if (!fsrTriggered) {
        fsrStartTime = millis();
        fsrTriggered = true;
        Serial.println("Pressure sensor triggered, starting 3-second timer.");

      } else if (millis() - fsrStartTime >= 3000) {
        // 进入判定状态
        Serial.println("Entering Judgement State");
        
        // 在判定状态中，维持直到判定成功
        while (true) {
          // 读取光线传感器的数值
          lightValue = analogRead(LIGHT_SENSOR_PIN);

          // 打印当前光线值，以检查是否发生了变化
          Serial.print("Light Value during judgement: ");
          Serial.println(lightValue);


          // 记录前 10 次光线值
          if (lightValueCount < 10) {
            initialLightValues[lightValueCount] = lightValue;
            lightValueCount++;
          }

          // 将读取的光线值映射到亮度范围 0-255
          int brightness = map(lightValue, 0, 4095, 0, 255);
          brightness = 255 - brightness;
          ledcWrite(pwmChannel, brightness);
          
          // 打印亮度调节信息
          Serial.print("Mapped Brightness: ");
          Serial.println(brightness);
          Serial.print("PWM Channel Brightness: ");
          Serial.println(ledcRead(pwmChannel));
          Serial.print("AverageLightValue");
          Serial.println(AverageLightValue);
          // 判断 lightValue 是否在 0-1500 的区间内
          if (lightValue >= (AverageLightValue-1800) && lightValue <= (AverageLightValue-200)) {
            if (!lightInRange) {
              lightStartTime = millis();
              lightInRange = true;
              minLightValueInRange = lightValue; // 初始化区间内的最小值
              Serial.println("Light value in range, starting 3-second timer.");
            } else {
              if (millis() - lightStartTime >= 3000) {
                // lightValue 在区间内并持续了三秒，判定成功
                Serial.println("Light value in range for 3 seconds. Success!");

                // 发出播放成功音频命令
                command = "PLAY+T1-s.mp3";
                sendCommandAndWait(command);

                // 计算前 10 次光线值的平均值
                int sum = 0;
                for (int i = 0; i < lightValueCount; ++i) {
                  sum += initialLightValues[i];
                }
                int averageValue = (lightValueCount > 0) ? (sum / lightValueCount) : 0;

                // 发送 AI command，包含游戏选择、前 10 次平均值和最后区间内最小值
                command = "AI+1+" + String(averageValue) + "+" + String(minLightValueInRange);
                Serial.println("Sending command: " + command);
                sendCommandAndWait(command);

                return;
              } else {
                // 更新区间内的最小光线值
                if (lightValue < minLightValueInRange) {
                  minLightValueInRange = lightValue;
                }
              }
            }
          } else {
            // 如果不在范围内，重置计时
            lightInRange = false;
            Serial.println("Light value out of range, resetting timer.");

            // 发出播放失败音频命令
            command = "PLAY+T1-f.mp3";
            sendCommandAndWait(command);
          }
          
          // 打印调试信息
          Serial.print("Light Value: ");
          Serial.println(lightValue);
          delay(10000);
        }
      }
    } else {
      // 如果压力值小于 1000，不退出方法，只重置触发状态
      fsrTriggered = false;
      fsrStartTime = 0; // 重置开始时间以便重新计时
      Serial.println("Pressure sensor value below threshold, resetting trigger state.");
    }
  }
}




void gameWeightDetection() {
    // 测量重量，设定区间和计时，通关播放音频，提示音频
    Serial.println("Game 2: Weight Detection");

    // 发出播放命令，播放 T2.mp3
    String command = "PLAY+T2.mp3";
    sendCommandAndWait(command);
    
    int pressureValue;
    unsigned long startTime = 0;
    bool inRange = false;

    while (true) {
        pressureValue = analogRead(PRESSURE_SENSOR_PIN);
        Serial.print("Pressure: ");
        Serial.println(pressureValue);

        if (pressureValue >= 1500 && pressureValue <= 3000) {
            if (!inRange) {
                      Serial.print("inRange");


                // 开始计时
                startTime = millis();
                inRange = true;
            }

            // 检查是否持续在范围内超过 3 秒
            if (millis() - startTime >= 3000) {
              Serial.print("millis");
                // 成功通关，播放成功音频
                command = "PLAY+T2-s.mp3";
                sendCommandAndWait(command);

                delay(5000);

                // 发送最后检测到的压力值
                command = "AI+2+" + String(pressureValue);
                Serial.println("Sending command: " + command);
                sendCommandAndWait(command);

                break;
            }
        } else {
            // 如果不在范围内，重置计时
            inRange = false;
        }

        // 如果未能达到目标压力范围，则播放提示音频
        if (!inRange) {
            if (pressureValue < 1000) {
                command = "PLAY+T2-f1.mp3";
            } else if (pressureValue > 3000) {
                command = "PLAY+T2-f2.mp3";
            }
            sendCommandAndWait(command);
        }

        delay(200);
    }

    
}






void gameHumidityDetection() {
  //读取湿度
  Serial.println("Game 3: Humidity Detection");

  // 发出播放命令，播放 T3.mp3
  String command = "PLAY+T3.mp3";
  sendCommandAndWait(command);


  float humidity;
  float initialHumidity = 0; 
  unsigned long fsrStartTime = 0;
  bool fsrTriggered = false;
  bool humidityInRange = false;
  unsigned long humidityStartTime = 0;

  while (true) {
    delay(200);
    
    // 读取压力传感器的数值
    int fsrValue = analogRead(PRESSURE_SENSOR_PIN);
    Serial.print("Pressure Sensor Value: ");
    Serial.println(fsrValue);
    
    
    // 检测 fsrValue 是否大于 1000 并持续三秒
    if (fsrValue > 1000) {
      if (!fsrTriggered) {
        fsrStartTime = millis();
        fsrTriggered = true;
        Serial.println("Pressure sensor triggered, starting 3-second timer.");
      } else if (millis() - fsrStartTime >= 3000) {
        // 进入判定状态
        Serial.println("Entering Judgement State");
        
        // 在判定状态中，维持直到判定成功
        while (true) {
          // 读取湿度传感器的数值
          humidity = dht.readHumidity();
          
          // 打印湿度信息         
          Serial.print("Humidity: ");
          Serial.println(humidity);

          // 记录最开始的湿度值
          if (initialHumidity == 0 && humidity > 0) {
            initialHumidity = humidity;
          }
          
          // 判断 humidity 是否在60以上
          if (humidity >= 64 ) {
            if (!humidityInRange) {
              humidityStartTime  = millis();
              humidityInRange = true;
              Serial.println("Humidity value in range, starting 3-second timer.");
            } else if (millis() - humidityStartTime  >= 1000) {
              // 湿度大于60并持续了三秒，判定成功，跳出方法
              Serial.println("Humidity value in range for 3 seconds. Success!");

              // 发出播放成功音频命令
              command = "PLAY+T3-s.mp3";
              sendCommandAndWait(command);

              // 发送最开始的湿度值和成功的湿度值
              command = "AI+3+" + String(initialHumidity) + "+" + String(humidity);
              Serial.println("Sending command: " + command);
              sendCommandAndWait(command);
              
              return;
            }
          } else {
            // 如果不在范围内，重置计时
            humidityInRange  = false;
            Serial.println("Humidity value out of range, resetting timer.");

            // 发出播放失败音频命令
            command = "PLAY+T3-f.mp3";
            sendCommandAndWait(command);
            
          }
          
          // 打印调试信息
          Serial.print("Humidity value: ");
          Serial.println(humidity);
          delay(500);
        }
      }
    } else {
      // 如果压力值小于 1000，不退出方法，只重置触发状态
      fsrTriggered = false;
      fsrStartTime = 0; // 重置开始时间以便重新计时
      Serial.println("Pressure sensor value below threshold, resetting trigger state.");
    }
  }
}



void gameTemperatureDetection() {
  // Game: Temperature Detection
  Serial.println("Game: Temperature Detection");

  // 发出播放命令，播放 T4.mp3
  String command = "PLAY+T4.mp3";
  sendCommandAndWait(command);

  float initialTemperature = dht.readTemperature();
  float currentTemperature;
  float lowestTemperature = 100;
  unsigned long fsrStartTime = 0;
  bool fsrTriggered = false;

  unsigned long temperatureStartTime = 0;
  bool temperatureInRange = false;

  while (true) {
    delay(200);

    // 读取压力传感器的数值
    int fsrValue = analogRead(PRESSURE_SENSOR_PIN);
    Serial.print("Pressure Sensor Value: ");
    Serial.println(fsrValue);

    // 检测 fsrValue 是否大于 1000 并持续三秒
    if (fsrValue > 1000) {
      if (!fsrTriggered) {
        fsrStartTime = millis();
        fsrTriggered = true;
        Serial.println("Pressure sensor triggered, starting 3-second timer.");
      } else if (millis() - fsrStartTime >= 3000) {
        // 进入判定状态
        Serial.println("Entering Judgement State");

        // 在判定状态中，持续检测温度
        while (true) {
          currentTemperature = dht.readTemperature();
          Serial.print("Temperature: ");
          Serial.println(currentTemperature);

          // 更新最低温度
          if (currentTemperature < lowestTemperature) {
            lowestTemperature = currentTemperature;
            Serial.print("New lowest temperature recorded: ");
            Serial.println(lowestTemperature);
          }

          // 检查温度是否比最低温度高出 2 度
          if (currentTemperature >= lowestTemperature + 1) {
            if (!temperatureInRange) {
              temperatureStartTime = millis();
              temperatureInRange = true;
              Serial.println("Temperature in range, starting 3-second timer.");
            } else if (millis() - temperatureStartTime >= 100) {
              // 温度高于最低温度 2 度并持续了三秒，判定成功，跳出方法
              Serial.println("Temperature increased by 2 degrees from the lowest value for 3 seconds. Success!");

              // 发出播放成功音频命令
              command = "PLAY+T4-s.mp3";
              sendCommandAndWait(command);

              // 发送成功的温度值
              command = "AI+4+" + String(currentTemperature) + "+" + String(lowestTemperature);
              Serial.println("Sending command: " + command);
              sendCommandAndWait(command);

              return;
            }
          } else {
            // 如果温度不满足条件，重置计时并播放失败音频
            temperatureInRange = false;
            Serial.println("Temperature not sufficient, resetting timer.");
            command = "PLAY+T4-f.mp3";
            sendCommandAndWait(command);
          }

          // 打印调试信息
          Serial.print("Current temperature: ");
          Serial.println(currentTemperature);
          Serial.print("Lowest temperature recorded: ");
          Serial.println(lowestTemperature);
          delay(2000);
        }
      }
    } else {
      // 如果压力值小于 1000，不退出方法，只重置触发状态
      fsrTriggered = false;
      fsrStartTime = 0; // 重置开始时间以便重新计时
      Serial.println("Pressure sensor value below threshold, resetting trigger state.");
    }
  }
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

void sendCommandAndWait(String command) {
    WiFiClient client;
    
    // 无限尝试连接直到成功
    while (!client.connect(computerIP, port)) {
        Serial.println("无法连接到电脑，重试中...");
        delay(1000);  // 等待一秒后再尝试连接，防止过快重试
    }
    
    Serial.println("连接成功！");
    
    // 发送指令
    client.println(command);
    Serial.println("已发送指令: " + command);

    // 等待来自服务器的完成响应
    bool playbackComplete = false;
    while (client.connected() && !playbackComplete) {
        if (client.available()) {
            String response = client.readStringUntil('\n');
            response.trim();
            Serial.println("收到响应: " + response);  // 打印收到的每个响应

            if (response == "AUDIO_PLAYBACK_COMPLETE") {
                Serial.println("音频播放已完成");
                playbackComplete = true;  // 设置标志以退出等待循环
            }
        } else {
            Serial.println("Waiting for server response...");  // 表示在等待响应
            delay(1000);  // 延长等待时间以避免频繁输出
        }
    }

    if (!client.connected() && !playbackComplete) {
        Serial.println("连接已断开，未收到播放完成信号，尝试重新连接...");
        client.stop();  // 关闭当前连接
        delay(2000);    // 等待片刻
        sendCommandAndWait(command);  // 尝试重新发送指令
        return;
    }
    
    // 确保连接关闭
    client.stop();
    
    // 播放完成后继续执行的下一步代码
    if (playbackComplete) {
        Serial.println("准备执行下一步操作...");
        // 下一步代码可以放在这里
    } else {
        Serial.println("未收到播放完成信号，执行超时处理...");
        // 可以在这里添加处理未收到信号的情况
    }
}

int readAverageLightValue(int pin, int sampleCount) {
    long sum = 0;  // 使用 long 类型来存储总和，避免数值溢出
    
    // 连续读取 sampleCount 次光敏电阻值
    for (int i = 0; i < sampleCount; i++) {
        int lightValue = analogRead(pin);  // 读取光敏电阻的模拟值
        sum += lightValue;  // 将读到的值加到总和中
        delay(10);  // 延迟 10 毫秒，防止读取过于频繁
    }
    
    // 计算平均值
    int averageValue = sum / sampleCount;
    return averageValue;
}
