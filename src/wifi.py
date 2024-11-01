import socket
import pygame
import os
import time
import requests
import json
from gtts import gTTS

# 初始化音频播放模块
pygame.mixer.init()

# 设置服务器IP和端口
host = "0.0.0.0"  # 允许来自任何IP的连接
port = 12345
# 创建Socket服务器
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))
server_socket.listen(5)
api_key = "AIzaSyB91H1x6yd80Oo604V2gA7gkX5J5uGGiks"  # TODO: 替换为实际的 API 密钥
api_endpoint = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key={api_key}"


def string_to_int(value):
    # 首先将字符串转换为浮点数，然后转换为整数
    int_value = int(float(value))
    return int_value

def play_audio(file_path, client_socket):
    """
    播放指定路径的音频文件，并在播放完成后向ESP32发送消息
    :param file_path: 音频文件的路径
    :param client_socket: 与ESP32通信的socket
    """
    

    if os.path.exists(file_path):
        print("exists")
        print(file_path)
        pygame.mixer.init()

        pygame.mixer.music.load(file_path)
        pygame.mixer.music.play()
        
        # 等待音频播放完成
        while pygame.mixer.music.get_busy():
            time.sleep(0.1)
        
        # 音频播放完成后向ESP32发送消息
        client_socket.sendall("AUDIO_PLAYBACK_COMPLETE".encode())
        print("音频播放完成，已通知ESP32")
    else:
        print(f"文件不存在: {file_path}")

def open_file(file_path):
    """
    打开指定路径的文件
    :param file_path: 文件的路径
    """
    if os.path.exists(file_path):
        os.startfile(file_path)
    else:
        print(f"文件不存在: {file_path}")

def execute_command(command, client_socket):
    """
    根据接收到的命令执行操作
    :param command: 接收到的命令
    :param client_socket: 与ESP32通信的socket
    """
    parts = command.split("+")  # Split command by '+'

    if len(parts) < 2:
        print(f"命令格式错误: {command}")
        return

    # Basic command handling
    if parts[0] == "PLAY":
        # If first part is "PLAY", interpret as play audio command
        audio_file = f"E:/{parts[1]}"  # Assuming the file path format
        play_audio(audio_file, client_socket)
        
    elif parts[0] == "OPEN_FILE":
        # If first part is "OPEN_FILE", open the specified file
        open_file("E:/path/to/your/file.txt")  # Replace with actual file path
        
    elif parts[0] == "RUN_PROGRAM":
        # If first part is "RUN_PROGRAM", execute the specified program
        os.system("E:/path/to/your/program.exe")  # Replace with actual program path

    elif parts[0] == "AI":
        try:
            game_num = int(parts[1])  # Get the game number from the command
            
            # Process different game modes based on game_num
            if game_num == 1:
                if len(parts) >= 4:
                    num1 = int(parts[2])
                    num2 = int(parts[3])
                    ai_game_mode_1(num1, num2, client_socket)
                else:
                    print("参数不足，无法执行游戏模式1")
                    
            elif game_num == 2:
                if len(parts) >= 3:
                    num1 = int(parts[2])
                    ai_game_mode_2(num1, client_socket)
                else:
                    print("参数不足，无法执行游戏模式2")
                    
            elif game_num == 3:
                if len(parts) >= 4:
                    num1 = string_to_int(parts[2])
                    num2 = string_to_int(parts[3])
                    print(num1)
                    print(num2)
                    ai_game_mode_3(num1, num2, client_socket)
                else:
                    print("参数不足，无法执行游戏模式3")
                    
            elif game_num == 4:
                if len(parts) >= 4:
                    num1 = string_to_int(parts[2])
                    num2 = string_to_int(parts[3])
                    ai_game_mode_4(num1, num2, client_socket)
                else:
                    print("参数不足，无法执行游戏模式4")
                
            else:
                print(f"未知游戏模式: {game_num}")
        
        except ValueError:
            print(f"游戏模式编号错误: {parts[1]}")
            
    else:
        print(f"未知指令: {command}")


def generate_ai_response(user_input):
    """
    Send a request to the AI API and get a response.
    """
    headers = {
        "Content-Type": "application/json"
    }
    request_body = {
        "contents": [{"parts": [{"text": user_input}]}]
    }

    # Make a POST request
    response = requests.post(api_endpoint, headers=headers, data=json.dumps(request_body))

    if response.status_code != 200:
        print("API request failed")
        print("HTTP Status Code:", response.status_code)
        print("Server returned error:", response.text)
        return None

    # Parse the JSON response
    response_data = response.json()
    print("API returned content:", json.dumps(response_data, indent=2))  # Print full response for debugging

    try:
        # Extract the specific field containing the AI response
        ai_response = response_data["candidates"][0]["content"]["parts"][0]["text"]
        return ai_response
    except (KeyError, IndexError) as e:
        print("Error parsing the response:", e)
        return None

