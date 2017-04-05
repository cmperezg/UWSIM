#ifndef UWCPU_H
#define UWCPU_H

#include <vector>
#include <string>
#include <tuple>
#include <limits>

#include "ram.h"

class UWCPU
{
public:
    UWCPU(int _wordsize, int _uregs, int _regs);

    //instructions
    std::tuple<int,std::string> ADD(int rd, int r1, int r2);
    std::tuple<int,std::string> ADDI(int rd, int r1, int imm);
    std::tuple<int,std::string> ADDU(int rd, int r1, int r2);
    std::tuple<int,std::string> AND(int rd, int r1, int r2);
    std::tuple<int,std::string> BEQ(int r1, int r2, int off);
    std::tuple<int,std::string> BNE(int r1, int r2, int off);
    std::tuple<int,std::string> J(int add);
    std::tuple<int,std::string> JR(int r1);
    std::tuple<int,std::string> LW(int rd, int r1, int off, RAM& _ram);
    std::tuple<int,std::string> LUW(int rd, int u1);
    std::tuple<int,std::string> NOT(int rd, int r1);
    std::tuple<int,std::string> OR(int rd, int r1, int r2);
    std::tuple<int,std::string> SL(int rd, int r1, int imm);
    std::tuple<int,std::string> SLV(int rd, int r1, int r2);
    std::tuple<int,std::string> SR(int rd, int r1, int imm);
    std::tuple<int,std::string> SRV(int rd, int r1, int r2);
    std::tuple<int,std::string> SUB(int rd, int r1, int r2);
    std::tuple<int,std::string> SUBU(int rd, int r1, int r2);
    std::tuple<int,std::string> SW(int r1, int r2, int off, RAM& _ram);

    std::tuple<int,std::string> UWADD(int ud, int u1, int u2);
    std::tuple<int,std::string> UWAND(int ud, int u1, int u2);
    std::tuple<int,std::string> UWBEQ(int u1, int u2, int off);
    std::tuple<int,std::string> UWBNE(int u1, int u2, int off);
    std::tuple<int,std::string> UWCOM(int ud, int u1);
    std::tuple<int,std::string> UWEXP(int ud, int u1);
    std::tuple<int,std::string> UWLB(int ud, int j, int base, RAM& _ram);
    std::tuple<int,std::string> UWLC(int ud, int base, RAM& _ram);
    std::tuple<int,std::string> UWLR(int ud, int r1);
    std::tuple<int,std::string> UWLUW(int ud, int base, RAM& _ram);
    std::tuple<int,std::string> UWNOT(int ud, int u1);
    std::tuple<int,std::string> UWOR(int ud, int u1, int u2);
    std::tuple<int,std::string> UWSB(int ud, int j, int base, RAM& _ram);
    std::tuple<int,std::string> UWSC(int u1, int u2, int base, RAM& _ram);
    std::tuple<int,std::string> UWSL(int ud, int u1, int imm);
    std::tuple<int,std::string> UWSR(int ud, int u1, int imm);
    std::tuple<int,std::string> UWSSQ(int ud, int imm);
    std::tuple<int,std::string> UWSUB(int ud, int u1, int u2);
    std::tuple<int,std::string> UWSUW(int u1, int base, RAM& _ram);
};

#endif // UWCPU_H
