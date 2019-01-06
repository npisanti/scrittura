
// Nicola Pisanti, GPLv3 License, 2016

#include "SymbolPermutator.h"

void np::SymbolPermutator::setup(int fill, int maxSteps, float mutateChance){
    
    this->fill = fill;
    this->maxSteps = maxSteps;
    this->mutateChance = mutateChance;
    
    newFill = fill;
    this->fill = 0;
    maxSteps = 32;
    possibilities.reserve(maxSteps*4);
    
    
    for(int i=0; i<(int)symbolChances.size(); ++i){
        int ichance = maxSteps*2.0f*symbolChances[i];
        for( int k=0; k < ichance; ++k){
            possibilities.push_back(i); // accent
        }
    }
    
    steps.resize(maxSteps);
    
    for(int i=0; i< maxSteps; ++i){
        steps[i] = 0;
    }


}

void np::SymbolPermutator::update(){
   
        // if fill has changed
        int nextFill = newFill;
        
        if(nextFill>fill){
            
            while(fill!=nextFill){
                if(fill!=0){
                    int irand = pdspDice( maxSteps -1 ) + 1;
                    if( steps[irand] != 0){
                        while(steps[irand] != 0 ){
                            irand++;
                            if(irand == maxSteps) irand = 1;
                        }
                    }

                    int prand = pdspDice( (int)possibilities.size() );
                    int temp = possibilities[prand];
                    possibilities.erase(possibilities.begin() + prand);
                    steps[irand] = temp;
                }else{
                    steps[0] = 2;
                }
                fill++;

            }            

        }else if (nextFill<fill) {

            // change to random removal
            while(fill!=nextFill){
                if(fill!=1){
                
                    int irand = pdspDice( maxSteps-1 ) + 1;
                    if( steps[irand] == 0){
                        while(steps[irand] == 0 ){
                            irand++;
                            if(irand == maxSteps) irand = 1;
                        }
                    }
                    possibilities.push_back(steps[irand]);
                    steps[irand] = 0;
                }else{
                    steps[0] = 0;
                    
                }
                fill--;
            }        
        }
        
    
        if(fill!=0){
            if(pdspChance(mutateChance)){
                // mutate
                int irand = pdspDice(maxSteps-1) +1;
                while(steps[irand] == 0){
                    irand++;
                    if(irand == maxSteps) irand = 1;
                }    
                int prand = pdspDice( (int)possibilities.size() );
                int temp = possibilities[prand];
                possibilities[prand] = steps[irand];
                steps[irand] = temp;
            }else{
                // search for swapping
                int irand = pdspDice(maxSteps-1) + 1;
                int irand2 = pdspDice(maxSteps-1) + 1;
                while(steps[irand] == steps[irand2]){
                    irand2++;
                    if(irand2 == maxSteps) irand2 = 1;
                }            
         
                // swap
                int temp = steps[irand2];
                steps[irand2] = steps[irand];
                steps[irand] = temp;        
            }            
        }
        
        for(int i=1; i<maxSteps; i+=2){
            int iprev = i-1;

            if(steps[iprev] == 0){
                steps[iprev] = steps[i];
                steps[i] = 0;
            }else if(steps[i] == 2){
                steps[i] = steps[iprev];
                steps[iprev] = 2;     
            }
        }
        
        
}



