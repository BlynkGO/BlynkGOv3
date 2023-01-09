/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GTable.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTABLE_H___
#define __GTABLE_H___

#include "GObject.h"

#define TYPE1  1
#define TYPE2  2
#define TYPE3  3
#define TYPE4  4

class FLOAT {
  public:
    FLOAT(float f_num, uint8_t dec=2) : f(f_num), decimal(dec) {}
    float   f;
    uint8_t decimal;
};

class GTable : public GObject {
  public:
    GTable(GWidget& parent=GScreen);
    GTable(GWidget* parent);
    virtual ~GTable();
    
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createTableStyle();
            void freeTableStyle();

    void      cells(uint16_t row_num, uint16_t col_num);
    void      rows(uint16_t num);
    uint16_t  rows();
    void      columns(uint16_t num);
    uint16_t  columns();

    GStyle    style[5]; // style[BG], style[TYPE1], style[TYPE2], style[TYPE3], style[TYPE4]


    friend class GTableCellBlock;
    friend class GTableRowBlock;
    friend class GTableRowBlock;


    //---------------------------------
    class GTableCellBlock{
      public:
        GTableCellBlock(GTable&table, uint16_t r, uint16_t c): _table(table), _r(r), _c(c) {}

        /* กำหนดค่า ให้ cell */
        void operator=(String data)       { _table.cell_value(_r,_c, data);           if(_table.cell_type(_r,_c)< TYPE3) _table.cell_type(_r,_c, (_r==0)? TYPE2 : TYPE1 ); }
        void operator=(const char* cstr)  { _table.cell_value(_r,_c, String(cstr));   if(_table.cell_type(_r,_c)< TYPE3) _table.cell_type(_r,_c, (_r==0)? TYPE2 : TYPE1 ); }
        void operator=(int num)           { _table.cell_value(_r,_c, num);            if(_table.cell_type(_r,_c)< TYPE3) _table.cell_type(_r,_c, (_r==0)? TYPE2 : TYPE1 );}
        void operator=(double num)        { _table.cell_value(_r,_c, (double) num, 2);if(_table.cell_type(_r,_c)< TYPE3) _table.cell_type(_r,_c, (_r==0)? TYPE2 : TYPE1 );}
        void operator=(FLOAT float_num)   { _table.cell_value(_r,_c, float_num.f, float_num.decimal); if(_table.cell_type(_r,_c)< TYPE3) _table.cell_type(_r,_c, (_r==0)? TYPE2 : TYPE1 ); }

        /* คืนค่า ของ cell */
        float toFloat()     { return _table.cell_value(_r,_c).toFloat(); }
        int   toInt()       { return _table.cell_value(_r,_c).toInt(); }
        const char* c_str() { return _table.cell_value(_r,_c).c_str(); }
        String toString()   { return _table.cell_value(_r,_c); }

        /* format ต่างๆ ของ cell */
        void crop(bool enable)            { _table.cell_crop(_r,_c,enable);  }
        bool crop()                       { return _table.cell_crop(_r,_c);  }
        void style_type(uint8_t type)     { _table.cell_type(_r,_c, type);   }
        uint8_t style_type()              { return _table.cell_type(_r,_c);  }
        void align(label_align_t align)   { _table.cell_align(_r,_c, align); }
        label_align_t align()             { return _table.cell_align(_r,_c); }
        void merge_right(bool enable)     { _table.cell_merge_right(_r,_c, enable); }
        bool merge_right()                { return _table.cell_merge_right(_r,_c);  }

        inline void format(label_align_t align, uint8_t type=TYPE1) {
          this->align(align);
          this->style_type(type);
        }

        inline void format(label_align_t align, bool crop, uint8_t type) {
          this->align(align);
          this->style_type(type);
          this->crop(crop);
        }
      private:
        GTable& _table;
        uint16_t _r;
        uint16_t _c;
    };


    class GTableCell {
      public:
        GTableCell() {}
        void init(GTable &table) { this->_table = &table; }
        GTableCellBlock  operator()(uint16_t r, uint16_t c) { return GTableCellBlock(*_table, r,c);  }
        GTable* _table;
    };

    class GTableRowBlock {
      public:
        GTableRowBlock(GTable&table, uint16_t r): _table(table), _r(r) {}
        void style_type(uint8_t type )     { _table.row_style(_r, type);  }
        void align(label_align_t align)    { _table.row_align(_r, align); }
      private:
        GTable& _table;
        uint16_t _r;
    };

    class GTableRow {
      public:
        GTableRow(){}
        void init(GTable &table) { this->_table = &table; }
        GTableRowBlock operator()(uint16_t r) { return GTableRowBlock(*_table, r); }
        GTable* _table;
    };

    class GTableColumnBlock {
      public:
        GTableColumnBlock(GTable&table, uint16_t c): _table(table), _c(c) {}
        void style_type(uint8_t type, bool except_header=false )     { _table.column_style(_c, type, except_header); }
        void align(label_align_t align, bool except_header=false)    { _table.column_align(_c, align,except_header); }
        void width(uint16_t w)      { _table.column_width(_c, w); }
        uint16_t width()            { return _table.column_width(_c); }
      private:
        GTable& _table;
        uint16_t _c;
    };

    class GTableColumn {
      public:
        GTableColumn(){}
        void init(GTable &table) { this->_table = &table; }
        GTableColumnBlock operator()(uint16_t c) { return GTableColumnBlock(*_table, c); }
        GTable* _table;
    };

    GTableRow    row;
    GTableColumn column;
    GTableCell   cell;


  private:
    uint16_t _row_num=3;
    uint16_t _col_num=2;
    void      recalc_cells(uint16_t row, uint16_t col);

    void      column_width(uint16_t col, uint16_t width);
    uint16_t  column_width(uint16_t col);
    void      column_align(uint16_t col, label_align_t align, bool except_header=false);
    void      column_style(uint16_t col, uint8_t style_type, bool except_header=false);

    void      row_align(uint16_t row, label_align_t align);
    void      row_style(uint16_t row, uint8_t style_type);

    void      cell_align(uint16_t row, uint16_t col, label_align_t align);
    label_align_t cell_align(uint16_t row, uint16_t col);
    void      cell_type(uint16_t row, uint16_t col, uint8_t style_type); // stype_type :TYPE1,TYPE2,TYPE3,TYPE4
    uint8_t   cell_type(uint16_t row, uint16_t col);
    void      cell_crop(uint16_t row, uint16_t col, bool crop);
    bool      cell_crop(uint16_t row, uint16_t col);
    void      cell_merge_right(uint16_t row, uint16_t col, bool enable);
    bool      cell_merge_right(uint16_t row, uint16_t col);

    void      cell_value(uint16_t row, uint16_t col, String data);
    void      cell_value(uint16_t row, uint16_t col, int data);
    void      cell_value(uint16_t row, uint16_t col, double data, uint8_t decimal=2);
    void      cell_value(uint16_t row, uint16_t col, String data                  , label_align_t align, uint8_t style_type=TYPE1);
    void      cell_value(uint16_t row, uint16_t col, int data                     , label_align_t align, uint8_t style_type=TYPE1);
    void      cell_value(uint16_t row, uint16_t col, double data, uint8_t decimal , label_align_t align, uint8_t style_type=TYPE1);
    String    cell_value(uint16_t row, uint16_t col);
};

#endif //__GTABLE_H___
