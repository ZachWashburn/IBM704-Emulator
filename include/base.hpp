//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/25/23 10:41 PM
// include/base.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_BASE_HPP
#define IBM704_BASE_HPP


// extra added mem locking
#define MEMLOCKING


#ifdef MEMLOCKING
#define CONCUROP( op ) op
#else
#define CONCUROP( op ) /**/
#endif


#define _DEBUG

#endif //IBM704_BASE_HPP
