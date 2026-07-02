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

enum Mode { MODE_BLUETOOTH, MODE_SD };
Mode currentMode = MODE_BLUETOOTH; 

int volume = 12; 
int trackIndex = 0;
int trackCount = 0;
String playlist[50]; 
unsigned long lastPress = 0;
String btSongTitle = "No Audio";

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
Audio sdAudio;       
I2SStream btI2S;
BluetoothA2DPSink a2dp(btI2S);

void updateScreen(String modeName, String songName) {
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(5, 10);
  tft.print(modeName);
 
  tft.drawFastHLine(0, 35, 240, ST77XX_WHITE);
  
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(5, 50);
  tft.println(songName);
}

void onBTMetadata(uint8_t id, const uint8_t* text) {
  if (id == ESP_AVRC_MD_ATTR_TITLE) {
    btSongTitle = String((char*)text);
    if (currentMode == MODE_BLUETOOTH) {
      updateScreen("BLUETOOTH", btSongTitle);
    }
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
  updateScreen("STARTING...", "Please Wait");

  SPIClass sdSPI(HSPI);
  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  if (SD.begin(SD_CS, sdSPI)) {
    File root = SD.open("/music");
    if (root) {
      File f = root.openNextFile();
      while (f && trackCount < 50) {
        String name = String(f.name());
        if (name.endsWith(".mp3") || name.endsWith(".MP3")) {
          playlist[trackCount++] = name;
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
  
  a2dp.set_avrc_metadata_callback(onBTMetadata); 
  a2dp.start("Simple Music Player");

  sdAudio.setPinout(DAC_BCLK, DAC_WSEL, DAC_DIN);
  sdAudio.setVolume(volume);

  updateScreen("BLUETOOTH", "Ready to pair...");
}

void loop() {
  if (currentMode == MODE_SD) {
    sdAudio.loop();
  }

  if (millis() - lastPress > 200) {
    
    if (digitalRead(BTN_PLAY) == LOW && digitalRead(BTN_UP) == LOW) {
      lastPress = millis();
      
      if (currentMode == MODE_BLUETOOTH && trackCount > 0) {
        currentMode = MODE_SD;
        playSDTrack(0);
      } else {
        currentMode = MODE_BLUETOOTH;
        sdAudio.stopSong(); 
        updateScreen("BLUETOOTH", btSongTitle);
      }
      return; 
    }

    if (digitalRead(BTN_PLAY) == LOW) {
      lastPress = millis();
      if (currentMode == MODE_SD) {
        sdAudio.pauseResume();
      } else {
        a2dp.pause(); 
      }
    }
   
    if (digitalRead(BTN_UP) == LOW) {
      lastPress = millis();
      if (currentMode == MODE_SD) {
        playSDTrack(trackIndex + 1);
      } else {
        a2dp.next(); 
      }
    }

    if (digitalRead(BTN_DOWN) == LOW) {
      lastPress = millis();
      if (currentMode == MODE_SD) {
        playSDTrack(trackIndex - 1);
      } else {
        a2dp.previous(); 
      }
    }

    if (digitalRead(BTN_VOLU) == LOW) {
      lastPress = millis();
      volume = min(21, volume + 1);
      sdAudio.setVolume(volume);
      a2dp.set_volume(map(volume, 0, 21, 0, 127));
    }

    if (digitalRead(BTN_VOLD) == LOW) {
      lastPress = millis();
      volume = max(0, volume - 1);
      sdAudio.setVolume(volume);
      a2dp.set_volume(map(volume, 0, 21, 0, 127));
    }
  }
}

void playSDTrack(int index) {
  if (trackCount == 0) return;
  trackIndex = (index + trackCount) % trackCount;
  String path = "/music/" + playlist[trackIndex];
  sdAudio.connecttoFS(SD, path.c_str());
  
  updateScreen("SD CARD MODE", playlist[trackIndex]);
}

void audio_eof_mp3(const char* info) {
  if (currentMode == MODE_SD) {
    playSDTrack(trackIndex + 1);
  }
}
