#ifndef UWCPU_H
#define UWCPU_H

#include <vector>
#include <string>

#include "ram.h"

class UWCPU
{
    RAM& ram;
public:
    int WORD_SIZE; //Ultraword will be WORD_SIZE^2
    int NUM_UREGS;
    int NUM_REGS;
    std::vector<std::vector<int>> UREG;
    std::vector<int> REG;
    int PC= 0;
    std::string IR;
    int CLOCK = 0;
    UWCPU(RAM& _ram, int _wordsize, int _uregs, int _regs) : ram(_ram){
        WORD_SIZE = _wordsize;
        NUM_UREGS = _uregs;
        NUM_REGS = _regs;

        for(int i = 0; i<NUM_UREGS; i++){
            UREG.push_back(std::vector<int>(WORD_SIZE,0));
        }

        for(int i = 0; i<NUM_REGS; i++){
            REG.push_back(0);
        }

    }
};

#endif // UWCPU_H
