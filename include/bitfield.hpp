//============================ 2023 ObjectSecurity ============================//
// Zachary Washburn
// email : zachary.washburn.business@gmail.com
// tel   : +1 (571)-354-0641 
// include/bitfield.hpp
// 
// Purpose : 
//
//=============================================================================//




#ifndef IBM704_BITFIELD_HPP
#define IBM704_BITFIELD_HPP

#include <stdio.h>
#include <stdint.h>
#include <exception>
#include <bit>
#include <algorithm>
#include "helpers.hpp"

// __forceinline
#define inline_func __attribute__((always_inline))

// index off 0
// this class doesn't protect you, use at risk of your bad development
#define boff(pos) ((pos + offset) % 8)
class bitfield_raw{
public:
    bitfield_raw(){}
    bitfield_raw(unsigned char* d, size_t s ,unsigned int o = 0, bool rv=false) : data(d), size(s)
    {
        offset = rv ?  size - o : o;
    }


    unsigned char* inline_func bpos(unsigned int pos){
        return &(data[((pos + offset) / 8)]);
    }

    bool getbit(unsigned int pos){
        return *bpos(pos) & (1 << boff(pos));
    }

    void setbit(unsigned int pos, bool val){
        unsigned char* byte = bpos(pos);
        *byte = (*byte & ~((unsigned char)1 << boff(pos))) | val << (boff(pos));
    }


    int reverseoffset(int offset){
        return size - offset;
    }

    void copyfrom(bitfield_raw other, int bitpos=0, int otheroffset=0){
        // set all bytes to 0
        setall(0);
        // copy from other to where we can
        set(other.fetchdata(), std::min(other.getsize(), getsize()), bitpos,otheroffset);
    }

    void setall(bool val){
        int i = 0;
        int count = (getsize() >> 3) + ((getsize() % 8) ? 1 : 0);
        for(; i < count ; i++){
            data[i] = val ? 0xFF : 0x00;
        }
    }


    constexpr void fromnum(int64_t num, bool except=true, bool bsetsign=true){
        size_t min_bits =  ((std::bit_width ((uint64_t)abs(num)) / 2) - 1);
        if(size < min_bits && except){
            // bad addr
            printf("fromnum bad %d %d %d\n", num, size, min_bits);
            throw std::exception();
        }

        setall(0);


        // TODO : clear op
        int64_t n = abs(num);
        bool s = false;
        if(num < 0)
            s = true;
        n = num;
#if 0

        printf("%d writing to buf %d %d %d\n",
               n,
               sizeof(n) * 8,
               size,
               std::min((sizeof(n) * 8)-1,size )
        );
#endif
        set(&n, std::min((sizeof(n) * 8)-1 , size ), 0, 0);

        //print_bits (8);
        //bitfield_raw numo((unsigned char*)&n, sizeof(n) * 8);
        //numo.print_bits (8);


        if(bsetsign && s) {
            setsign ( s );
        }
        //for(num)
    }

    template<class T>
    T tonum(bool _setsign=true){
        T num = 0;
        bool s = getsign();

        setsign (false); // no matter what we need to clear the sign, certainly NOT thread-safe
        get(&num, std::min(size - 1, sizeof(T) * 8), 0,0);
        if(_setsign)
            setsign (s);
        //bitfield_raw other((unsigned char*)&num, sizeof(T) * 8);
        // other.setsign(getsign());
        num = s ? -1 * num : num;

        //print_bits (4);
        //other.print_bits (4);

        return num;
    }

    constexpr void fromoctalnum(int64_t num){
        return fromnum ( oct2dec (num));
    }

    void setsign(bool sign){
        setbit (size-1, sign);
    }

    bool getsign(){
        return getbit (size-1);
    }



    void get(void* out, unsigned int numbits, unsigned int bitpos, unsigned int outoff = 0){
        bitfield_raw other((unsigned char*)out, (size_t)numbits, outoff);
        for(unsigned int i = 0 ; i < numbits; i++){
            other.setbit (i + outoff, getbit(i + bitpos));
        }
    }

    void set(void* in, unsigned int numbits, unsigned int bitpos, unsigned int inoff = 0){
        bitfield_raw other((unsigned char*)in, (size_t)numbits, inoff);
        for(unsigned int i = 0 ; i < numbits; i++){
            // +1 (skip bit)
            setbit(i + bitpos, other.getbit (i));
        }
    }

    void print_bits(int split=8, bool back = true){
        for(unsigned int i = 0; i < size; i++){
            if(!(i % split)) {
                printf ( " " );
            }
            printf("%d", getbit(back ? (size - 1) - i : i));
        }
        printf("\n");
    }

    void print_bits_from(int split=3,int from =0, int count = 0, bool back = true){
        for(unsigned int i = from; i < std::min((size_t)(from + count),size); i++){
            if(!(i % split) && i != from) {
                printf ( " " );
            }
            printf("%d", getbit(back ? (size - 1) - i : i));
        }
        // printf("\n");
    }

    void* fetchdata(){
        return data;
    }

    size_t getsize(){
        return size;
    }

    template<class T=uint32_t, class A=uint32_t>
    void add(A other){
        T v;
        get(&v, getsize(), 0,0);
        v += other;
        fromnum (v,0,true);
    }

    template<class T=uint32_t, class A=uint32_t>
    void sub(A other){
        int64_t v = 0;
        get(&v, getsize(), 0,0);
        v -= other;

        //printf("%x after sub by %x\n", v, other);
        fromnum ((int64_t)v,0,true);
        get(&v, getsize(), 0,0);
        //v -= other;
        //printf("%x after sub by %x\n", v, other);

    }

    template<class T>
    T getas_raw(int clear=0){
        T rep = 0;
        get(&rep, std::min(sizeof(T), size), 0, 0);
        return rep;
    };

protected:
    unsigned char* data;
    unsigned int offset;
    size_t size;
};

template <int S>
class bitfield : public bitfield_raw
{
public:
    bitfield() : bitfield_raw (mem, S){
    }

protected:
    unsigned char mem[(S >> 3) + 1] = {0};
};






#endif //IBM704_BITFIELD_HPP
