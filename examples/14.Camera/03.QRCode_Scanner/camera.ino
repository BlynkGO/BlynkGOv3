camera_fb_t *fb   = NULL;

//Camera setting
bool camera_init(bool pixel_gray, bool camera_rear) {
  //camera config
  static bool _inited = false;
  if( _inited ) { esp_camera_deinit(); _inited = false; }
  
  camera_config_t camera_config;

  camera_config.ledc_channel = LEDC_CHANNEL_0;
  camera_config.ledc_timer   = LEDC_TIMER_0;
  camera_config.pin_d0       = Y2_GPIO_NUM;
  camera_config.pin_d1       = Y3_GPIO_NUM;
  camera_config.pin_d2       = Y4_GPIO_NUM;
  camera_config.pin_d3       = Y5_GPIO_NUM;
  camera_config.pin_d4       = Y6_GPIO_NUM;
  camera_config.pin_d5       = Y7_GPIO_NUM;
  camera_config.pin_d6       = Y8_GPIO_NUM;
  camera_config.pin_d7       = Y9_GPIO_NUM;
  camera_config.pin_xclk     = XCLK_GPIO_NUM;
  camera_config.pin_pclk     = PCLK_GPIO_NUM;
  camera_config.pin_vsync    = VSYNC_GPIO_NUM;
  camera_config.pin_href     = HREF_GPIO_NUM;
  camera_config.pin_sscb_sda = SIOD_GPIO_NUM;
  camera_config.pin_sscb_scl = SIOC_GPIO_NUM;
  camera_config.pin_pwdn     = PWDN_GPIO_NUM;
  camera_config.pin_reset    = RESET_GPIO_NUM;
  camera_config.xclk_freq_hz = 20000000;

  camera_config.pixel_format = (pixel_gray)? PIXFORMAT_GRAYSCALE : PIXFORMAT_RGB565;
  camera_config.frame_size   = FRAMESIZE_QVGA;
  camera_config.jpeg_quality = 10;
  camera_config.fb_count = 1;

  // camera init
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK)
  {
    Serial.printf("[camera] init failed with error 0x%x\n", err);
    return false;
  }

  sensor_t *s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV2640_PID)
  {
    s->set_vflip(s, !camera_rear);      //flip it back
    s->set_brightness(s, 0); //up the blightness just a bit
    s->set_saturation(s, 1); //lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
  Serial.println("[camera] inited" );
  _inited = true;
  return true; 
}

// สำหรบสลับ byte ของ frame buffer ของกล้อง 
uint8_t* camara_swapbyte(camera_fb_t* fb) {
  uint16_t* fb_buf16 = (uint16_t*) fb->buf;
  for (int16_t j = 0; j < fb->height; j++){
    for (int16_t i = 0; i < fb->width; i++) {
      fb_buf16[j * fb->width + i]  = (fb_buf16[j * fb->width + i] << 8) | (fb_buf16[j * fb->width + i] >> 8);
    }
  }
  return (uint8_t*) fb->buf;
}

// สำหรบปรับ frame buffer ของกล้อง จาก สี 16 bit (สี 565) เป็น ขาวดำ
uint8_t* color565_2grayscale( camera_fb_t* fb){
  static uint8_t* gray_buf = (uint8_t*) esp32_malloc(sizeof(uint8_t) * fb->width * fb->height);
  uint16_t* fb_buf16 = (uint16_t*) fb->buf;

  lv_color_t color16;
  for (int16_t j = 0; j < fb->height; j++){
    for (int16_t i = 0; i < fb->width; i++) {
      color16.full = fb_buf16[j * fb->width + i];
      gray_buf[j * fb->width + i] = (uint8_t)(color16.ch.red + color16.ch.green + color16.ch.blue)/3.0;
    }
  }
  return (uint8_t*) gray_buf;
}

// สำหรบปรับ frame buffer ของกล้อง จาก ขาวดำ ให้เป็น สี 16 bit (สี 565)
uint8_t* grayscale2color565( camera_fb_t* fb){
  static lv_color_t* fb_buf16 = (lv_color_t*) esp32_malloc(sizeof(lv_color_t) * fb->width * fb->height);
  for (int16_t j = 0; j < fb->height; j++){
    for (int16_t i = 0; i < fb->width; i++) {
      uint8_t gray = fb->buf[j * fb->width + i];
      fb_buf16[j * fb->width + i].ch.red     = gray>>3;
      fb_buf16[j * fb->width + i].ch.green   = gray>>2;
      fb_buf16[j * fb->width + i].ch.blue    = gray>>3;
    }
  }
  return (uint8_t*) fb_buf16;
}

void camera_update(void *param) {
  fb = esp_camera_fb_get();
  if(fb->format == PIXFORMAT_RGB565) {
    camera_img.setImage(camara_swapbyte(fb), fb->width, fb->height);
  }else
  if(fb->format == PIXFORMAT_GRAYSCALE) {
    camera_img.setImage(grayscale2color565(fb), fb->width, fb->height);
  }
}

uint16_t camera_width(){
  if( fb == NULL) fb = esp_camera_fb_get();
  return fb->width;
}

uint16_t camera_height(){
  if( fb == NULL) fb = esp_camera_fb_get();
  return fb->height;
}

void qr_scanner(void* param){
  Serial.println("[qr decoder] begin");
  static GTimer label_hidden_timer;
  struct quirc *q;
  struct quirc_code code;
  struct quirc_data qr_data;
  uint8_t *image;

  uint8_t *fb_buf;
  uint32_t fb_len;

  while(1) {
    if(fb == NULL) { delay(10); continue; }

    if(fb->format == PIXFORMAT_RGB565) { 
      fb_buf = color565_2grayscale(fb);
      fb_len = fb->width * fb->height;
    }else
    if(fb->format == PIXFORMAT_GRAYSCALE){
      fb_buf = fb->buf;
      fb_len = fb->len;
    }
    
    q = quirc_new();
    if (!q) {
      Serial.println("[quirc] error : can't create quirc object");
      delay(10); continue;
    }
    
    if (quirc_resize(q, fb->width, fb->height) < 0){
      Serial.println("[quirc] error : resize err");
      quirc_destroy(q);
      delay(10); continue;
    }
    image = quirc_begin(q, NULL, NULL);
    memcpy(image, fb_buf, fb_len);
    quirc_end(q);
    int id_count = quirc_count(q);
    if (id_count == 0) {
//      Serial.println("[quirc] error : not a valid qrcode");
      quirc_destroy(q);
      delay(50); continue;
    }
    quirc_extract(q, 0, &code);
    auto err = quirc_decode(&code, &qr_data);
    if(!err) {
      Serial.printf("[quirc]  %s\n", qr_data.payload);
      lb_qrcode.hidden(false);
      lb_qrcode = String((char*)qr_data.payload);
      BlynkGO.update();
      label_hidden_timer.setOnce(3000L, [](){
        lb_qrcode.hidden(true);
      });
    }
    quirc_destroy(q);
    delay(50);
  }
}

