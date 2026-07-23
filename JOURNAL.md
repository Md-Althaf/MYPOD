---
title: "MYPOD"
author: "BOSE-NOVA"
description: "MYPODS is a handheld Music Player , which u can listen music via Bluetooth or play from SD card and control Songs , listen music via headphone jack , Also Get songs meta data on the display ."
created_at: "2026-03-20"
---
# 7/23/2026 8 PM - Case Reshaping
_Time spent: 1.5h_

Done some changes in the 3d case added mouting structure for esp32 . and some more changes 
like adding test and other things.
![alt text](images/3dimage1.png)
![alt text](images/3dimage2.png)
![alt text](images/3dimage3.png)
![alt text](images/3dimage4.png)
![alt text](images/3dimage5.png)


# 7/2/2026 11 PM - CODE complete

_Time spent: 3h_


Completed the code and changed the ESP32-S3 TO ESP32 as in my search for libraries i concluded 
that S3 can't  recieve audio data over bluetooth so i switched to esp32 .
So i have to redone the schematics for esp32 .
Also changed the code completely i didnt recorded the timelapse .
![SCHEMATIC image](Images/image.png)
https://lapse.hackclub.com/timelapse/C1CmekGi5kCg
https://lapse.hackclub.com/timelapse/6sTZUo41alyg


# 7/1/2026 8 AM - 3D Case done

_Time spent: 2h_

I have very little experience with Fusion 360, but not with 3D modeling. I started with Blender and I'm pretty good at it, but Fusion 360 is still new to me. Because of that, it took me a long time to create this model.

Some parts, like the DAC dimensions, weren't available, and it took me around 15 minutes to figure them out. Even AI couldn't help, but I eventually found the dimensions myself. I think that's because the module is relatively new.

![image](https://stasis.hackclub-assets.com/images/1782895199123-oyopdr.png)

Completed the 3D model.

![image](https://stasis.hackclub-assets.com/images/1782895199123-oyopdr.png)

# 6/25/2026 10 AM - Circuit Connection

_Time spent: 2.5h_

Connections is done i am using 7 buttons for controlls like(PLay/pause , vol + , vol - , Up , down , Ok , back) 
For selecting modes and songs to play.


![image](https://stasis.hackclub-assets.com/images/1782382607653-fc3gd3.png)


The main problem was DAC connections. I don't understand how it works. I have to go through many tutorials  but couldn't find any. I googled it and asked Gemini, and i got a connection from adafruit webistes 
![image](https://stasis.hackclub-assets.com/images/1782383047050-32jqd3.png)
https://learn.adafruit.com/adafruit-tlv320dac3100-i2s-dac/circuitpython

![image](https://stasis.hackclub-assets.com/images/1782382607653-fc3gd3.png)
![image](https://stasis.hackclub-assets.com/images/1782383047050-32jqd3.png)

# 6/24/2026 8 PM - Desgined the Whole MyPod

_Time spent: 2h_

It is hard to find Items with its dimensions 
also some parts which are cheaper are either out of stock or cant be shipped to my location.


1.Display (This is best this is different this is a strip display perfect for handheld device and also it is perfect for using esp32)

![image](https://stasis.hackclub-assets.com/images/1782332472297-32na38.png)
2.DAC (It is DAC esp32 can't generate analog signals, so we need DAC for this bonus, it has a speaker output also)

![image](https://stasis.hackclub-assets.com/images/1782332530374-rsuut0.png)
I cant find any cheaper options i got this it looks great but dont know how it works also no tutorial in yt .

3.ESP32-S3 (Main component for the Mypod It 
controls everything)
![image](https://stasis.hackclub-assets.com/images/1782332572061-y4wwkk.png)
.

4.Battery(I am using 2x 1200mah battery as 1200 mah baterry is in perfect size and i My pod has to have a longer working duration)

![image](https://stasis.hackclub-assets.com/images/1782332640787-mj6869.png)

5.SD card reader (Can listen to music even offline)

![image](https://stasis.hackclub-assets.com/images/1782332781688-v5gp46.png)


It almost took me 1.5 hr to find the items cause i am not getting smaller boards, especially display and DAC. I can't find the perfect size one; mostly

#MyPod
My pod is a handheld music player u can listen to music via wired earphones over Bluetooth if you don't have and wirless Ear phones or want to use wired earphones to use as one just plug it and connect to esp32 and just play music and control it and can also play music form sd card .

![image](https://stasis.hackclub-assets.com/images/1782332472297-32na38.png)
![image](https://stasis.hackclub-assets.com/images/1782332530374-rsuut0.png)
![image](https://stasis.hackclub-assets.com/images/1782332572061-y4wwkk.png)
![image](https://stasis.hackclub-assets.com/images/1782332640787-mj6869.png)
![image](https://stasis.hackclub-assets.com/images/1782332781688-v5gp46.png)




