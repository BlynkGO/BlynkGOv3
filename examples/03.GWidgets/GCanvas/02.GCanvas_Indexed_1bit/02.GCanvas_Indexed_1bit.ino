#include <BlynkGOv3.h>

// IMG_CF_INDEXED_1BIT : canvas แบบ indexed ด้วย palette สี 1 bit (2 สี)
// IMG_CF_INDEXED_2BIT : canvas แบบ indexed ด้วย palette สี 2 bit (4 สี)
// IMG_CF_INDEXED_4BIT : canvas แบบ indexed ด้วย palette สี 4 bit (16 สี)
// IMG_CF_INDEXED_8BIT : canvas แบบ indexed ด้วย palette สี 8 bit (256 สี)

GCanvas canvas(IMG_CF_INDEXED_1BIT) ;  

void setup(){
  BlynkGO.begin();

  canvas.size( 300, 250);

  // กำหนด palette สี  
  // หากเป็น IMG_CF_INDEXED_1BIT กำหนดได้ PALETTE_0, PALETTE_1
  // หากเป็น IMG_CF_INDEXED_2BIT กำหนดได้ PALETTE_0, PALETTE_1, PALETTE_2, PALETTE_3
  // หากเป็น IMG_CF_INDEXED_4BIT กำหนดได้ PALETTE_0, PALETTE_1, ..., PALETTE_14, PALETTE_15
  // หากเป็น IMG_CF_INDEXED_8BIT กำหนดได้ PALETTE_0, PALETTE_1, ..., PALETTE_254, PALETTE_255
  canvas.palette( PALETTE_0, TFT_WHITE);
  canvas.palette( PALETTE_1, TFT_RED);

  canvas.fillScreen(PALETTE_0);                           // ใช้สีจาก palette ลงสีพื้นหลังให้ canvas 
  canvas.fillRect( Point(10,20), 120, 80, PALETTE_1);      // วาดสี่เหลี่ยมลงบน canvas

  canvas.font(prasanmit_40, PALETTE_0);
  canvas.drawText( "Hello", Point( 20, 25), 120, TEXT_ALIGN_CENTER);

}

void loop(){
  BlynkGO.update();  
}

