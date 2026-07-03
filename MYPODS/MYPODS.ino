C++
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include "Audio.h"              
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_SCK  18
#define TFT_MOSI 23
#define TFT_RST  4
#define TFT_DC   13
#define TFT_CS   33

#define SD_CS    25
#define SD_MOSI  26
#define SD_MISO  27
#define SD_SCK   32

#define DAC_BCLK 21
#define DAC_WSEL 19
#define DAC_DIN  22

#define BTN_PLAY 34   
#define BTN_UP   35  
#define BTN_DOWN 36   
#define BTN_VOLU 39   
#define BTN_VOLD 5    

bool btConnected = false; 

int volume = 12; 
int trackIndex = 0;
int trackCount = 0;
String playlist[3]; 

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
Audio sdAudio;       
I2SStream btI2S;
BluetoothA2DPSink a2dp(btI2S);

void onBTConnectionChanged(esp_a2d_connection_state_t state, void* obj) {
  if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
    btConnected = true; 
    sdAudio.stopSong(); 
    
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(5, 10);
    tft.print("BLUETOOTH");
    tft.setCursor(5, 50);
    tft.print("Connected!");
  } else {
    btConnected = false; 
    
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(5, 10);
    tft.print("SD MODE");
    tft.setCursor(5, 50);
    tft.print(playlist[trackIndex]);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_PLAY, INPUT);
  pinMode(BTN_UP,   INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_VOLU, INPUT);
  pinMode(BTN_VOLD, INPUT_PULLUP);

  tft.init(76, 284);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(5, 10);
  tft.print("SD MODE");

  SPIClass sdSPI(HSPI);
  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (SD.begin(SD_CS, sdSPI)) {
    File root = SD.open("/");
    if (root) {
      File f = root.openNextFile();
      while (f && trackCount < 3) {
        String name = String(f.name());
        if (name.endsWith(".mp3") || name.endsWith(".MP3")) {
          playlist[trackCount] = "/" + name;
          trackCount = trackCount + 1;
        }
        f = root.openNextFile();
      }
    }
  }

  auto cfg = btI2S.defaultConfig();
  cfg.pin_bck  = DAC_BCLK;
  cfg.pin_ws   = DAC_WSEL;
  cfg.pin_data = DAC_DIN;
  btI2S.begin(cfg);
  
  a2dp.set_on_connection_state_changed(onBTConnectionChanged); 
  a2dp.start("Simple Music Player");

  sdAudio.setPinout(DAC_BCLK, DAC_WSEL, DAC_DIN);
  sdAudio.setVolume(volume);
  
  if (trackCount > 0) {
    sdAudio.connecttoFS(SD, playlist[trackIndex].c_str());
    tft.setCursor(5, 50);
    tft.print(playlist[trackIndex]);
  }
}

void loop() {
  if (btConnected == false) {
    sdAudio.loop(); 
  }


  if (digitalRead(BTN_PLAY) == LOW) {
    if (btConnected == false) {
      sdAudio.pauseResume();
    } else {
      a2dp.pause(); 
    }
    delay(300); 
  }
 
  if (digitalRead(BTN_UP) == LOW) {
    if (btConnected == false) {
      trackIndex = trackIndex + 1;
      if (trackIndex >= trackCount) {
        trackIndex = 0; 
      }
      sdAudio.connecttoFS(SD, playlist[trackIndex].c_str());
      
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(5, 10);
      tft.print("SD MODE:");
      tft.setCursor(5, 50);
      tft.print(playlist[trackIndex]);
    } else {
      a2dp.next(); 
    }
    delay(300); 
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    if (btConnected == false) {
      trackIndex = trackIndex - 1;
      if (trackIndex < 0) {
        trackIndex = trackCount - 1; 
      }
      sdAudio.connecttoFS(SD, playlist[trackIndex].c_str());
      
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(5, 10);
      tft.print("SD MODE:");
      tft.setCursor(5, 50);
      tft.print(playlist[trackIndex]);
    } else {
      a2dp.previous(); 
    }
    delay(300); 
  }


  if (digitalRead(BTN_VOLU) == LOW) {
    if (volume < 21) {
      volume = volume + 1;
    }
    sdAudio.setVolume(volume);
    a2dp.set_volume(map(volume, 0, 21, 0, 127));
    delay(150); 
  }

  if (digitalRead(BTN_VOLD) == LOW) {
    if (volume > 0) {
      volume = volume - 1;
    }
    sdAudio.setVolume(volume);
    a2dp.set_volume(map(volume, 0, 21, 0, 127));
    delay(150); 
  }
}

void audio_eof_mp3(const char* info) {
  if (btConnected == false) {
    trackIndex = trackIndex + 1;
    if (trackIndex >= trackCount) {
      trackIndex = 0; 
    }
    sdAudio.connecttoFS(SD, playlist[trackIndex].c_str());
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(5, 10);
    tft.print("SD MODE:");
    tft.setCursor(5, 50);
    tft.print(playlist[trackIndex]);
  }
}
