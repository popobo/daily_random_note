from moviepy.editor import VideoFileClip
from pydub import AudioSegment

# 读取视频文件
video_file = VideoFileClip("video.mp4")

# 截取时间段
start_time = 0  # 开始时间（单位：秒）
end_time = 60 # 结束时间（单位：秒）

# 提取音频
audio = video_file.audio.subclip(start_time, end_time)

# 保存为WAV格式
audio.write_audiofile("audio.wav")
