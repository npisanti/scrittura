
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "../sequencers/WolframSequencer.h"
#include "../synths/Zap.h"
#include "../framework/Element.h"

namespace np {

class WolframZaps : public Element {
    
public:

    void position(int x, int y, int w, int h) override;
    void setColor( ofColor color ) override;
    
    void setup( int steps, double division, int rule, int generations, int threshold, string label, pdsp::ScoreSection & assignedSection,ofxPDSPValue & key_control, std::initializer_list<float> tuning );   
    
    void draw() override;

    void setGain(float dBvalue);
    
    void enable() override;
    void disable() override;
    
    bool keyPressed( int key ) override;
    
    
    pdsp::Amp            outL;
    pdsp::Amp            outR;
    
    double quantize;
    
    vector<Zap>          zaps;
    WolframSequencer     seq;
    
    
private:
    
    pdsp::ScoreSection *  section;
    
    pdsp::Sequence        wolframSequence;


};


}
