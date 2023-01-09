#pragma once

#include "Exception.h"
#include <string>

struct sqlite3;
struct sqlite3_blob;

namespace SQLite
{

// Forward declaration
class Database;
extern const int OK; ///< SQLITE_OK

class Blob{
public:
  Blob(Database& aDatabase, const char* table_name, const char* column_name, long long rowid);
  ~Blob();

  int   size();
  int   read(unsigned char* buffer, int buffer_len, int offset=-1);
  void  write(unsigned char* buffer, int buffer_len, int offset=-1);
  void  position( int offset);
  int   position();

private:
  sqlite3*        mpSQLite;
  sqlite3_blob*   mpBlob;
  std::string     mTableName;
  std::string     mColumnName;
  long long       mRowId;
  int             mOffset;

  inline void check(const int aRet) const
  {
      if (SQLite::OK != aRet)
      {
          throw SQLite::Exception(mpSQLite, aRet);
      }
  }
};

}  // namespace SQLite
