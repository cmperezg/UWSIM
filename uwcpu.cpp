#include "uwcpu.h"

int WORD_SIZE; //Ultraword will be WORD_SIZE^2
int NUM_BLOCKS;
int NUM_UREGS;
int NUM_REGS;
std::vector<std::vector<int>> UREG;
std::vector<int> REG;
int PC= 0;
std::string IR;
int OVF;
int CAR;
int CLOCK = 0;

UWCPU::UWCPU(int _wordsize, int _uregs, int _regs){
    WORD_SIZE = _wordsize;
    NUM_BLOCKS = _wordsize;//Ultraword will be WORD_SIZE^2
    NUM_UREGS = _uregs;
    NUM_REGS = _regs;

    for(int i = 0; i<NUM_UREGS; i++){
        UREG.push_back(std::vector<int>(WORD_SIZE,0));
    }

    for(int i = 0; i<NUM_REGS; i++){
        REG.push_back(0);
    }
}

//1 if register number is valid
int validr(int r){
    if((r>=0) & (r<NUM_REGS)){
        return 1;
    }
    return 0;
}

int validur(int ur){
    if((ur>=0) & (ur<NUM_UREGS)){
        return 1;
    }
    return 0;
}

//BASIC REGISTER INSTRUCTIONS
//tuple<error,msg> error = 1 if error occurrs

