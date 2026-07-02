//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/14/23 8:27 PM
// include/helpers.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_HELPERS_HPP
#define IBM704_HELPERS_HPP

#include <math.h>

template <typename T>
constexpr T cpow(T num, unsigned int pow)
{
    return (pow >= sizeof(unsigned int)*8) ? 0 :
           pow == 0 ? 1 : num * cpow(num, pow-1);
}


template <typename T>
constexpr const T cabs(T num){
    return num * ((num>0) - (num<0));
}





constexpr inline int oct2dec(const int octalNumber)
{
    int decimalNumber = 0, i = 0, rem;
    int on = octalNumber;
    while (on != 0)
    {
        rem = on % 10;
        on /= 10;
        decimalNumber += rem * cpow<int>(8, i);
        ++i;
    }
    return decimalNumber;
}

// use for negative guys
constexpr inline const int octins2dec(const int octalNumber)
{
    int decimalNumber = 0, i = 0, rem;
    int on = cabs(octalNumber);
    while (on != 0)
    {
        rem = on % 10;
        on /= 10;
        decimalNumber += rem * cpow<int>(8, i);
        ++i;
    }

    if(octalNumber < 0){
        // set top bit
       //(((char*)&decimalNumber)[1])  |= 0b1000;
       decimalNumber |= (1 << 11);
    }

    return decimalNumber;
}

constexpr inline int dec2oct(int n)
{
    int octalNum[100];
    int i = 0;
    int ret = 0;
    while (n != 0) {
        octalNum[i] = n % 8;
        n = n / 8;
        i++;
    }
    for (int j = i - 1; j >= 0; j--) {
        ret *= 10;
        ret += octalNum[j];
    }
    return ret;
}

#endif //IBM704_HELPERS_HPP
