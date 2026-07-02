//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : Project
// dev env : CLion
// DATE    : 11/28/23 5:47 PM
// src/assembler.cpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//


#include <base.hpp>
#include <instruction.hpp>
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>


#define NOINSTRUCTION 5
#define OPLOOKUPFAILURE (-1)
#define PARSEFAILURE (-2)
#define SYMBOLLOOKUPFAILURE (-3)
#define TAGPARSEFAILURE (-4)
#define ADDRESSPAREFAILURE (-6)
#define DESCRIPTORFAILURE (-7)
#define DATAPARSEFAILURE (-8)

char lasterrorstr[8192];

// opcode lookup table
std::map<std::string, uint16_t> typeblookup {
        {"CLA", oct2dec(500)},
        {"ADD", oct2dec(400)},
        {"ADM", oct2dec(401)},
        {"CLS", oct2dec(401)},
        {"SUB", oct2dec(402)},
        {"SBM", octins2dec(-400)},
        {"SLW", oct2dec(602)},
        {"ALS", oct2dec(767)},
        {"LDQ", oct2dec(560)},
        {"STO", oct2dec(601)},
        {"STQ", octins2dec(-600)}
};


const char* codetoerr(int err){
    switch(err){
        case NOINSTRUCTION:
            return "No Instruction";
        case OPLOOKUPFAILURE:
            return "Opcode Lookup Failure";
        case ADDRESSPAREFAILURE:
            return "Address Parse Failure";
        case DESCRIPTORFAILURE:
            return "Generic Descriptor Error";
        case DATAPARSEFAILURE:
            return "Data Parse Failure";
        default:
            return "Generic Failure";
    }
}

int textoccurance(const char* str, const char c){
    for(int i = 0; str[i] != 0;i++)
        if(str[i] == c)
            return i;
    return -1;
}

int nonspace(const char* str){
    for(int i = 0; str[i] != 0;i++)
        if(str[i] != ' ' && str[i] != '\n' && str[i] != '\r' && str[i] != '\t')
            return i;
    return -1;
}

int findsemi(const char* str){
    return textoccurance(str, ';');
}

int replacesemiwithzero(char* str){
    int offset = findsemi(str);
    if( offset == -1 )
        return -1;
    str[offset] = 0x00;
    return 0;
}

int countnonspaceorcomma(const char* str){
    int num = 0;
    for(int i = 0 ; str[i] != 0;i++){
        if(str[i] != ' ' && str[i] != ',')
            num++;
    }
    return num;
}

int commaloc(const char* d){
    return textoccurance (d, ',');
}

int dotloc(const char* d){
    return textoccurance (d, '.');
}

int semiloc(const char* d){
    return textoccurance (d, ';');
}

bool colonloc( const char* d ){
    return textoccurance (d, ':');
}

bool spaceloc( const char* d ){
    return textoccurance (d, ' ');
}

bool newlineloc( const char* d ){
    return textoccurance (d, '\n');
}

bool islinecomment( const char* str ){
    if(nonspace(str) == semiloc(str))
        return true;
    return false;
}

bool isdescriptor(const char* str){
    return nonspace (str) == dotloc(str);
}



int findspaceorcomma(const char* str){
    for(int i = 0; str[i] != 0;i++)
        if(str[i] == ' ' || str[i]==',')
            return i;
    return -1;
}



int findspaceorcolon(const char* str){
    for(int i = 0; str[i] != 0;i++)
        if(str[i] == ' ' || str[i]==':')
            return i;
    return -1;
}

int findspaceorcommaorend(const char* str){
    int r = findspaceorcomma (str);
    return r == -1 ? strlen(str) : r;
}

int gettagnumber(char* c){
#if 0
    int o = commaloc (c);
    if( o == -1)
        return 0;
    c += o;
#endif
    int j = findspaceorcomma (c);
    if( j == -1 )
        return 0;

    c += j;

    uint8_t mask = 0;
    for(int i = 0 ; c[i] != 0; i++){
        if (c[i] == ' ' || c[i] == ',')
            continue;

        char r = toupper(c[i]);
        if( r  == 'A'){
            mask |= 0b001;
        } else if ( r == 'B'){
            mask |= 0b010;
        } else if ( r == 'C' ){
            mask |= 0b100;
        }
    }
    return mask;
}




uint16_t typebopcode(const char* data, size_t len){
    if( len < 3 )
        return UINT16_MAX;

    char opstr[4] = {0};
    memcpy(opstr, data, 3);

    if( typeblookup.find(std::string(opstr)) == typeblookup.end() )
        return UINT16_MAX;

    return typeblookup[std::string(opstr)];
}

