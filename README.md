City Hash Ring
==============

A hash ring that uses [cityhash](http://code.google.com/p/cityhash/), currently only CityHash64 is supported.

How to use
==========

The hash ring depends on google's cityhash library, so this needs to be on your machine, typically /usr/local/lib on Ubuntu

You can add the include the header file in you code and compile as required. See hash_ring_test.cpp, which uses BOOST for its test framework.

e.g.

     g++ -o hash_ring_test -g -Wall hash/hash.cpp hash_ring/hash_ring.cpp hash_ring_test.cpp -lcityhash

The hash ring creates 100 points around the ring for each node that you create in the ring. Therefore if you add 3 nodes, you have 300 points around the ring. This aids the distribution of hashes around the ring. Test case 8 demonstrates this with adding 10K hashes to a ring with 3 nodes. The distrubution is as follows:

     10.239.0.1:2223 : 3162
     10.239.0.2:2223 : 3247
     10.239.0.3:2223 : 3591

With 5 nodes, the distribution becomes:

     10.239.0.1:2223 : 2046
     10.239.0.2:2223 : 1787
     10.239.0.3:2223 : 1973
     10.239.0.4:2223 : 2149
     10.239.0.5:2223 : 2045

License
=======

See License.md

