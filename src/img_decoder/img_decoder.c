/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ img_decoder.c นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย TridentTD
 *   
 *   2.ไม่อนุญาต ให้แจกจ่าย สำเนา หรือแก้ไข โดยไม่ได้รับอนุญาต 
 *   
 *   3.สำหรับผู้ได้รับ ลิขสิทธิ์ สามารถนำไปใช้สร้าง firmware/rom 
 *   สำหรับ บอร์ด ESP32 ที่ระบุเท่านั้น เพื่อใช้ทางการค้าต่างๆได้
 *   โดยห้ามแจกจ่าย จำหน่าย ดัดแปลง แก้ไขใดๆ ในตัว source ไลบรารี่ออกไป
 *
 *   4.หากมีการนำไปใช้คอมไพล์ถือว่าได้ยอมรับ ตามเงื่อนไขลิขสิทธิ์ดังกล่าว
 *   เป็นที่เรียบร้อยแล้ว
 * 
 *********************************************************************
 */
#include "img_decoder.h"

/**********************
        TYPEDEFS
 **********************/
typedef enum { JPEG_SCALE_NONE, JPEG_SCALE_2, JPEG_SCALE_4, JPEG_SCALE_8, JPEG_SCALE_MAX } jpg_scale_t;
typedef enum { JDEV_TYPE_FILE, JDEV_TYPE_ARRAY } jpg_device_type_t;

typedef struct {
  jpg_device_type_t type;
  FILE *file;
  uint8_t* input_data;
  uint32_t input_size;
  uint32_t input_index;
  uint8_t* img_data;   // link to another inner esp32_malloc
  uint16_t w;
  uint16_t h;
} jpg_device_t;

/**********************
    STATIC PROTOTYPES
 **********************/
/* using for lgfx's tjgpd decoder cb */
// static uint32_t jpgfile_reader(lgfxJdec *jpg_decoder, uint8_t *data, uint32_t data_len);
// static uint32_t jpg_data_draw(lgfxJdec *jpg_decoder, void *bitmap, JRECT *rect);

/* using esp32's tjpgd decoder cb */
static uint32_t jpg_reader(JDEC *jpg_decoder, uint8_t *data, uint32_t data_len);
static uint32_t jpg_data_draw(JDEC *jpg_decoder, void *bitmap, JRECT *rect);

/* using for png color converter*/
static void convert_color_depth(uint8_t * img, uint32_t px_cnt, uint32_t cf);

/**********************
    STATIC VARIABLES
 **********************/
static uint8_t       jpg_work[3100];
static jpg_scale_t   jpg_scale = JPEG_SCALE_NONE;

/**********************
     GLOBAL FUNCTIONS
 **********************/
void free_img_dsc(lv_img_dsc_t* img_dsc){
  if(img_dsc->data != NULL) {
    free((void*)img_dsc->data);
    img_dsc->data = NULL;
  }
  memset(img_dsc, 0, sizeof(lv_img_dsc_t));
}

lv_res_t bmp_to_img_dsc(const char* fn, lv_img_dsc_t* img_dsc){
  if(!strcmp(&fn[strlen(fn) - 3], "bmp") || !strcmp(&fn[strlen(fn) - 3], "BMP")) {              /*Check the extension*/
    free_img_dsc(img_dsc);

    FILE * file = fopen(fn, "rb" );
    if(!file) { return LV_RES_INV; }

    uint8_t data[54];
    fread(data, sizeof(uint8_t), 54, file);
    fclose(file);

    // check is_bmp_format ?
    uint16_t bmp_magic;
    bmp_magic = (uint16_t)(data[1]<<8 & 0xFF00) | data[0]; //(*uint16_t*) &data[0];
    // printf("bmp magic : 0x%04X\n", bmp_magic);
    if( bmp_magic != 0x4D42) { return LV_RES_INV; }

    // get bmp's width & height;
    uint16_t bmp_w, bmp_h;
    bmp_w = (uint16_t) *(uint32_t*) &data[18];
    bmp_h = (uint16_t) *(uint32_t*) &data[22];


    uint8_t * img_data = NULL;

    bmp_img bmp_dec;
    bmp_img_read(&bmp_dec, fn);

    img_data = (uint8_t*) esp32_malloc(LV_IMG_BUF_SIZE_TRUE_COLOR(bmp_w, bmp_h));
    if(img_data == NULL) return LV_RES_INV;

    // convert_color_depth to color16bit for TFT only
    uint16_t* img_data16 = (uint16_t*) img_data;
    lv_color_t c;
    for(uint16_t y=0; y < bmp_dec.img_header.biHeight; y ++ ){
      for(uint16_t x=0; x < bmp_dec.img_header.biWidth; x ++ ){
        c = LV_COLOR_MAKE( bmp_dec.img_pixels[y][x].red, bmp_dec.img_pixels[y][x].green, bmp_dec.img_pixels[y][x].blue);
        img_data16[x + y * bmp_dec.img_header.biWidth] = c.full;
      }
    }
    bmp_img_free(&bmp_dec);


    img_dsc->header.always_zero  = 0;
    img_dsc->header.cf           = LV_IMG_CF_TRUE_COLOR;      /*Set the color format*/
    img_dsc->header.w            = bmp_w;
    img_dsc->header.h            = bmp_h;
    img_dsc->data_size           = LV_IMG_BUF_SIZE_TRUE_COLOR(bmp_w, bmp_h);
    img_dsc->data                = img_data;

    return LV_RES_OK;     /*The image is fully decoded. Return with its pointer*/
  }
  return LV_RES_INV;    /*If not returned earlier then it failed*/
}