void print_typeb_instruction(word704 i){
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




int processinstruction(const char* data, word704& out,int lnum=0, const std::map<std::string, uint16_t>* ymbollookup = nullptr){

    size_t len = strlen(data);
    char* end = const_cast<char *>(len + data);

    int semiloc = findsemi(data);
    int spaceloc = nonspace(data);

    if(len == 0 || spaceloc < 0 || semiloc != -1 && spaceloc >= semiloc){
        return NOINSTRUCTION;
    }

    replacesemiwithzero ((char*)data);

    // todo : typeb/typea deduction
    char* p = const_cast<char *>(&data[spaceloc]);

    uint16_t opcode = typebopcode (p, len - spaceloc );

    if(opcode == UINT16_MAX)
        return OPLOOKUPFAILURE;

    // walk to space
    int off = findspaceorcomma (p);

    if(off == -1)
        return ADDRESSPAREFAILURE;

    p += off;
    //printf("%s\n", p);


    // set address
    int nextloc = nonspace (p);

    if( nextloc < 0)
        return ADDRESSPAREFAILURE;

    p += nextloc;

    int addrend = findspaceorcommaorend (p);

    if(addrend == 0 || addrend > 8)
        return ADDRESSPAREFAILURE;

    char _address[8] = {0};
    char* address = _address;
    memcpy(address, p, addrend);

    bool hex = true;
    if(address[1] == 'x')
        address += 2;
    else if (addrend > 6)
        return ADDRESSPAREFAILURE;
    else
        hex = false;

    if(addrend == 8){
        if(address[0] > '7') {
            //printf(_address);
            snprintf(lasterrorstr, sizeof(lasterrorstr), "Address exceeds avaliable memory bounds %s > 0x7FFF", address);
            return ADDRESSPAREFAILURE;
        }
    }

    //printf("addr:%s\n", address);

    uint32_t _addr = strtoul(address, nullptr, hex ? 16 : 10);

    if(_addr > 0x7FFF){
        snprintf(lasterrorstr, sizeof(lasterrorstr), "Address exceeds avaliable memory bounds %x > 0x7FFF", _addr);
        return ADDRESSPAREFAILURE;
    }
    uint16_t addr = static_cast<uint16_t>(_addr);

    p += addrend;
    int tagpos = 0;
    uint8_t mask = 0;
    if( p == end )
        goto complete_function;

    // printf("mask time!\n");

    // do tag parsing
    tagpos = findspaceorcomma ( p);

    if(tagpos == -1)
        goto complete_function;


    p += tagpos;
    // printf("mask : %s\n", p);
    mask = gettagnumber (p);





complete_function:

    // set opcode
#if 0
    typebsetoperation((unsigned char*)out.fetchdata(), opcode);
    typebsettag((unsigned char*)out.fetchdata(), mask);
    typebsetaddress((unsigned char*)out.fetchdata(), addr);
#endif
    instruction704b i(opcode, addr, mask);

    out.copyfrom (i);

    bitfield<12> prefix;
    prefix.fromnum (opcode);

    int octval = dec2oct(opcode);

    print_typeb_instruction(out);
    uint64_t nval = 0;// = *(uint64_t*)i.fetchdata()
    i.get(&nval, i.getsize(), 0,0);
    printf(" (%08lx)\t %s opcode : %d (%d) addr : %x mask %x bin", nval, data, octval, opcode, addr, mask);
    printf("\n");
    //print_typeb_instruction(i);
    //printf("\n");
    //prefix.print_bits (3);



    //instruction704b inst;
    return 0;
}


uint64_t processaddress (const char* data, int& outoff, bool& noaddr, bool& failure, bool limit=true ){
    int len = strlen (data);

    if(len == 0) {
        noaddr = true;
        return 0;
    }

    int ns = nonspace (data);

    if(ns < 0 || ns == len && data[1] == ' '){
        // printf("Non space exceeds %d %d %s\n", ns, len, data);
        noaddr = true;
        return 0;
    }

    char* p = (char*)data + ns;
    int addrend = findspaceorcommaorend (p);

    if(addrend == 0 || ((addrend > 8) && limit)) {
        printf("no addr %s, %d (%s) %d\n", p, addrend, data, limit);
        snprintf(lasterrorstr, sizeof(lasterrorstr), "Address exceeds avaliable memory bounds UINT_MAX%d > 0x7FFF", addrend);
        noaddr = true;
        failure = true;
        // return ADDRESSPAREFAILURE;
    }

    char _address[8] = {0};
    char* address = _address;
    memcpy(address, p, addrend);

    bool hex = true;
    if(address[1] == 'x')
        address += 2;
    else if (addrend > 6 && limit) {
        failure = true;
        return ADDRESSPAREFAILURE;
    }
    else
        hex = false;

    if(addrend == 8 && limit){
        if(address[0] > '7') {
            //printf(_address);
            failure = true;
            snprintf(lasterrorstr, sizeof(lasterrorstr), "Address exceeds avaliable memory bounds %s > 0x7FFF", address);
            return ADDRESSPAREFAILURE;
        }
    }

    outoff = addrend;
    printf("addr:%s\n", address);

    // snprintf(lasterrorstr, sizeof(lasterrorstr), "Address exceeds avaliable memory bounds %s > 0x7FFF", address);
    uint64_t _addr = strtoull(address, nullptr, hex ? 16 : 10);

    if(_addr > 0x7FFF && limit){
        snprintf(lasterrorstr, sizeof(lasterrorstr), "Address exceeds avaliable memory bounds %lx > 0x7FFF", _addr);
        failure = true;
        return ADDRESSPAREFAILURE;
    }
    //uint16_t addr = static_cast<uint16_t>(_addr);



    return _addr;
}


int processdescriptor(const char* data,  bool& codeset, bool& dataset, bool& addrset, uint16_t& address, int lnum=0, const std::map<std::string, uint16_t>* symbollookup = nullptr){

    int dl = dotloc(data);
    int len = strlen(data);

    if(dl < 0 || len > 4096) {
        snprintf(lasterrorstr, sizeof(lasterrorstr), "line %d, descriptor line to long or missing dot, dot loc : %d, len %d", lnum, dl, len);
        return DESCRIPTORFAILURE;
    }

    int enlc = findspaceorcolon (data+dl);
    int s = spaceloc(data+dl);

    printf("%d %d %d %d\n", dl, enlc, s, len);
    printf("%s\n", data);
    printf("%s\n", data+dl);
    if(enlc < 0 || enlc == dl + 1 || data[dl+1] == ' ') {
        int nlloc = -1; // newlineloc (data+dl);
        int semloc= findsemi(data+dl);

        if((nlloc < 0 || nlloc == dl + 1) && (semloc < 0 || semloc == dl + 1) && (len -1) == 0 || data[dl+1] == ' '){
            snprintf ( lasterrorstr, sizeof (lasterrorstr),
                       "line %d, descriptor unnamed or missing termination dl %d enlc %d nlloc %d semloc %d", lnum, dl, enlc, nlloc, semloc );
            return DESCRIPTORFAILURE;
        }

        //len -= 1;
        printf("line %d, descriptor unnamed or missing termination dl %d enlc %d nlloc %d semloc %d len %d", lnum, dl, enlc, nlloc, semloc, len );
        enlc = std::min(std::min(*(unsigned*)&nlloc, *(unsigned*)&semloc), *(unsigned*)(&len));

        //len +=1;

    }


    char desc[4096] = {0};
    printf("%s\n", &data[dl+1]);
    printf("%d %d\n", dl+1, enlc);
    memcpy(desc, &data[dl+1], enlc-1);

    printf("Descriptor {%s}\n", desc);



    char* p = (char*)data + dl + enlc + 1;

    if(!strcasecmp(desc, "code")){
        printf("Code Region Detected!\n");

        codeset = true;

        if(dl + enlc + 1 == len)
            return 0;

        // uint16_t processaddress (const char* data, int& outoff, bool& noaddr, bool& failure )
        bool noaddr = false;
        bool failure = false;
        int outoff = 0;
        uint16_t addr = static_cast<uint16_t>(processaddress (p,std::ref(outoff), std::ref(noaddr), std::ref(failure)));

        if(failure){
            return ADDRESSPAREFAILURE;
        }

        if(!noaddr){
            printf("DD Address : %x\n", addr);
            addrset = true;
            address = addr;
        } else {
            printf("WARNING : NO CODE SEGMENT ADDRESS %s\n", p);
        }


    } else if(!strcasecmp(desc, "data")){
        printf("DATA Region Detected!\n");

        dataset = true;

        if(dl + enlc + 1 == len)
            return 0;

        // uint16_t processaddress (const char* data, int& outoff, bool& noaddr, bool& failure )
        bool noaddr = false;
        bool failure = false;
        int outoff = 0;
        uint32_t addr = static_cast<uint16_t>(processaddress (p,std::ref(outoff), std::ref(noaddr), std::ref(failure), false));

        if(failure){
            // snprintf (lasterrorstr, sizeof(lasterrorstr), "Unable To Parse Data Definition\n");
            return DATAPARSEFAILURE;
        }

        if(!noaddr){
            printf("DD Address : %x\n", addr);
            addrset = true;
            address = addr;
        } else {
            printf("WARNING : NO DATA SEGMENT ADDRESS %s\n", p);
        }
    }

    // 0 means no address change
    return 0;
}






class executable704 {
public:
    executable704() {

    }
    executable704(const char* path){
        out = std::ofstream (path, std::ios::out | std::ios::binary);
        //out.seekp((0x7FFF<<20) - 1);
        out.write("", 1);
        out.flush();
    }


    void write(char* data, uint16_t offset){
        //int ofbytes =;
        //auto soff = std::streamoff(offset * ofbytes);
        out.seekp( offset *  ((35 >> 3) + 1), std::ios::beg);
        out.write (data, ((35 >> 3) + 1));
        out.flush();
    }

    void write(word704 w, uint16_t offset){
        //int ofbytes =;
        //auto soff = std::streamoff(offset * ofbytes);
        uint64_t val = 0 ;//= w.tonum<uint64_t>();
        w.get (&val, 36, 0,0 );
        //printf("Writing %lx\n", val);
        //w.print_bits ();
        write((char*)&val, offset);
    }

private:
    std::ofstream out;
};
class assembler704{
public:
    assembler704(const char* path, const char* outpath){
        in = std::ifstream(path, std::ios::in);
        out = executable704(outpath);
    }


    void assemble( ){
        std::string line;
        int i = 0;
        while(std::getline(in, line, '\n')){
            word704 word;
            int ret = 0;

            if(islinecomment (line.c_str()))
                continue; // who cares
            else if (isdescriptor (line.c_str())) {
                bool cset = false;
                bool dset = true;
                bool aset = false;
                uint16_t address = 0;
                ret = processdescriptor ( line.c_str ( ),  std::ref(cset), std::ref(dset), std::ref(aset), std::ref(address), i);

                if(aset){
                    m_currentaddress = address;
                }
                if(cset){
                    printf("INFO : CODE SEGEMENT SWITCH AT %d -> 0x%x\n", i, m_currentaddress);
                    incoderegion = true;
                    indataregion = false;
                } else if(dset){
                    printf("INFO : DATA SEGEMENT SWITCH AT %d -> 0x%x\n", i, m_currentaddress);
                    indataregion = true;
                    incoderegion= false;
                }


            }
            else if(incoderegion) {

                printf("INS 0x%04x (off: 0x%04x)\t: ", m_currentaddress , m_currentaddress * ((35 >> 3) + 1));
                ret = processinstruction ( line.c_str ( ), word, i );

                if(ret >= 0){
                    // ,
                    out.write ( word, m_currentaddress);
                    m_currentaddress += 1;
                    //printf("h\n");
                }
            } else if (indataregion){
                bool noaddr = false;
                bool failure = false;
                int outoff = 0;
                uint64_t addr = processaddress (line.c_str(),std::ref(outoff), std::ref(noaddr), std::ref(failure), false);

                if(!noaddr && !failure) {
                    if(addr > 68719476735){
                        snprintf(lasterrorstr, sizeof(lasterrorstr), "Unable To Compile Data Byte, %lx > 36_MAX (%lx)", addr, 68719476735);
                        ret = DATAPARSEFAILURE;
                    } else {
                        printf ( "Data byte @ %x = %x\n", m_currentaddress, addr );
                        word.fromnum ( addr, false, true );
                        out.write ( word, m_currentaddress);
                        m_currentaddress += 1;
                    }
                }
            }


            if( ret < 0 ) // todo : intelligent error
            {
                //printf ( "ret %d\n", ret );
                printf("ASM ERR [%d] (%s) @ %d :\n\t%s\n", ret, codetoerr(ret), i, lasterrorstr);
                printf("%s\n", line.c_str());
                return;
            }
            i++;

        }
    }


private:
    std::ifstream in;
    executable704 out;
    uint16_t m_currentaddress = 0;
    bool incoderegion=true;
    bool indataregion=false;
    const std::map<std::string, uint16_t> symbols;
};



int main( ){
    word704 inst;
    assembler704 assembler("source.asm", "compiled.bin");
    assembler.assemble();

    // processline("CLA 403", inst, nullptr );





}




