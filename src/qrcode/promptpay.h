/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ promptpay.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __promptpay_h__
#define __promptpay_h__

#include <Arduino.h>

/* id : 13 หลัก ---> บัตรประชาชนไทย ... หรือ */
/* id : 10 หลัก ---> เบอร์มือถือไทย */
String promptpay_encode_tag29(String id, float amount, bool multi_use = true);

/* biller_id     : 15 หลัก fix */
/* reference_id1 : ไม่เกิน 20 หลัก */
/* reference_id2 : ไม่เกิน 20 หลัก */
String promptpay_encode_tag30(String biller_id, String reference_id1, String reference_id2,
                float amount, bool multi_use=true, String terminal_label="");

/* kps_merchant_id : รหัสร้านค้าบน K+ Shop */
String promptpay_kplusshop(String kps_merchant_id, String bill_id, float amount, bool multi_use=true, String terminal_label="");

#endif //__promptpay_h__
