
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "../misc/SymbolPermutator.h"
#include "../framework/Element.h"

namespace np{

class perMutante1 : public Element {

public:

    void position(int x, int y, int w, int h) override;
    void setColor( ofColor color ) override;
    
    void setup( string name,  ofxPDSPValue & key_control, pdsp::ScoreSection & assignedSection );
    
    void draw() override;
    
    void setGain(float dBvalue);
    
    void enable() override;
    void disable() override;
    
    double quantize;
    
    bool keyPressed( int key ) override;  
    
    SymbolPermutator permutator;
    
    pdsp::Amp ampL;
    pdsp::Amp ampR;

    pdsp::CRSlew         click_ctrl;
    pdsp::Amp            click_muteL;
    pdsp::Amp            click_muteR;

private:
    
    ofColor color;
    ofColor colorDim;
    
    pdsp::ScoreSection * section;
    
    pdsp::LFOPhazor clock;
    pdsp::PRNoiseGen noiseL;
    pdsp::PRNoiseGen noiseR;
    pdsp::MultiLadder4 filterL;
    pdsp::MultiLadder4 filterR;

    pdsp::AHR ampEnv;
    pdsp::AHR filterEnv;
    
    pdsp::PitchToFreq p2f;
    pdsp::PitchToFreq p2filter;
    
    pdsp::Sequence sequence;
    
    
    vector<float> pitches;
    ofxPDSPValue * key;
    
    float gain;
    
    int uiX;
    int uiY;
    int uiWidth;
    int uiHeight;
    
    string label;
};

}
