
#include "../hash/hash.h"
#include "hash_ring.h"
#include "city.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <map>

// ----------------------------------------------------------
// Constuctor / Destructor
// ----------------------------------------------------------
CHash_Ring::CHash_Ring() {
    _replicas = 100;
}

CHash_Ring::~CHash_Ring()
{
    //std::cout << htype << " dest\n";
};
// ----------------------------------------------------------


int CHash_Ring::create(enum hash_t Hash_Type)
{
    if (Hash_Type == CITYHASH64) // || Hash_Type == CITYHASH128) 
    {
        htype = Hash_Type;
        return 0;
    }
    else
    {
        //std::cout << "ERROR: invalid hash type" << std::endl;
        return 1;
    };
};

/* inline void toString(const int value, char * output)
{
   sprintf(output, "%i", value) ;
} */


int CHash_Ring::type()
{
    return htype;
};

/*
void createNodeName(char *&node, char *&num, int i)
{
    sprintf((char*)num, "%i", i);
    std::cout << "num: " << num << std::endl;
    strcat((char*)node,(char*)num);
    std::cout << "node: " << node << std::endl;
};
*/

int CHash_Ring::addNode(char * str, int str_len)
{
    uint64 hash_value;
    char node[35];
    char num[4];

    for (int i=0; i < _replicas; i++)
    {
        num[0] = '\0';                // init
        node[0] = '\0';

        sprintf(num, "%i", i);        // concat
        strcat(node,str);
        strcat(node,num);

        hash_value = calcHash(node); 

        if (hash_value != 0)
            mapping[hash_value] = str;
    }
    return 0; 
};

uint64 CHash_Ring::calcHash(char *str)
{
    uint64 hash_value;
    
    switch (htype)
    {
        case CITYHASH64:
            hash_value = myhash.calcHash64(str); // hash the server string
            break;
        //case CITYHASH128:
        //    hash_value = myhash.calcHash128(str);
        //    break;
        default:
            hash_value = 0;
            break;
    };

    return hash_value;
};

int CHash_Ring::removeNode(char * node)
{
    return mapping.erase(calcHash(node));
};

int CHash_Ring::removeAllNodesFor(char *str)
{
    //uint64 hash_value;
    char node[35];
    char num[4];
    
    for (int i=0; i < _replicas; i++)
    {
        num[0] = '\0';             // init
        node[0] = '\0';

        sprintf(num, "%i", i);     // cocat num to str, making node
        strcat(node,str);
        strcat(node,num);

        removeNode(node);
    }
    return 0;
};

int CHash_Ring::numberOfNodes()
{
    return mapping.size();
};

uint64 CHash_Ring::findNode(char * node)
{
    uint64 hash = 0;

    hash = calcHash(node);          // calc hash
    int cnt = mapping.count(hash);
    if (cnt)      // check it exists
        return hash;
    else
        return 0;
};

void CHash_Ring::listNodeDetails()
{
    std::map<uint64, const char*>::iterator it;

    for ( it=mapping.begin() ; it != mapping.end(); it++ )
        std::cout << (*it).first << " => " << (*it).second << std::endl; // or it->first, it->second
}


char * CHash_Ring::determineServer(char * str)
{

    uint64 hash_value;
    char *node;

    hash_value = calcHash(str);

    std::map<uint64, const char*>::iterator it = mapping.lower_bound(hash_value); // set lower bounds for iterator 

    // if we're at the end of the iterator, then set a new iterator back to the start and use that node
    // i.e. we loop around the ring as the hash value is greater that the hash of the last point on
    // the ring
    if(it == mapping.end()) {
        std::map<uint64, const char*>::iterator it2 = mapping.begin(); // set to initial mapping entry 
        node = (char*)it2->second;
    }
    else
    {
        node = (char*)it->second; 
    };

    return node;
}