def text_to_speech(text, output_path):
    """
    Convert text to speech and save as an MP3 file.
    """
    tts = gTTS(text=text, lang='en')
    tts.save(output_path)
    print(f"AI response saved as MP3 at {output_path}")

    # 使用 pygame 播放音频
    pygame.mixer.init()
    pygame.mixer.music.load(output_path)
    pygame.mixer.music.play()
    while pygame.mixer.music.get_busy():  # 等待音频播放完成
        time.sleep(1)
    

def send_ai(user_input):
    """
    Generate AI response and handle a request as in the original code.
    """
    # Generate AI response
    ai_response = generate_ai_response(user_input)
    if ai_response:
        print("AI Response:", ai_response)

        # Convert response to MP3
        text_to_speech(ai_response, "feedback.mp3")

        # Simulate sending response to a client (for illustration)
        # Here you can adapt it to your network or messaging system
        # For now, just print the response
        print("Response sent to client:", ai_response)
    else:
        print("Failed to get AI response.")


# Define dummy functions for the AI game modes for demonstration purposes
def ai_game_mode_1(num1, num2, client_socket):
    prompt = f"""Children are using photoresistors to experiment on the light transmittance of leaves. The normal return value of the photoresistor is {num1}, and the return value after the leaves are shielded is {num2}. Please give a 30-word science popularization for children with the theme of leaves, environment, and science, and use the {num1} value and {num2} value as examples."""
    send_ai(prompt)
    client_socket.sendall("AUDIO_PLAYBACK_COMPLETE".encode())
    print("音频播放完成，已通知ESP32")

    print(f"执行游戏模式1，参数: {num1}, {num2}")

def ai_game_mode_2(num1, client_socket):
    prompt = f"""Children are using a pressure sensor to conduct a weighing experiment. 
    The original value of the pressure sensor is 0, and the return value is {num1}. 
    Please give a 30-word science popularization for children with a scientific explanation of why the weight is different for the same volume. 
    The scientific explanation needs to mention the return value {num1} as an example."""
    send_ai(prompt)
    client_socket.sendall("AUDIO_PLAYBACK_COMPLETE".encode())
    print("音频播放完成，已通知ESP32")
    print(f"执行游戏模式2，参数: {num1}")

def ai_game_mode_3(num1, num2, client_socket):
    prompt = f"Children are using a humidity sensor to measure moisture. The starting humidity is {num1}, and the rising humidity is {num2}. Please give a 30-word science popularization for children with a science lesson about moisture and humidity, and mention the starting humidity {num1} and the rising humidity {num2} as examples."
    send_ai(prompt)
    client_socket.sendall("AUDIO_PLAYBACK_COMPLETE".encode())
    print("音频播放完成，已通知ESP32")
    print(f"执行游戏模式3，参数: {num1}, {num2}")

def ai_game_mode_4(num1, num2, client_socket):
    prompt = f"Children are using a temperature sensor to compare the temperature in the sun and indoors. The indoor temperature is {num1} and the temperature in the sun is {num2}. Please give a 30-word science popularization for children with a science lesson about sunlight and heat energy. The science lesson needs to mention the indoor temperature {num1} and the temperature in the sun {num2} as examples."
    send_ai(prompt)
    client_socket.sendall("AUDIO_PLAYBACK_COMPLETE".encode())
    print("音频播放完成，已通知ESP32")
    print(f"执行游戏模式4，参数: {num1}, {num2}")

# Assuming play_audio and open_file functions are defined elsewhere

# 提供 prompt 以生成 AI 响应





print("等待ESP32的连接...")

try:
    while True:
        client_socket, addr = server_socket.accept()
        print(f"连接来自: {addr}")
        
        # 持续接收来自ESP32的指令
        while True:
            command = client_socket.recv(1024).decode().strip()
            if not command:
                break
            print(f"收到指令: {command}")
            
            # 执行接收到的指令
            execute_command(command, client_socket)
        
        # 关闭客户端连接
        client_socket.close()
finally:
    # 关闭服务器套接字
    server_socket.close()