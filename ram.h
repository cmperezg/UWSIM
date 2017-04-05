#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <map>
#include <string>
#include <cmath>
class RAM
{
 public:
    //number of words
    int PROG_SIZE;
    int DATA_SIZE;

    //program instructions
    std::map<int,std::string> iRAM;
    //data
    std::map<int,int> dRAM;

    RAM(int _progsize, int _datasize);
    std::string get(int memloc);
    void set(int memloc, std::string content);
    void RAM_info();
};

#endif // RAM_H