lv_res_t jpg_to_img_dsc(const char* fn, lv_img_dsc_t* img_dsc){
  if(!strcmp(&fn[strlen(fn) - 3], "jpg") || !strcmp(&fn[strlen(fn) - 3], "JPG")) {              /*Check the extension*/
    free_img_dsc(img_dsc);

    // lgfxJdec      jpg_decoder;
    JDEC          jpg_decoder;
    jpg_device_t  jpg_device;
    JRESULT       jres;

    jpg_device.type  = JDEV_TYPE_FILE;
    jpg_device.file  = fopen(fn, "rb" );
    if (!jpg_device.file) {  return LV_RES_INV; }

    // jres = lgfx_jd_prepare(&jpg_decoder, jpgfile_reader, jpg_work , 3100, &jpg_device );
    jres = jd_prepare(&jpg_decoder, jpg_reader, jpg_work , 3100, &jpg_device );
    if (jres != JDR_OK) { fclose(jpg_device.file); return LV_RES_INV; }

    jpg_device.w = jpg_decoder.width  / (1 << (uint8_t)(jpg_scale));
    jpg_device.h = jpg_decoder.height / (1 << (uint8_t)(jpg_scale));

    uint8_t * img_data = NULL;
    img_data = (uint8_t*) esp32_malloc( LV_IMG_BUF_SIZE_TRUE_COLOR(jpg_device.w, jpg_device.h)); //dsc->header.w, dsc->header.h) );
    if(img_data == NULL) return LV_RES_INV;
    jpg_device.img_data = img_data;

    // jres = lgfx_jd_decomp( &jpg_decoder, jpg_data_draw, (uint8_t) jpg_scale);
    jres = jd_decomp( &jpg_decoder, jpg_data_draw, (uint8_t) jpg_scale);
    fclose(jpg_device.file);
    if (jres != JDR_OK) { return LV_RES_INV; }

    img_dsc->header.always_zero  = 0;
    img_dsc->header.cf           = LV_IMG_CF_TRUE_COLOR;      /*Set the color format*/
    img_dsc->header.w            = jpg_device.w;
    img_dsc->header.h            = jpg_device.h;
    img_dsc->data_size           = LV_IMG_BUF_SIZE_TRUE_COLOR(jpg_device.w, jpg_device.h);
    img_dsc->data                = img_data;

    return LV_RES_OK;     /*The image is fully decoded. Return with its pointer*/
  }
  return LV_RES_INV;    /*If not returned earlier then it failed*/
}

