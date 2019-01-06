
#pragma once

#include "ofxPDSP.h"
#include "nplib.h"


class Sub : public np::Element {
    
public:

    void position(int x, int y, int w, int h) override;
    void setColor( ofColor color );
    void setup(string name, ofxPDSPValue & key_control, pdsp::ScoreSection & assignedSection, np::WolframSequencer & wtarget, np::SymbolPermutator & ptarget);
    
    void draw() override;
        
    pdsp::Amp           amp;
    vector<float>       bars;

    void enable() override;
    void disable() override;
    
    bool keyPressed( int key ) override;
    
private:
    
    int uiX;
    int uiY;
    int uiWidth;
    int uiHeight;
    
    string label;   
    
    vector<pdsp::Sequence> seqs;
    pdsp::Sequence      sequence;

    int                 steps;

    pdsp::ScoreSection * section;

    pdsp::FMOperator    osc;
    pdsp::AHR           aEnv;   // modulable attack, hold and relese

    pdsp::AHR           pEnv;   // modulable release, attack=0; hold = 0;
    pdsp::Amp           pMod;   // modulable pitch mod
    
    np::WolframSequencer *   target;
    np::SymbolPermutator *  clicks;
    
    pdsp::OneBarTimeMs  bar;
    
    ofColor             subColor;
};





