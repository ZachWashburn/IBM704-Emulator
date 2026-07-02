//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/25/23 10:39 PM
// include/magcore.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_MAGCORE_HPP
#define IBM704_MAGCORE_HPP
#include "base.hpp"
#include "instruction.hpp"
#include <map>

#include <mutex>
#include <fstream>
// magnetic core
class magcore704{
public:
    word704 read(uint16_t idx){
        CONCUROP(std::lock_guard<std::mutex> _(m_lock));
        //printf("Reading Memory From Location %x\n", idx);
        word704 w;
        w.fromnum (mem[idx],false,false);

        //printf("%x Read! %lx\n", idx, mem[idx]);
        w.print_bits ();

        return w;
    }

    void write( uint16_t idx, word704 word){
        CONCUROP(std::lock_guard<std::mutex> _(m_lock));


        //mem[idx] = word;
        uint64_t v = 0;
        word.get(&v, word.getsize(), 0,0);
        mem[idx] = v;

        if(idx < 3 || idx == 0x3000){
            //printf("%x Write! %lx\n", idx, v);
            // 000 101 110 000 XXXXXX 000 011000000000000 (170003000)
        }

    }

    void loadfromfile( const char* bin ){
        std::ifstream f(bin, std::ios::in | std::ios::binary);

        f.seekg (0, std::ios::end);
        size_t s = f.tellg();
        f.seekg (0, std::ios::beg);

        for(int i = 0 ; i < s; i+=((35 >> 3) + 1)){
            word704 w;
            uint64_t val = 0;
            f.read ((char*)&val, ((35 >> 3) + 1));
            w.fromnum (val, false, false);
            // printf("Loading Address %x (%x) (read : %lx)\n", i, ((35 >> 3) + 1), val);
            write(i / ((35 >> 3) + 1), w);
        }

    }

private:
    // can be 4096, or 8192
    uint16_t m_size = 32768;
    std::map<uint16_t, uint64_t> mem;

    CONCUROP(std::mutex m_lock);


};

#endif //IBM704_MAGCORE_HPP
