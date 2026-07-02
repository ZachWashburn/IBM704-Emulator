#include <iostream>


#include <bitfield.hpp>
#include <ibm704.hpp>
#include <instruction.hpp>




bool bitfield_test( ){
    bitfield<32> field;

    field.setbit (0, true);
    field.setbit (2, true);

    field.print_bits();

    if(*(uint32_t*)field.fetchdata() != 5)
        return false;


    if(!field.getbit (0) || field.getbit (1))
        return false;

    field.setbit (4, true);
    field.setbit (6, true);

    field.print_bits();

    uint32_t data = 0;
    field.get (&data, 4, 2, 0);
    // printf("%d\n", data);
    if(data != 5)
        return false;

    field.setbit (16, true);

    field.print_bits();

    return true;
}



int main ( ) {

    printf("YO DUMBASS DELETE");
    return 0;

    if(!bitfield_test()){
        printf("bitfield test failure!\n");
    }

    printf("%d\n", oct2dec(-707));
    printf("%d\n", dec2oct(-455));
    printf("%d\n", dec2oct(oct2dec(-707)));

    printf("%d\n", oct2dec(0707));
    printf("%d\n", dec2oct(0455));
    printf("%d\n", dec2oct(oct2dec(0707)));

    bitfield<oct2dec(707)> out;

    printf("%d\n", out.getsize());


    bitfield<oct2dec(dec2oct(707))> out2;

    printf("%d\n", out2.getsize());
#if 0
    for(int i = 0 ; i <= 50;i++) {
        int l = dec2oct(i);
        printf ( "%d\n", oct2dec(l));
        printf ( "%d\n", dec2oct(i));
    }
#endif

    bitfield<12> prefix;
    prefix.fromoctalnum (-602);
    prefix.print_bits(3);
    prefix.fromoctalnum (-760);
    prefix.print_bits(3);
    prefix.fromoctalnum (767);
    prefix.print_bits(3);



    // 111 110 111
    // 111 110 111
    // 110 000 010

    instruction704bst sensetype;
    printf("instruction type : %d\n", dec2oct(sensetype.opcode()));
    prefix.fromoctalnum (-760);
    prefix.print_bits(3);
    printf("\n");
    // print_typeb_instruction(sensetype);
    printf("\n");
    sensetype.setstioc (143);
    sensetype.print_bits (3);

    printf("Sensetype Instruction : %d ( %d )\n", sensetype.getstioc(), sensetype.getstiod());

    uint16_t addr = octins2dec (-760);

    printf("%d\n", octins2dec(-760));
    bitfield_raw cool( reinterpret_cast<unsigned char *>(&addr), 12);
    cool.print_bits ();

    ibm704 emu;
    emu.getmagcore()->loadfromfile ("compiled.bin");

    printf("%ld\n", (int64_t)0xDEAD000);
    emu.runemulation();





    std::cout << "Hello, World!" << std::endl;
    return 0;
}