lv_res_t jpg_data_to_img_dsc(uint8_t* input_data, size_t input_size, lv_img_dsc_t* img_dsc){
  if (input_data == NULL || input_size == 0) {  return LV_RES_INV; }

  free_img_dsc(img_dsc);

  JDEC          jpg_decoder;
  jpg_device_t  jpg_device;
  JRESULT       jres;

  jpg_device.type        = JDEV_TYPE_ARRAY;
  jpg_device.input_data  = input_data;
  jpg_device.input_size  = input_size;
  jpg_device.input_index = 0;

  jres = jd_prepare(&jpg_decoder, jpg_reader, jpg_work , 3100, &jpg_device );
  if (jres != JDR_OK) { return LV_RES_INV; }

  jpg_device.w = jpg_decoder.width  / (1 << (uint8_t)(jpg_scale));
  jpg_device.h = jpg_decoder.height / (1 << (uint8_t)(jpg_scale));

  // printf("[j decoder] %d,%d\n", jpg_device.w,jpg_device.h);

  uint8_t * img_data = NULL;
  img_data = (uint8_t*) esp32_malloc( LV_IMG_BUF_SIZE_TRUE_COLOR(jpg_device.w, jpg_device.h)); //dsc->header.w, dsc->header.h) );
  if(img_data == NULL) return LV_RES_INV;
  jpg_device.img_data = img_data;

  jres = jd_decomp( &jpg_decoder, jpg_data_draw, (uint8_t) jpg_scale);
  if (jres != JDR_OK) { return LV_RES_INV; }

  img_dsc->header.always_zero  = 0;
  img_dsc->header.cf           = LV_IMG_CF_TRUE_COLOR;      /*Set the color format*/
  img_dsc->header.w            = jpg_device.w;
  img_dsc->header.h            = jpg_device.h;
  img_dsc->data_size           = LV_IMG_BUF_SIZE_TRUE_COLOR(jpg_device.w, jpg_device.h);
  img_dsc->data                = img_data;

  return LV_RES_OK;     /*The image is fully decoded. Return with its pointer*/
}


lv_res_t png_to_img_dsc(const char* fn, lv_img_dsc_t* img_dsc) {
    if(!strcmp(&fn[strlen(fn) - 3], "png") || !strcmp(&fn[strlen(fn) - 3], "PNG")) {              /*Check the extension*/
        free_img_dsc(img_dsc);

        FILE* file;
        file = fopen(fn, "rb" );
        if(!file) return LV_RES_INV;
        fseek(file, 24, SEEK_SET);
        uint8_t color_info[2];
        fread(color_info, 1, 2, file);
        fclose(file);
        if(color_info[1] != LCT_RGB && color_info[1] != LCT_RGBA) return LV_RES_INV;

        img_dsc->header.always_zero  = 0;
        img_dsc->header.cf           = (color_info[1] == LCT_RGB)? LV_IMG_CF_TRUE_COLOR : LV_IMG_CF_TRUE_COLOR_ALPHA;      /*Set the color format*/


        uint8_t * img_data = NULL;

            unsigned char * png_data;      /*Pointer to the loaded data. Same as the original file just loaded into the RAM*/
            size_t png_data_size;          /*Size of `png_data` in bytes*/

            uint32_t error = lodepng_load_file(&png_data, &png_data_size, fn);   /*Load the file*/
            if(error) {
                printf("error %u: %s\n", error, lodepng_error_text(error));
                return LV_RES_INV;
            }

            // printf("%d\n",png_data_size);

            /*Decode the PNG image*/
            uint32_t png_width;             /*Will be the width of the decoded image*/
            uint32_t png_height;            /*Will be the width of the decoded image*/

            /*Decode the loaded image in ARGB8888 */
            error = lodepng_decode32(&img_data, &png_width, &png_height, png_data, png_data_size);
            free(png_data); //TridentTD  free png file data ออกไป เพราะหาก decoded ได้แล้วจะไปอยู่ที่ img_data แทนแล้ว
            if(error) {
                printf("error %u: %s\n", error, lodepng_error_text(error));
                if(img_data != NULL) free(img_data);
                return LV_RES_INV;
            }

            // printf("png wxh : %dx %d\n", png_width, png_height);
            /*Convert the image to the system's color depth*/
            convert_color_depth(img_data,  png_width * png_height , img_dsc->header.cf);

        
        img_dsc->header.w            = png_width;
        img_dsc->header.h            = png_height;
        img_dsc->data_size           = (color_info[1] == LCT_RGB)? LV_IMG_BUF_SIZE_TRUE_COLOR(png_width, png_height) : LV_IMG_BUF_SIZE_TRUE_COLOR_ALPHA(png_width, png_height) ;

        // convert_color_depth(img_data,  png_width * png_height , img_dsc->header.cf);

        img_dsc->data                = img_data;

        return LV_RES_OK;     /*The image is fully decoded. Return with its pointer*/
    }
    return LV_RES_INV;    /*If not returned earlier then it failed*/
}

