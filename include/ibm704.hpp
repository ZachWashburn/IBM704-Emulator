//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/25/23 10:45 PM
// include/ibm704.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_IBM704_HPP
#define IBM704_IBM704_HPP

#include "central.hpp"
#include "magcore.hpp"


class ibm704{
public:
    ibm704(){
        magcore = new magcore704();
        cpu = new cpu704(magcore);
    }

    magcore704* getmagcore(){
        return magcore;
    }

    void runemulation(){
        while(true) {
            cpu->stepinstruction ( );
        }
    }

protected:
    magcore704* magcore;
    cpu704* cpu;

private:
};

#endif //IBM704_IBM704_HPP
