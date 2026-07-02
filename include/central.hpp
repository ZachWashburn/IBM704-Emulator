//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/25/23 10:14 PM
// include/central.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_CENTRAL_HPP
#define IBM704_CENTRAL_HPP


#include "bitfield.hpp"
#include "instruction.hpp"
#include "magcore.hpp"

#include <map>

class cpu704{
public:
    cpu704(magcore704* magcore) : m_magcore(magcore)
    {

    }

    void handletypea(instruction704a * i);
    void handletypeb(instruction704b * i);

    void handleinstruction(word704 insn);

    void stepinstruction( );


protected:
    // storage register
    word704 sr;

    // accumulator, positions 36 and 37 are P and Q overflow
    bitfield<37> ac;

    // multiplier-quotient
    // cpy instruction uses MQ as a buffer
    // multiplier must be in MQ before mult instruction
    // quotient appears in MQ (remained in AC)
    // MQ contrains less sig hald of the product after multiplication
    // (right hand extension of AC)
    // Least sig 35 bit of results from FAD, UFA, FSB, and UFS inst.
    word704 mq;

    // instruction location counter
    // capacity of 12, 13, or 15 bits
    // depending on 4096,8192 or 32768
    // words of storage
    // always +1 from last exeucted
    // except for HTR ins.
    bitfield<15> ilc;

    // instruction register
    // instructions are loaded into
    // SR. Type A or B is determined,
    // then 18 bit positions are moved
    // here
    bitfield<18> ir;

    // used for relative indexing
    // All Type-A instructions are
    // non-indexed
    // All Type-B instructions are
    // indexable EXCEPT for instructions
    // ending in 'X'
    struct index_registers_704_t{
        // CLA B 2117 where B contains
        // 2117, references address
        // 2000
        // if multiple registers are
        // notated, then they are OR'd
        bitfield<15> a;
        bitfield<15> b;
        bitfield<15> c;
    };

    std::map<uint16_t, word704> mem;

private:
    magcore704* m_magcore;

};


#endif //IBM704_CENTRAL_HPP
