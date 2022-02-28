aenc模块的调用示例，提供了aenc模块接口的调用方法。
demo的输入参数：ak_aenc_demo [采样率] [音频类型] [保存路径] [保存时间（单位：秒）] [音量] [采集源（mic/linein）]
例如输入：ak_aenc_demo 8000 mp3 /mnt/ 10 7 mic

测试者调用aenc demo可以把采集到的音频裸数据进行编码，编码后保存到指定的路径中，测试者通过查看保存的音频文件，可以知道音频编码是否正常工作。

注意：
1、要注意保存路径最后必须带“/”。
2、支持的音频编码格式有mp3/amr/aac/g711a/g711u/pcm。
3、对于保存的音频文件，aac是不带信息头的，所以使用一般的播放器无法播放，但是可以使用adec demo播放；
4、对于保存的音频文件，g711a、g711u是不带信息头的，可以使用cool edit播放；
5、测试此demo时需要先kill掉anyka_ipc，kill掉anyka_ipc的命令是service.sh stop；
6、对于aac、mp3、g711a、g711u格式可以支持采集sample rate不大于48000，sample bit为16，单声道的文件；
7、采集的音频都是单声道的音频；