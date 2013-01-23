
#include "city.h"
#include <map>
//#include <pair>
/*
 * hash_ring
 *
 * Perform consistent hashing, across servers and id's
 *
 */

class CHash_Ring
{
    private:
        //std::vector<std::pair<const char*, uint64>> nodes;
        //std::map<const char*, uint64> mapping;
        std::map<uint64, const char*> mapping;
        int htype;
        CHash myhash;
        int _replicas; // number of replicas for a node i.e. 100 instances around the ring
        uint64 calcHash(char *str);
        //void createNodeName(char *&node, char *&num, int i);

    public:
        static const uint64 MAX_ADDR_SPACE64 = 2^64;
        //static const uint128 MAX_ADDR_SPACE128 = 2^128;
        enum hash_t {
            CITYHASH64,
            CITYHASH128
        };
        int create(enum hash_t Hash_Type);
        int type();
        int numberOfNodes();
        ~CHash_Ring();
        CHash_Ring();
        int addNode(char * str, int str_len);
        int removeNode(char *node);
        int removeAllNodesFor(char *str);
        uint64 findNode(char * node);
        void listNodeDetails();
        char * determineServer(char * str);
};

