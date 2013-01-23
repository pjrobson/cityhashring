
#include "hash.h"
//#include "city.h"
#include <string.h>

using namespace std;

uint64 CHash::calcHash64 (const char * str)
{
   //uint64 v = CityHash64( *p, strlen(*p));
   return CityHash64( str, strlen(str));
}

uint128 CHash::calcHash128 (const char * str)
{
   //uint64 v = CityHash64( *p, strlen(*p));
   return CityHash128( str, strlen(str));
}






