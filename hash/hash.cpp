
#include "hash.h"
#include <string.h>

using namespace std;

uint64 CHash::calcHash64 (const char * str)
{
   return CityHash64( str, strlen(str));
}

uint128 CHash::calcHash128 (const char * str)
{
   return CityHash128( str, strlen(str));
}






