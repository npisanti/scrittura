
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np{

class SymbolPermutator{
    
    
    
public:

    vector<float> symbolChances;
    void setup(int fill, int maxSteps, float mutate);

    void update();

    vector<int> steps;
    int fill;
    atomic<int> newFill;    
    
    atomic<float> mutateChance;    
    
    
private:
    
    vector<int> possibilities;
    
    int maxSteps;

};


}
