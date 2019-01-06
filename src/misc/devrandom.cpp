
// Nicola Pisanti, GPLv3 License, 2016

#include "devrandom.h"

unsigned int np::dev_random_seed()
{
    unsigned int random_seed;
    std::ifstream file ("/dev/urandom", std::ios::binary);
    if (file.is_open())
    {
        char * memblock;
        int size = sizeof(int);
        memblock = new char [size];
        file.read (memblock, size);
        file.close();
        random_seed = *reinterpret_cast<int*>(memblock);
        delete[] memblock;
    }// end if
    else
    {
        random_seed = 0;
        std::cout<<"FAILED TO GET SEED!!!!\n";
    }
    //std::cout << " random_seed =  " << random_seed << std::endl;
    return random_seed;
} // end good_seed()


void np::seed_oracle(){
    ofSeedRandom(dev_random_seed());
    pdsp::seedGlobalRandom(dev_random_seed());
}
