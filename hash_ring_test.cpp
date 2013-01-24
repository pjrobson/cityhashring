#define BOOST_TEST_MODULE hashringtest
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <map>
#include "./hash/hash.h"
#include "./hash_ring/hash_ring.h"

/* 
 * Test suite, use's BOOST
 *
 * 
 */


BOOST_AUTO_TEST_SUITE (hashringtest) 

BOOST_AUTO_TEST_CASE (_001_createHashRing128)
{
    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH128);
    BOOST_CHECK(h1.type() != 1);
    BOOST_CHECK(res == 1);
}

BOOST_AUTO_TEST_CASE (_002_createHashRing64)
{
    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH64);
    BOOST_CHECK(h1.type() == 0);
    BOOST_CHECK(res == 0);
}

BOOST_AUTO_TEST_CASE (_003_addNode)
{
    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH64);
    BOOST_CHECK(h1.type() == 0);
    BOOST_CHECK(res == 0);

    h1.addNode((char *)"10.239.0.1:2223", sizeof ("10.239.0.1:2223"));
    h1.addNode((char *)"10.239.0.2:2223", sizeof ("10.239.0.2:2223"));

    //std::cout << h1.numberOfNodes() << std::endl;
    BOOST_CHECK(h1.numberOfNodes() ==  200); 
}

BOOST_AUTO_TEST_CASE (_004_maxAddrSpace)
{
    BOOST_CHECK(CHash_Ring::MAX_ADDR_SPACE64 == (2^64));
    BOOST_CHECK(CHash_Ring::MAX_ADDR_SPACE64 != 101);
}

BOOST_AUTO_TEST_CASE (_005_removeNode)
{
    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH64);
    BOOST_CHECK(h1.type() == 0);
    BOOST_CHECK(res == 0);

    char *srv1 = (char *)"10.239.0.1:2223";
    char *srv2 = (char *)"10.239.0.2:2223";
    h1.addNode(srv1, sizeof (srv1));
    h1.addNode(srv2, sizeof (srv2));

    //h1.listNodeDetails();

    BOOST_CHECK(h1.numberOfNodes() == 200); 

    uint64 hv = h1.findNode((char *)"10.239.0.1:22230"); // hash value 10.239.0.1:2223 + 0th instance
    BOOST_CHECK(hv != 0);

    hv = h1.findNode((char *)"10.239.0.1:222345");       // hash value 10.239.0.1:2223 + 45th instance
    BOOST_CHECK(hv != 0);

    h1.removeNode((char *)"10.239.0.1:222345");          // remove 10.239.0.1:2223 + 45th instance
    BOOST_CHECK(h1.numberOfNodes() == 199);

    BOOST_CHECK(hv != 0);
    hv = h1.findNode((char *)"10.239.0.1:222345");       // no hash returned as removed
    BOOST_CHECK(hv == 0);

    hv = h1.findNode((char *)"1212.1212");              // no hash returned as invalid node name
    BOOST_CHECK(hv == 0);

}

BOOST_AUTO_TEST_CASE(_006_removeAllNodesFor)
{
    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH64);
    BOOST_CHECK(h1.type() == 0);
    BOOST_CHECK(res == 0);

    char *srv1 = (char *)"10.239.0.10:2223";
    h1.addNode(srv1, sizeof (srv1));

    uint64 hv = h1.findNode((char *)"10.239.0.10:222332"); // hash value 10.239.0.1:2223 + 32nd instance
    BOOST_CHECK(hv != 0);

    BOOST_CHECK(h1.numberOfNodes() == 100);

    h1.removeAllNodesFor(srv1);

    hv = h1.findNode((char *)"10.239.0.10:222332");       // hash value 10.239.0.1:2223 + 32nd instance
    BOOST_CHECK(hv == 0);                                 // == 0, should not exist

    BOOST_CHECK(h1.numberOfNodes() == 0);
    
}


BOOST_AUTO_TEST_CASE(_007_determineServer)
{

    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH64);
    BOOST_CHECK(h1.type() == 0);
    BOOST_CHECK(res == 0);

    char *srv1 = (char *)"10.239.0.1:2223";
    char *srv2 = (char *)"10.239.0.2:2223";
    h1.addNode(srv1, sizeof (srv1));
    h1.addNode(srv2, sizeof (srv2));

    char * node = h1.determineServer((char *)"New York");

    BOOST_CHECK(h1.numberOfNodes() == 200);
    BOOST_CHECK(node == (char *)"10.239.0.2:2223");
}

BOOST_AUTO_TEST_CASE(_008_checkServerDistribution)
{
    char node[35];
    char num[4];

    std::map<char *, int> dist;

    CHash_Ring h1;
    int res = h1.create(CHash_Ring::CITYHASH64);
    BOOST_CHECK(h1.type() == 0);
    BOOST_CHECK(res == 0);

    char *srv1 = (char *)"10.239.0.1:2223";
    char *srv2 = (char *)"10.239.0.2:2223";
    char *srv3 = (char *)"10.239.0.3:2223";


    h1.addNode(srv1, sizeof (srv1));
    h1.addNode(srv2, sizeof (srv2));
    h1.addNode(srv3, sizeof (srv3));

    char * str = (char *)"str_details_0";
    int count=0;

    dist[srv1] = 0;
    dist[srv2] = 0;
    dist[srv3] = 0;

    char * dest_node;
    try {

        for (int i=0; i< 1000; i++)
        {
            for (int j=0; j < 10; j++)
            {
                num[0] = '\0';             // init
                node[0] = '\0';

                count++;
                sprintf(num, "%i", count);     // cocat num to str, making node
                strcat(node,str);
                strcat(node,num);

                dest_node = h1.determineServer(node);     // get the server to use, for the given string
                dist[dest_node]++;                        // sum up the mapping of servers used
            }
        }
    }
    catch (int e) {
        std::cout << "Error in test 8: " << e << std::endl;
    }

    std::cout << "======================================\n";
    std::cout << "_008_checkServerDistribution =========\n";
    std::cout << "distrubution of strings across server:\n";
    std::cout << count << std::endl;
    std::cout << srv1 << " : " << dist[srv1] << std::endl;
    std::cout << srv2 << " : " << dist[srv2] << std::endl; 
    std::cout << srv3 << " : " << dist[srv3] << std::endl;
    std::cout << "======================================\n";

}

BOOST_AUTO_TEST_SUITE_END( )