/**
 * If the display is not in 32 bit format (ARGB888) then covert the image to the current color depth
 * @param img the ARGB888 image
 * @param px_cnt number of pixels in `img`
 */
static void convert_color_depth(uint8_t * img, uint32_t px_cnt, uint32_t cf)
{
#if LV_COLOR_DEPTH == 32
    lv_color32_t * img_argb = (lv_color32_t*)img;
    lv_color_t c;
    lv_color_t * img_c = (lv_color_t *) img;
    uint32_t i;
    for(i = 0; i < px_cnt; i++) {
        c = LV_COLOR_MAKE(img_argb[i].ch.red, img_argb[i].ch.green, img_argb[i].ch.blue);
        img_c[i].ch.red = c.ch.blue;
        img_c[i].ch.blue = c.ch.red;
    }
#elif LV_COLOR_DEPTH == 16
    lv_color32_t * img_argb = (lv_color32_t*)img;
    lv_color_t c;
    uint32_t i;
    for(i = 0; i < px_cnt; i++) {
        // c = LV_COLOR_MAKE(img_argb[i].ch.red, img_argb[i].ch.green, img_argb[i].ch.blue);
        c = LV_COLOR_MAKE(img_argb[i].ch.blue, img_argb[i].ch.green, img_argb[i].ch.red);
        if( cf == LV_IMG_CF_RAW_ALPHA || cf == LV_IMG_CF_TRUE_COLOR_ALPHA) {
            img[i*3 + 2] = img_argb[i].ch.alpha;
            img[i*3 + 1] = c.full >> 8;
            img[i*3 + 0] = c.full & 0xFF;
        }else
        if( cf == LV_IMG_CF_RAW || cf == LV_IMG_CF_TRUE_COLOR ) {
            img[i*2 + 1] = c.full >> 8;
            img[i*2 + 0] = c.full & 0xFF;
        }
    }
#elif LV_COLOR_DEPTH == 8
    lv_color32_t * img_argb = (lv_color32_t*)img;
       lv_color_t c;
       uint32_t i;
       for(i = 0; i < px_cnt; i++) {
           // c = LV_COLOR_MAKE(img_argb[i].red, img_argb[i].green, img_argb[i].blue);
           c = LV_COLOR_MAKE(img_argb[i].ch.blue, img_argb[i].ch.green, img_argb[i].ch.red);
           img[i*2 + 1] = img_argb[i].alpha;
           img[i*2 + 0] = c.full
       }
#endif
}

static uint32_t jpg_reader(JDEC *jpg_decoder, uint8_t *data, uint32_t data_len) {
  jpg_device_t *jpg_device = (jpg_device_t *) jpg_decoder->device;

  if(jpg_device->type == JDEV_TYPE_FILE) {
    if (data) {
      return fread(data, 1, data_len , jpg_device->file);
    } else {
      fseek(jpg_device->file, data_len, SEEK_CUR);
      return data_len;
    }
  }
  else if(jpg_device->type == JDEV_TYPE_ARRAY ) {
    if (!jpg_device->input_data) return 0;
    if (jpg_device->input_index >= (jpg_device->input_size + 2)) return 0; // end of stream

    if ((jpg_device->input_index + data_len) > (jpg_device->input_size + 2))
      data_len = (jpg_device->input_size + 2) - jpg_device->input_index;

    if (data)
    { // Read len bytes from the input strem
      memcpy(data, jpg_device->input_data + jpg_device->input_index, data_len);
      jpg_device->input_index += data_len;
      return data_len; // Returns number of bytes read
    }
    else
    { // Remove len bytes from the input stream
      jpg_device->input_index += data_len;
      return data_len;
    }
  }
  return data_len;
}

static uint32_t jpg_data_draw(JDEC *jpg_decoder, void *bitmap, JRECT *rect) {
  jpg_device_t *jpg_device = (jpg_device_t *) jpg_decoder->device;

  uint8_t * rgb_data = (uint8_t*) bitmap; //rgb data

  uint16_t *img_data16 = (uint16_t*) jpg_device->img_data;
  uint16_t  img_w      = jpg_device->w;


  int16_t x1 = rect->left;
  int16_t y1 = rect->top;
  int16_t x2 = rect->right;
  int16_t y2 = rect->bottom;

  lv_color_t c;
  uint8_t r, g, b;
  for (uint16_t yp = y1; yp <= y2; yp++) {
    for (uint16_t xp = x1; xp <= x2; xp++) {
      r = *rgb_data++; g = *rgb_data++; b = *rgb_data++;
      c = LV_COLOR_MAKE( r, g, b);
      img_data16[xp + yp * img_w] = c.full;
    }
  }
  return 1;
}


