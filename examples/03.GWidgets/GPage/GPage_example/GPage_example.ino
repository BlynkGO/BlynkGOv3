#include <BlynkGOv3.h>

IMG_DECLARE(rockman_shoot08);

GPage   page;
GImage  img(page);
GLabel  label(page);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  page.size(200,200);         // กำหนดขนาด page ตามต้องการ

  img = rockman_shoot08;
  Serial.println(page.width());
  img.center( page.width()/2, 5);

  label.position(5, 10 + img.height());
  label.color(TFT_BLACK);
  label.wrapable(true);       // ให้ข้อความมีการ ตัดขึ้นบรรทัด ให้พอดีกับ page
  label = "ร็อคแมน เป็นแฟรนไชส์วิดีโอเกมของแคปคอม ซึ่งมีตัวละครหลักคือ ร็อคแมน\n"
          "และตัวละครอื่น ๆ วิดีโอเกมร็อคแมนที่ออกจำหน่ายครั้งแรกในปี พ.ศ. 2530 \n"
          "เป็นเกมสำหรับเครื่องแฟมิคอม และหลังจากนั้นก็มีเกมต่าง ๆ ออกจำหน่ายมากกว่า 50 เกม\n"
          "สำหรับเครื่องเล่นเกมหลากหลายประเภท จนถึงวันที่ 31 ธันวาคม พ.ศ. 2553\n"
          "แคปคอมได้จำหน่ายวิดีโอเกมร็อคแมนไปแล้วกว่า 29 ล้านชุด\n"
          "ทำให้เป็นแฟรนไชส์ที่ออกดอกออกผลมากที่สุดของบริษัท";
  Serial.println(page.width());
}

void loop() {
  BlynkGO.update();
}

