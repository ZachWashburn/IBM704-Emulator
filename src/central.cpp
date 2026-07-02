//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/25/23 10:49 PM
// src/central.cpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//


#include <central.hpp>
#include "instruction.hpp"

void cpu704::handletypea ( instruction704a* i ) {

}

#define CLEARAC( ac )              ac.setbit (36, 0); \
                                                      \
                                                      \
                                    ac.setbit (37, 0);

#define EMUDBG(x) x

#ifdef _DEBUG
inline void dbg_print_typeb_instruction(word704 i){
    printf(" ");
    i.print_bits_from (3, 0, 12);
    printf(" ");
    //i.print_bits_from (15, 12,6);
    printf("XXXXXX");
    printf(" ");
    i.print_bits_from (6, 18, 3);
    printf(" ");
    i.print_bits_from (800, 21, 15);
}
#endif

void cpu704::handletypeb ( instruction704b* i ) {

    printf("Handling Opcode (%d) %d @ %x\n", i->opcode(), dec2oct(i->opcode()), ilc.tonum<uint16_t>());
    //dbg_print_typeb_instruction(*i);

    switch(i->opcode()) {
        case oct2dec ( 500 ): {
            // CLA Y +0500
            // C(Y) replaces C(AC)
            // clears bits P & Q of AC
            ac.copyfrom ( m_magcore->read ( i->addressval ( )));
            CLEARAC( ac );
        }
            break;
        case oct2dec ( 400 ): {
            // ADD Y +0400
            // Adds C(Y) to C(AC)
            // Replaces C(AC) with this sum
            ac.add< uint64_t, uint64_t > (
                    m_magcore->read (
                            i->addressval ( )
                    ).tonum< uint64_t > ( )
            );

        }
            break;
        case oct2dec ( 401 ): {
            // ADM Y +0401
            // adds the magnitude to the ac
            // C(AC) += abs(C(Y))
            word704 val = m_magcore->read (
                    i->addressval ( )
            );
            // clear the sign
            val.setsign ( 0 );
            ac.add< uint64_t, uint64_t > (
                    val.tonum< uint64_t > ( )
            );
        }
            break;
        case oct2dec ( 502 ): {
            // CLS Y +502
            // clears ac, then subs
            // (basically sets AC to
            // opposite of C(Y) )
            word704 val = m_magcore->read (
                    i->addressval ( )
            );
            val.setsign ( !val.getsign ( ));
            ac.copyfrom ( val );
            CLEARAC( ac );
        }
            break;
        case oct2dec ( 560 ): {
            // LDQ Y +560
            // C(Y) replaces (MQ)
            // C(Y) unchanged
            mq.copyfrom ( m_magcore->read ( i->addressval ( )));
        }
            break;
        case octins2dec ( -600 ): {
            // Store MQ
            // 2 STQ Y —0600
            // The c(MQ) replace the c(v). The c(MQ) are un-changed.
            m_magcore->write (i->addressval(), mq);
        }
            break;
        case oct2dec ( 601 ): {
            // Store
            // 2 STO Y 0601
            // The c(Ac)g,1.35 replace the ¢(¥)g135. The c(ac)
            // are unchanged.
            word704 w(ac);
            m_magcore->write (i->addressval(), w);
        }
            break;
        case oct2dec ( 402 ): {
            // SUB Y +402
            // subs C(Y) from C(AC)
            // and replaces C(AC) with
            // value. C(Y) replaces, overflow
            // possible
            word704 val = m_magcore->read (
                    i->addressval ( )
            );
            uint64_t v;
            val.get (&v, val.getsize(), 0,0);
            ac.sub< uint64_t, uint64_t > (
                    v
            );
        }
            break;
        case oct2dec( -400): {
            // SBM Y -0400
            // Subtracts C(Y) from C(AC)
            // Replaces C(AC) With Difference
            word704 val = m_magcore->read (
                    i->addressval ( )
            );
            uint64_t v;
            val.get (&v, val.getsize(), 0,0);
            //ac.sub< uint64_t, uint64_t > (
            //        abs(v)
            //);
        }


        default:
            EMUDBG(printf("UNKNOWN OPCODE TYPE %d\n", dec2oct (i->opcode())));
            EMUDBG(getchar());
            break;
    }

    EMUDBG(uint64_t v = 0;)
    EMUDBG(mq.get(&v, mq.getsize(),0,0));
    EMUDBG(printf("MQ VALUE NOW %lx\n", v));
    EMUDBG(ac.get(&v, ac.getsize(),0,0));
    EMUDBG(printf("AC VALUE NOW %lx\n", v));
    EMUDBG(sr.get(&v, sr.getsize(),0,0));
    EMUDBG(printf("SR VALUE NOW %lx\n", v));
    EMUDBG(getchar());

}

void cpu704::handleinstruction ( word704 i ) {
    instruction704 insn(i);

    if(insn.instype() == ins_704_t::k_A){

    } else if(insn.instype() == ins_704_t::k_B){
        instruction704b btype(insn);
        handletypeb (&btype);
    }

    // todo : set AC overflow light
}

void cpu704::stepinstruction ( ) {

    // read instruction into storage register at ilc
    sr = m_magcore->read (ilc.tonum<uint16_t>());

    handleinstruction(sr);

    // increment ip


    ilc.add<uint16_t, uint8_t>(1);
}