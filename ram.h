#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <map>
#include <string>
class RAM
{
public:
    //number of words
    int PROG_SIZE;
    int DATA_SIZE;

    std::map<int,std::string> iRAM;
    std::map<int,int> dRAM;

    RAM(int _progsize, int _datasize);
};

#endif // RAM_H
