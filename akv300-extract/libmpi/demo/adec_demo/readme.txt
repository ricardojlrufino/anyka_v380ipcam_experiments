adec模块的调用示例，提供了adec模块接口的调用方法。
demo的输入参数：ak_ai_demo [采样率] [通道数] [音频类型] [播放音频文件全路径]
例如输入：ak_adec_demo 8000 1 mp3 /mnt/20161123-153020.mp3

支持的音频编码格式有mp3/amr/aac/g711a/g711u/pcm。
测试者调用adec demo播放音频文件，可以通过喇叭或者耳机听播放的音频是否正常。

注意：
1、如果需要播放的音频文件是放置在/tmp/目录下，音频文件不要使用太大的文件，否则可能会因为内存不足而无法运行；
2、为了保证测试效果，音频文件请使用语音广播之类的音频，尽量不要使用音乐；
3、播放amr格式只支持sample rate为8000，sample bit为16，单声道的文件；
4、对于aac、mp3、g711a、g711u格式可以支持播放sample rate不大于48000，sample bit为16，单声道的文件；
5、测试此demo时需要先kill掉anyka_ipc，kill掉anyka_ipc的命令是service.sh stop；