//ADD $rd, $r1, $r2 : $rd = $r1 + $r2;
std::tuple<int,std::string> UWCPU::ADD(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(ADD): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    //overflow
    if(((REG[r2]>0) & (REG[r1] > (std::numeric_limits<float>::max()-REG[r2]))) |
          ((REG[r1]>0) & (REG[r2] > (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADD): Overflow detected";
    }

    //underflow
    if(((REG[r2]<0) & (REG[r1] < (std::numeric_limits<float>::min()-REG[r2]))) |
          ((REG[r1]<0) & (REG[r2] < (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADD): Underflow detected";
    }


    REG[rd] = REG[r1] + REG[r2];

    return std::make_tuple(0,msg);
}

//ADDI $rd, $r1, imm : $rd = $r1 + imm;
std::tuple<int,std::string> UWCPU::ADDI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(ADDI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    //overflow
    if(((imm>0) & (REG[r1] > (std::numeric_limits<float>::max()-imm))) |
          ((REG[r1]>0) & (imm > (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADDI): Overflow detected";
    }

    //underflow
    if(((imm<0) & (REG[r1] < (std::numeric_limits<float>::min()-imm))) |
          ((REG[r1]<0) & (imm < (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADDI): Underflow detected";
    }
    REG[rd] = REG[r1] + imm;

    return std::make_tuple(0,msg);
}

//ADDU $rd, $r1, $r2 : $rd = $r1 + $r2; (unsigned, no overflow)
std::tuple<int,std::string> UWCPU::ADDU(int rd, int r1, int r2){
    std::string msg = "";

    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(ADDU): Invalid register(s)";
        return std::make_tuple(1,msg);
    }


    REG[rd] = REG[r1] + REG[r2];

    return std::make_tuple(0,msg);
}

//AND $rd, $r1, $r2 : $rd = $r1 & $r2;
std::tuple<int,std::string> UWCPU::AND(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(AND): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] & REG[r2];

    return std::make_tuple(0,msg);
}

//BEQ $r1, $r2, off : if($r1 == $r2) then branch to off;
std::tuple<int,std::string> UWCPU::BEQ(int r1, int r2, int off){
    std::string msg = "";
    if((validr(r1)==0) | (validr(r2)==0)){
        msg = "(BEQ): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(off==0){
        msg = "(BEQ): Offset on branch cannot be zero";
        return std::make_tuple(1,msg);
    }
    if(REG[r1] == REG[r2]){
        PC = PC + off -1; //-1 since PC is updated before instruction is executed
    }

    return std::make_tuple(0,msg);
}


//BNE $r1, $r2, off : if($r1 != $r2) then branch to off;
std::tuple<int,std::string> UWCPU::BNE(int r1, int r2, int off){
    std::string msg = "";
    if((validr(r1)==0) | (validr(r2)==0)){
        msg = "(BNE): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(off==0){
        msg = "(BNE): Offset on branch cannot be zero";
        return std::make_tuple(1,msg);
    }
    if(REG[r1] != REG[r2]){
        PC = PC + off -1; //-1 since PC is updated before instruction is executed
    }

    return std::make_tuple(0,msg);
}

//J add: PC = add
std::tuple<int,std::string> UWCPU::J(int add){
    std::string msg = "";

    PC = add;

    return std::make_tuple(0,msg);
}

//JR add: PC = $r1
std::tuple<int,std::string> UWCPU::JR(int r1){
    std::string msg = "";
    if(validr(r1)==0){
        msg = "(BNE): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    PC = REG[r1];

    return std::make_tuple(0,msg);
}

//LW $rd, $r1, off :   $rd = MEM[$r1 + off];
std::tuple<int,std::string> UWCPU::LW(int rd, int r1, int off, RAM& _ram){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(LW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = atoi((_ram.get(REG[r1]+off)).c_str());

    return std::make_tuple(0,msg);
}

//LUW $rd, $u1: $rd = $u1[0]
std::tuple<int,std::string> UWCPU::LUW(int rd, int u1){
    std::string msg = "";
    if(validr(rd)==0){
        msg = "(LUW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(validur(u1)==0){
        msg = "(LUW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = UREG[u1][0];

    return std::make_tuple(0,msg);
}

//NOT $rd,$r1: $rd = ~$r1
std::tuple<int,std::string> UWCPU::NOT(int rd, int r1){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(UWNOT): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = ~(REG[r1]);

    return std::make_tuple(0,msg);
}

//OR $rd, $r1, $r2 : $rd = $r1 & $r2;
std::tuple<int,std::string> UWCPU::OR(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(OR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] | REG[r2];

    return std::make_tuple(0,msg);
}

//SL $rd, $r1, imm : $rd = $r1 << imm;
std::tuple<int,std::string> UWCPU::SL(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(SL): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(imm >= WORD_SIZE){
        msg = "(SL): Shift larger than WORD_SIZE is invalid";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] << imm;

    return std::make_tuple(0,msg);
}

//SLV $rd, $r1, $r2 : $rd = $r1 << imm;
std::tuple<int,std::string> UWCPU::SLV(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SLV): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] << REG[r2];

    return std::make_tuple(0,msg);
}

//SR $rd, $r1, imm : $rd = $r1 >> imm;
std::tuple<int,std::string> UWCPU::SR(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(SR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(imm >= WORD_SIZE){
        msg = "(SR): Shift larger than WORD_SIZE is invalid";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] >> imm;

    return std::make_tuple(0,msg);
}

//SRV $rd, $r1, $r2 : $rd = $r1 >> imm;
std::tuple<int,std::string> UWCPU::SRV(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SRV): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] >> REG[r2];

    return std::make_tuple(0,msg);
}

//SUB $rd, $r1, $r2 : $rd = $r1 - $r2;
std::tuple<int,std::string> UWCPU::SUB(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SUB): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    //overflow
    if(((REG[r2]<0) & (REG[r1] > (std::numeric_limits<float>::max()+REG[r2]))) |
          ((REG[r1]<0) & (REG[r2] > (std::numeric_limits<float>::max()+REG[r1])))){
        OVF = 1;
        msg = "(SUB): Overflow detected";
    }

    //underflow
    if(((REG[r2]>0) & (REG[r1] < (std::numeric_limits<float>::min()+REG[r2]))) |
          ((REG[r1]>0) & (REG[r2] < (std::numeric_limits<float>::min()+REG[r1])))){
        OVF = 1;
        msg = "(SUB): Underflow detected";
    }


    REG[rd] = REG[r1] - REG[r2];

    return std::make_tuple(0,msg);
}


//SUBU $rd, $r1, $r2 : $rd = $r1 - $r2; (unsigned, no overflow)
std::tuple<int,std::string> UWCPU::SUBU(int rd, int r1, int r2){
    std::string msg = "";

    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SUBU): Invalid register(s)";
        return std::make_tuple(1,msg);
    }


    REG[rd] = REG[r1] - REG[r2];

    return std::make_tuple(0,msg);
}

//SW $r1, $r2, off : MEM[$r1 + off] = $r2;
std::tuple<int,std::string> UWCPU::SW(int r2, int r1, int off, RAM& _ram){
    std::string msg = "";
    if((validr(r1)==0) | (validr(r2)==0)){
        msg = "(SW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    _ram.set(REG[r1]+off, std::to_string(REG[r2]));

    return std::make_tuple(0,msg);
}

//UW REGISTER INSTRUCTIONS
//tuple<error,msg> error = 1 if error occurrs
//No UW instruction considers overflow or carry

//UWADD $ud, $u1, $rs : $rd = $r1 + $rs;
std::tuple<int,std::string> UWCPU::UWADD(int ud, int u1, int u2){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWADD): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    //missing carry management (not for flags, but because UW should know no block boundaries)
    for(int i = NUM_BLOCKS-1; i>=0; i--){
        UREG[ud][i] = UREG[u1][i] + UREG[u2][i];
    }
    return std::make_tuple(0,msg);
}

//UWAND $ud, $u1, $rs : $ud = $u1 & $us;
std::tuple<int,std::string> UWCPU::UWAND(int ud, int u1, int u2){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWAND): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i = NUM_BLOCKS-1; i>=0; i--){
        UREG[ud][i] = UREG[u1][i] & UREG[u2][i];
    }
    return std::make_tuple(0,msg);
}

//UWBEQ $u1, $u2, off : if($r1 == $r2) then branch to off;
std::tuple<int,std::string> UWCPU::UWBEQ(int u1, int u2, int off){
    std::string msg = "";
    if((validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWBEQ): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(off==0){
        msg = "(UWBEQ): Offset on branch cannot be zero";
        return std::make_tuple(1,msg);
    }
    bool allequal = false;
    for(int i =0; i<NUM_BLOCKS; i++){
        if(UREG[u1][i] == UREG[u2][i]){
            allequal = true;
        }else{
            allequal = false;
            break;
        }
    }

    if(allequal){
        PC = PC + off -1; //-1 since PC is updated before instruction is executed
    }

    return std::make_tuple(0,msg);
}


//UWBNE $u1, $u2, off : if($u1 != $u2) then branch to off;
std::tuple<int,std::string> UWCPU::UWBNE(int u1, int u2, int off){
    std::string msg = "";
    if((validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWBNE): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(off==0){
        msg = "(UWBNE): Offset on branch cannot be zero";
        return std::make_tuple(1,msg);
    }
    bool allequal = false;
    for(int i =0; i<NUM_BLOCKS; i++){
        if(UREG[u1][i] == UREG[u2][i]){
            allequal = true;
        }else{
            allequal = false;
            break;
        }
    }

    if(!allequal){
        PC = PC + off -1; //-1 since PC is updated before instruction is executed
    }

    return std::make_tuple(0,msg);
}


//UWCOM $ud, $u1 : packs first bit of each block of $u1 into the first block of $ud
std::tuple<int,std::string> UWCPU::UWCOM(int ud, int u1){
    std::string msg = "";
    if((validur(ud)==0) | (validr(u1)==0)){
        msg = "(UWCOM): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(ud==u1){
        msg = "(UWCOM): Cannot compress onto same register";
        return std::make_tuple(1,msg);
    }
    unsigned int firstmask = 1<<(WORD_SIZE-1);
    for(int i=0; i<NUM_BLOCKS; i++){
        unsigned int maskedn = UREG[u1][i]&firstmask;
        unsigned int thebit = maskedn >> (WORD_SIZE-1);
        UREG[ud][0] = UREG[ud][0] << 1;
        UREG[ud][0] = UREG[ud][0] | thebit;
    }

    return std::make_tuple(0,msg);

}

//UWEXP $ud, $u1 : spreads first block of $u1 into the first bit of each block of $ud
std::tuple<int,std::string> UWCPU::UWEXP(int ud, int u1){
    std::string msg = "";
    if((validur(ud)==0) | (validr(u1)==0)){
        msg = "(UWEXP): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(ud==u1){
        msg = "(UWEXP): Cannot expand onto same register";
        return std::make_tuple(1,msg);
    }
    unsigned int shiftcount = 0;
    for(int i=NUM_BLOCKS-1; i>=0; i--){
        unsigned int mask = (unsigned int)1<<shiftcount;
        unsigned int maskedn = UREG[u1][0]&mask;
        unsigned int thebit = maskedn >> shiftcount;
        UREG[ud][i] = thebit<<(WORD_SIZE-1);
    }

    return std::make_tuple(0,msg);

}

//UWLB $ud,j,base:  $ud[j] = MEM[base+j]
std::tuple<int,std::string> UWCPU::UWLB(int ud, int j, int base, RAM& _ram){
    std::string msg = "";
    if(validur(ud)==0){
        msg = "(UWLB): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if((j<0) | (j>= NUM_BLOCKS)){
       msg =  "(UWLB): Invalid block index";
       return std::make_tuple(1,msg);
    }

    UREG[ud][j] = atoi((_ram.get(base+j)).c_str());

    return std::make_tuple(0,msg);
}

//UWLC $ud[j] = MEM[base+$ud[j]] (for each j in NUM_BLOCKS in parallel)
std::tuple<int,std::string> UWCPU::UWLC(int ud, int base, RAM& _ram){
    std::string msg = "";
    if(validur(ud)==0){
        msg = "(UWLC): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i = 0; i<NUM_BLOCKS; i++){
        UREG[ud][i] = atoi((_ram.get(base+UREG[ud][i])).c_str());
    }

    return std::make_tuple(0,msg);
}

//UWLR $ud, $r1: $ud[0] = $r1
std::tuple<int,std::string> UWLR(int ud, int r1){
    std::string msg = "";
    if(validur(ud)==0){
        msg = "(UWLR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(validr(r1)==0){
        msg = "(UWLR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    UREG[ud][0] = REG[r1];
    return std::make_tuple(0,msg);
}

//UWLUW $ud,base: $ud,base:  $ud[j] = MEM[base+j] (for each j in NUM_BLOCKS in parallel)
std::tuple<int,std::string> UWCPU::UWLUW(int ud, int base, RAM& _ram){
    std::string msg = "";
    if(validur(ud)==0){
        msg = "(UWLUW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i = 0; i<NUM_BLOCKS; i++){
        UREG[ud][i] = atoi((_ram.get(base+i)).c_str());
    }

    return std::make_tuple(0,msg);
}

//UWNOT $ud,$u1: $ud = ~$u1
std::tuple<int,std::string> UWCPU::UWNOT(int ud, int u1){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0)){
        msg = "(UWNOT): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i = NUM_BLOCKS-1; i>=0; i--){
        UREG[ud][i] = ~(UREG[u1][i]);
    }
    return std::make_tuple(0,msg);
}

//UWOR $ud, $u1, $rs : $rd = $r1 | $rs;
std::tuple<int,std::string> UWCPU::UWOR(int ud, int u1, int u2){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWOR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i = NUM_BLOCKS-1; i>=0; i--){
        UREG[ud][i] = UREG[u1][i] | UREG[u2][i];
    }
    return std::make_tuple(0,msg);
}

//UWSB $ud,j,base:  MEM[base+j] = $ud[j]
std::tuple<int,std::string> UWCPU::UWSB(int u1, int j, int base, RAM& _ram){
    std::string msg = "";
    if(validur(u1)==0){
        msg = "(UWSB): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if((j<0) | (j>= NUM_BLOCKS)){
       msg =  "(UWSB): Invalid block index";
       return std::make_tuple(1,msg);
    }

    _ram.set(base+j, std::to_string(UREG[u1][j]));

    return std::make_tuple(0,msg);
}

//UWSC $ud,$u1,base:  MEM[base+$u2[j]] = $u1[j] (for all j in NUM_BLOCKS in parallel)
std::tuple<int,std::string> UWCPU::UWSC(int u1,int u2, int base, RAM& _ram){
    std::string msg = "";
    if((validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWSC): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i =0; i<NUM_BLOCKS; i++){
        _ram.set(base+UREG[u2][i], std::to_string(UREG[u1][i]));
    }


    return std::make_tuple(0,msg);
}

//UWSL $ud, $u1, imm : $ud = $u1 << imm;
std::tuple<int,std::string> UWCPU::UWSL(int ud, int u1, int imm){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0)){
        msg = "(UWSL): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(imm >= (WORD_SIZE*NUM_BLOCKS)){
        msg = "(UWSL): Shift larger than register size is invalid";
        return std::make_tuple(1,msg);
    }

    int bts = floor((float)imm/float(WORD_SIZE));
    int split = imm%WORD_SIZE;
    unsigned int aux1 = 0;
    unsigned int aux2 = 0;
    int i = bts;
    int j = 0;

    while(i<NUM_BLOCKS){
        aux1 = 0;
        aux1 = UREG[u1][i]<<split;
        if(((i+1)<NUM_BLOCKS)&&(imm!=WORD_SIZE)){
            aux2 = UREG[u1][i]>>(WORD_SIZE-split);
        }else{
            aux2 = 0;
        }
        UREG[ud][j] = aux1|aux2;
        j++;i++;
    }

    return std::make_tuple(0,msg);
}

