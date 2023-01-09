#include <BlynkGOv3.h>

GKeyboard kbd;
GTextArea textarea;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  textarea.align(ALIGN_TOP, 0,10);
  textarea.oneline(false);
  textarea.size(BlynkGO.width()-10, 100);
  kbd.setTextArea(textarea);
}

void loop() {
  BlynkGO.update();
}

