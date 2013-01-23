#include "city.h"

//
// Hash class 
//
//

class CHash
{
  public:
    uint64 calcHash64(const char* str); 
    uint128 calcHash128 (const char * str);
};
