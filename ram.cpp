#include "ram.h"


int first_iram_add;
int last_iram_add;
int first_dram_add;
int last_dram_add;

RAM::RAM(int _progsize, int _datasize)
{
    //numbers will be rounded to nearest 100 (ceil)
    PROG_SIZE = ceil((double)_progsize/100.0)*100;
    DATA_SIZE = ceil((double)_datasize/100.0)*100;

    first_iram_add = 100;
    last_iram_add = first_iram_add + PROG_SIZE;
    first_dram_add = last_iram_add + 1000;
    last_dram_add = first_dram_add + DATA_SIZE;

    for(int i=first_iram_add; i<last_iram_add; i++){
        iRAM.insert(std::make_pair(i,""));
    }
    for(int i=first_dram_add; i<last_dram_add; i++){
        dRAM.insert(std::make_pair(i,0));
    }

}

std::string RAM::get(int memloc){
    if((memloc>=first_iram_add) & (memloc<=last_iram_add)){
        return iRAM.at(memloc);
    }else if((memloc>=first_dram_add) & (memloc<=last_dram_add)){
        return std::to_string(dRAM.at(memloc));
    }
    std::cout<<"memloc out of range";
    return "ERROR";
}

void RAM::set(int memloc, std::string content){
    if((memloc>=first_iram_add) & (memloc<=last_iram_add)){
        iRAM.at(memloc) = content;
        return;
    }else if((memloc>=first_dram_add) & (memloc<=last_dram_add)){
        dRAM.at(memloc) = atoi(content.c_str());
        return;
    }
    std::cout<<"memloc out of range";
}

void RAM::RAM_info(){
    std::cout << "PROG_SIZE: " << PROG_SIZE << "\n";
    std::cout << "DATA_SIZE: " << DATA_SIZE << "\n";

    std::cout << "iRAM: " << first_iram_add << " - " << last_iram_add << "\n";
    std::cout << "dRAM: " << first_dram_add << " - " << last_dram_add << "\n";

}
