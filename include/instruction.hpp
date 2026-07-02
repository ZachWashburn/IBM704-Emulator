//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/14/23 12:18 PM
// include/instruction.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//==================================================================================//




#ifndef IBM704_INSTRUCTION_HPP
#define IBM704_INSTRUCTION_HPP

#include "base.hpp"
#include "bitfield.hpp"
#include "word.hpp"
#include "helpers.hpp"

#define SENSETYPEOPCODE (-760)


enum ins_704_t{
    k_A = 0,
    k_B
};

class instruction704 : public word704
{
public:
    instruction704(word704& data) : instruction704() {
        data.get (mem, data.getsize(), 0, 0);
    }

    instruction704(){
        prefix = fetch_range(mem, 3, 0);
        tag = fetch_range(mem, 3, 18);
        address = fetch_range(mem, 15, 21);
    }

    ins_704_t instype( ){
        return (ins_704_t)!(prefix.getbit (34) || prefix.getbit (33));
    }

    uint16_t addressval(){
        uint16_t a;
        address.get(&a, address.getsize(), 0,0);
        return a;
    }
protected:
    bitfield_raw prefix;
    bitfield_raw tag;
    bitfield_raw address;
};

// A-Type
class instruction704a : public instruction704
{
public:
    instruction704a(){
        decrement = fetch_range(mem, 15, 3);
    }
protected:
    bitfield_raw decrement;

};
// B-Type

inline bitfield_raw typeboperation(unsigned char* mem){
    return bitfield_raw(mem, 12, ((36) - 12) - 12 );
}

inline void typebsetoperation(unsigned char* mem, int opcode){
    typeboperation(mem).fromnum (opcode);
}

inline bitfield_raw typebtag(unsigned char* mem){
    //         return bitfield_raw(_m, size, ((getsize()) - off) - size );
    return bitfield_raw(mem + 18, 3, ((36) - 18) - 3 );
}

inline void typebsettag(unsigned char* mem, int tag){
    typebtag(mem).fromnum (tag, false);
}

inline bitfield_raw typebaddress(unsigned char* mem){
    return bitfield_raw(mem + 21, 15, ((36) - 21) - 15 );
}

inline void typebsetaddress(unsigned char* mem, uint16_t address){
    typebaddress(mem).fromnum (address, false);
}

class instruction704b : public instruction704{
public:

    instruction704b() : instruction704() {
        operation = fetch_range(mem, 12, 0);
        unused = fetch_range(mem, 5, 12);
    }

    explicit instruction704b(instruction704& ot) : instruction704b() {
        ot.get (mem, ot.getsize(), 0, 0);
    }

    instruction704b(int opcode, int addr, int tagmask=0) : instruction704b() {
        create (opcode, addr, tagmask);
    }

    void create(int opcode, int addr, int tagmask=0){
        //printf("TAG\n");
        tag.fromnum (tagmask, false, false);
        //tag.print_bits (8);
        //print_bits (8);
        //printf("OP\n");
        operation.fromnum (opcode, false, false);
        //operation.print_bits (8);
        //print_bits (8);
        //printf("ADDRESS\n");
        address.fromnum (addr, false, false);
        //address.print_bits (8);
        //print_bits (8);
        //printf("DONE\n");
    }


    int16_t opcode(){
        int16_t o = 0;
        operation.get(&o, operation.getsize(), 0,0);
        return o;
    }

    instruction704b(int16_t opcode){

    }

protected:
    bitfield_raw operation;
    bitfield_raw unused;
};

// 8 bit addr mode
class instruction704b8 : public instruction704b {
public:
    instruction704b8(){
        addr8 = fetch_range(mem, 8, 28);
    }
protected:
    bitfield_raw addr8;

};

// sense type operations
class instruction704bst : public instruction704b8{
public:
    instruction704bst(){
        operation.fromoctalnum (-760);
        // typebsetoperation(data, oct2dec(-760));
    }

    void setstioc( uint16_t oct ){
        addr8.fromoctalnum (oct);
    }

    uint8_t getstioc(){
        return dec2oct(addr8.tonum<uint8_t>());
    }

    void setstiod( uint16_t oct ){
        addr8.fromnum (oct);
    }

    uint8_t getstiod(){
        return addr8.tonum<uint8_t>();
    }
private:
};



#endif //IBM704_INSTRUCTION_HPP