//UWSR $ud, $u1, imm : $ud = $u1 >> imm;
std::tuple<int,std::string> UWCPU::UWSR(int ud, int u1, int imm){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0)){
        msg = "(UWSR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(imm >= (WORD_SIZE*NUM_BLOCKS)){
        msg = "(UWSR): Shift larger than register size is invalid";
        return std::make_tuple(1,msg);
    }

    int bts = floor((float)imm/(float)WORD_SIZE);
    int split = imm%WORD_SIZE;
    unsigned int aux1 = 0;
    unsigned int aux2 = 0;
    int i = bts;
    int j = 0;

    while(i<NUM_BLOCKS){
        aux1 = UREG[u1][j]>>split;
        UREG[ud][i] = aux1|aux2;
        if(split!=0){
            aux2 = UREG[u1][j]<<(WORD_SIZE-split);
        }
        i++;j++;
    }

    return std::make_tuple(0,msg);
}

//UWSSQ $ud, imm1 : spreads sequence of imm and adds it to address in first block of $ud. stores in $ud.
//ex: UWSSQ $ud,4
//$ud[0] = 2, $ud[1] = 6, $ud[2] = 10, $ud[3] = 14...
//imm can be 0,1,2,4,8,16 or 32
std::tuple<int,std::string> UWCPU::UWSSQ(int ud, int imm){
    std::string msg = "";
    if(validur(ud)==0){
        msg = "(UWSSQ): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    //since instruction is meant for packing or replicating, only values that make sense are:
    //0,1,2,4,8,16,32. Limiting the values also makes the instruction feasible

    if((imm!=0) & (imm!=1) & (imm!=2) & (imm!=4) & (imm!=8) & (imm!=16) & (imm!=32)){
        msg = "(UWSSQ): Invalid sequence (must be a factor of WORD_SIZE or zero)";
        return std::make_tuple(1,msg);
    }

    int base = UREG[ud][0];
    for(int i=1; i<NUM_BLOCKS; i++){
        UREG[ud][i] = base + (i*imm);
    }

    return std::make_tuple(0,msg);
}


//UWSUB $ud, $u1, $rs : $rd = $r1 - $rs;
std::tuple<int,std::string> UWCPU::UWSUB(int ud, int u1, int u2){
    std::string msg = "";
    if((validur(ud)==0) | (validur(u1)==0) | (validur(u2)==0)){
        msg = "(UWADD): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    //missing carry management (not for flags, but because UW should know no block boundaries)
    for(int i = NUM_BLOCKS-1; i>=0; i--){
        UREG[ud][i] = UREG[u1][i] - UREG[u2][i];
    }
    return std::make_tuple(0,msg);
}

//UWSUW $ud,base:  MEM[base+j] = $u1[j] (for all j in NUM_BLOCKS in parallel)
std::tuple<int,std::string> UWCPU::UWSUW(int u1,int base, RAM& _ram){
    std::string msg = "";
    if(validur(u1)==0){
        msg = "(UWSC): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    for(int i =0; i<NUM_BLOCKS; i++){
        _ram.set(base+i, std::to_string(UREG[u1][i]));
    }


    return std::make_tuple(0,msg);
}
