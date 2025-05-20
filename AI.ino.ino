// ESP32 Voice Assistant Core
#include <WiFi.h>
#include <HTTPClient.h>
#include <driver/i2s.h>

#define RELAY_PIN 23
#define PIR_PIN 22
#define SWITCH_MODE_PIN 21
#define SPEAKER_BCLK 26
#define SPEAKER_LRC 25
#define SPEAKER_DOUT 27

bool gestureMode = false;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_MODE_PIN, OUTPUT);
  
  digitalWrite(SWITCH_MODE_PIN, LOW); // Start in voice mode

  // Init I2S for speaker
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = SPEAKER_BCLK,
    .ws_io_num = SPEAKER_LRC,
    .data_out_num = SPEAKER_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop() {
  if (digitalRead(PIR_PIN)) {
    Serial.println("Intruder Detected!");
    // Optional: Send alert via WiFi
    // HTTPClient http;
    // http.begin("http://your-server.local/fire");
    // http.GET();
  }

  String voiceCommand = listenToCommand(); // You can fake it for now
  handleCommand(voiceCommand);
}

String listenToCommand() {
  // Dummy function for now
  delay(2000);
  return "turn on light";
}

void handleCommand(String cmd) {
  cmd.toLowerCase();

  if (cmd.indexOf("turn on light") != -1) {
    digitalWrite(RELAY_PIN, HIGH);
    speak("Turning on the light");
  } else if (cmd.indexOf("turn off light") != -1) {
    digitalWrite(RELAY_PIN, LOW);
    speak("Turning off the light");
  } else if (cmd.indexOf("gesture mode") != -1) {
    gestureMode = true;
    digitalWrite(SWITCH_MODE_PIN, HIGH);
    speak("Switching to gesture mode");
  } else if (cmd.indexOf("voice mode") != -1) {
    gestureMode = false;
    digitalWrite(SWITCH_MODE_PIN, LOW);
    speak("Back to voice mode");
  }
}

void speak(String msg) {
  // Placeholder – replace with actual TTS or audio playback
  Serial.println("[SPEAK] " + msg);
}
