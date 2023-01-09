#ifndef _TRIDENT_UNICODE_H__
#define _TRIDENT_UNICODE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  // sprintf

/***************************************************************
 * 
 * TridentTD's UNICODE <---> UTF8 Converter (c library)
 * 
 ***************************************************************/
// for char
wchar_t       utf8_to_unicode(const char* utf8_char);
const char *  unicode_to_utf8(wchar_t unicode_char );

// for string 
//wchar_t*      UTF8_TO_UNICODE(const char* utf8_str);
size_t UTF8_TO_UNICODE(const char* utf8_str, wchar_t **unicode);
const char *  UNICODE_TO_UTF8(wchar_t* unicode_str );   // for string

// แปลงจาก uft8_str เป็น unicode_str แต่เอาแค่วามยาว
size_t UnicodeStrLength(const char* utf8_str);

// แปลง utf8 char ไปเป็น unicdoe code 2 byte
#ifndef UNICODE
#define UNICODE(utf8)     utf8_to_unicode(utf8)         // for char
#endif

// แปลง unicode code 2 byte เป็นไป utf8 char
#ifndef UTF8
#define UTF8(unicode)     unicode_to_utf8(unicode)      // for char
#endif

// แปลง unicode code 2 byte เป็นไป utf8 code 3 byte
#ifndef UTF8CODE
#define UTF8CODE(unicode)  ({ uint32_t cc=  *(uint32_t*) unicode_to_utf8(unicode); (cc&0xFF) <<16 |  (cc&0xFF00) |  (cc&0xFF0000) >> 16; })  
#endif

const char* UNICODE_HexString(const char* str);  // ดูรหัส unicode ของ string ที่ต้องการ เช่น "กขคงจฉ" -->   \u0E01\u0E02\u0E04\u0E07\u0E08\u0E09\u0E0A
const char* UTF8_HexString(const char* str);     // ดูรัหส utf8 ของ string ที่ต้องการ เช่น "กขคงจฉ" --> \xE0\xB8\x81\xE0\xB8\x82\xE0\xB8\x84\xE0\xB8\x87\xE0\xB8\x88\xE0\xB8\x89\xE0\xB8\x8A

#ifdef __cplusplus
}
#endif

// Example
//  wchar_t *unicode_str=NULL;
//  size_t unicode_len = UTF8_TO_UNICODE( "กข", &unicode_str);  // แปลง UTF8 String --> Unicode String 
//  for(int i=0; i < unicode_len; i++)
//    Serial.println( unicode_str[i],HEX);
//
//  Serial.println( UNICODE_TO_UTF8( unicode_str ));      // แปลง Unicode String --> UTF8 STring

#endif
