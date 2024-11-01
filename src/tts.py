import pyttsx3
from pathlib import Path

# 初始化 TTS 引擎
engine = pyttsx3.init()

# 设置音量，1.0 为最大音量
engine.setProperty('volume', 1.0)  # 设置为 0.0 到 1.0 的值

# 设置语速（可选）
engine.setProperty('rate', 200)  # 调整语速

# 要合成的文本
text = "Today is a wonderful day to build something people love!"

# 将语音保存到文件
speech_file_path = Path.home() / "Downloads" / "speech.wav"
engine.save_to_file(text, str(speech_file_path))

# 执行并保存文件
engine.runAndWait()

print(f"File saved to {speech_file_path}")

from gtts import gTTS
from pathlib import Path

# 要转换的文本
text = "Today is a wonderful day to build something people love!"

# 创建 TTS 对象
tts = gTTS(text=text, lang='en')

# 保存语音文件到 Downloads 文件夹
speech_file_path = Path.home() / "Downloads" / "speech.mp3"
tts.save(speech_file_path)

print(f"File saved to {speech_file_path}")

# 创建 TTS 对象并保存文件
text = "Today is a wonderful day to build something people love!"
tts = gTTS(text=text, lang='en')

# from gtts import gTTS
# from pathlib import Path
# import wave
# import audioop

# # 生成 gTTS 音频文件（先保存为 MP3 格式）
# text = "Today is a wonderful day to build something people love!"
# tts = gTTS(text=text, lang='en')

# # 保存初始 MP3 文件
# speech_file_path = Path.home() / "Downloads" / "speech.mp3"
# tts.save(speech_file_path)

# # 转换 MP3 文件为 WAV 格式
# wav_file_path = Path.home() / "Downloads" / "speech.wav"
# tts.save(wav_file_path)

# # 使用 wave 模块读取 WAV 文件并增加音量
# louder_wav_file_path = Path.home() / "Downloads" / "louder_speech.wav"
# volume_increase_factor = 2  # 放大音量的倍数

# with wave.open(wav_file_path, 'rb') as wf:
#     params = wf.getparams()
#     frames = wf.readframes(wf.getnframes())

#     # 使用 audioop 增加音量
#     louder_frames = audioop.mul(frames, params.sampwidth, volume_increase_factor)

#     # 保存放大音量后的音频文件
#     with wave.open(louder_wav_file_path, 'wb') as louder_wf:
#         louder_wf.setparams(params)
#         louder_wf.writeframes(louder_frames)

# print(f"File with increased volume saved to {louder_wav_file_path}")
