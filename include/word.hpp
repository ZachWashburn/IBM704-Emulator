//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/14/23 8:13 PM
// include/word.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_WORD_HPP
#define IBM704_WORD_HPP

#include <bitfield.hpp>

class word704 : public bitfield<36>
{
public:
    word704() {
        signbit = bitfield_raw(mem, 1);
    }

    template<int T=0>
    word704(bitfield_raw other) : word704() {
        copyfrom (other,0,0);
    }

    bool sign(){
        return signbit.getbit(0);
    }

    bitfield_raw octal(int pos, int bitoff = 0){
        if(pos < 0 || pos > 11 || bitoff < 0 || bitoff > 7){
            // err
            // bitoff can still cause errors btw
            throw std::exception();
        }
        return bitfield_raw(mem, 3, (pos * 8) + bitoff);
    }

    // fix for the fact that the top bit is "0" in the documentation
    bitfield_raw fetch_range(unsigned char* _m, unsigned int size, unsigned int off){
        //printf("pos %d (sz %d) becomes %d : %d - %d - %d\n", off,  size, (((getsize()) - off) - size), getsize(), off, size);
        return bitfield_raw(_m, size, (((getsize()) - off) - size) );
    }
    bitfield_raw fetch_range(unsigned int size, unsigned int off){
        return fetch_range(mem, size, off - 1);
    }
protected:
    bitfield_raw signbit;
};

#endif //IBM704_WORD_HPP