bool isPngFile(const char* fn) {
  /*If it's a PNG file...*/
  if (!strcmp(&fn[strlen(fn) - 3], "png") || !strcmp(&fn[strlen(fn) - 3], "PNG")) {             /*Check the extension*/
    // printf("png file\n");
    /* Read the width and height from the file. They have a constant location:
       [16..23]: width  ... น่าจะผิด
       [24..27]: height ... น่าจะผิด
       [16..19]: width   TridentTD
       [20..23]: height  TridentTD
       [24] bit_dept     TridentTD
       [25] color_type  ; 2 : LCT_RGB, 6 : LCT_RGBA  ..TridentTD
    */
    FILE* file;
    file = fopen(fn, "rb" );
    if (!file) return LV_RES_INV;
    fseek(file, 16, SEEK_SET);
    uint32_t size[2];
    fread(size, 1 , 8, file);
    fseek(file, 24, SEEK_SET);
    uint8_t color_info[2];
    fread(color_info, 1, 2, file);
    fclose(file);

    // printf("png color lct type : %d\n", color_info[1]);

    // ให้เฉพาะ RGB หรือ RGBA เท่านั้น
    if (color_info[1] != LCT_RGB && color_info[1] != LCT_RGBA) return false;

    return true;
  }else{
    return false;
  }
}

bool isBmpFile(const char* fn){
  /*If it's a BMP file...*/
  if (!strcmp(&fn[strlen(fn) - 3], "bmp") || !strcmp(&fn[strlen(fn) - 3], "BMP")) {           /*Check the extension*/

    FILE * file = fopen(fn, "rb" );
    if(!file) { return LV_RES_INV; }

    uint8_t data[54];
    fread(data, sizeof(uint8_t), 54, file);
    fclose(file);

    // check is_bmp_format ?
    uint16_t bmp_magic;
    bmp_magic = (uint16_t)(data[1]<<8 & 0xFF00) | data[0]; //(*uint16_t*) &data[0];
    // printf("bmp magic : 0x%04X\n", bmp_magic);
    if( bmp_magic != 0x4D42) { return false; }
    return true;
  }else{
    return false;
  }
}

bool isJpgFile(const char* fn){
  if (!strcmp(&fn[strlen(fn) - 3], "jpg") || !strcmp(&fn[strlen(fn) - 3], "JPG")) {             /*Check the extension*/
    // lgfxJdec      jpg_decoder;
    JDEC          jpg_decoder;
    jpg_device_t  jpg_device;
    JRESULT       jres;
  
    jpg_device.file  = fopen(fn, "rb" );
    if (!jpg_device.file) {
      return false;
    }
      // jres = lgfx_jd_prepare(&jpg_decoder, jpgfile_reader, jpg_work , 3100, &jpg_device );
      jres = jd_prepare(&jpg_decoder, jpg_reader, jpg_work , 3100, &jpg_device );
    if (jres != JDR_OK) {
      fclose(jpg_device.file);
      return false;
    }

    return true;
  }else{
    return false;
  }
}

bool isGifFile(const char* fn){
if (!strcmp(&fn[strlen(fn) - 3], "gif") || !strcmp(&fn[strlen(fn) - 3], "GIF")) {             /*Check the extension*/
    // printf("gif file\n");
    FILE* file;
    file = fopen(fn, "rb" );
    if (!file) return false;
    uint8_t sigver[3];
    fread(sigver, 1 , 3, file);
    /* Header ? */
    if (memcmp(sigver, "GIF", 3) != 0) {
      // fprintf(stderr, "invalid signature\n");
      printf("error invalid signature\n");
      fclose(file);
      return false;         /*If didn't succeeded earlier then it's an error*/
    }
    /* Version */
    fread(sigver, 1 , 3, file);
    if (memcmp(sigver, "89a", 3) != 0) {
      printf("error invalid version\n");
      fclose(file);
      return false;
    }
    fclose(file);
    return true;
  }
  return false;
}
