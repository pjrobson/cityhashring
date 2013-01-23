City Hash Ring
==============

A hash ring that uses cityhash, currently only CityHash64 is supported.

How to use
==========

The hash ring depends on google's cityhash library, so this needs to be on your machine, typically /usr/local/lib on Ubuntu

You can add the include the header file in you code and compile as required. See hash_ring_test, which uses BOOST.

e.g.

     g++ -o hash_ring_test -g -Wall hash/hash.cpp hash_ring/hash_ring.cpp hash_ring_test.cpp -lcityhash

