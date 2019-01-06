
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "../framework/Element.h"
#include "GrainDrone.h"

namespace np{

class SeqGDrone : public Element {
    
public:
    
    void position(int x, int y, int w, int h) override;

    void setup( string filePath, ofxPDSPValue & key_control, pdsp::ScoreSection & section);
    
    void setColor( ofColor color );
    
    
    void draw() override;

    void setGain(float dBvalue);
    
    void enable() override;
    void disable() override;
    
    bool keyPressed( int key ) override;
    
    pdsp::GrainCloud & getCloud();
    pdsp::Patchable & outL();
    pdsp::Patchable & outR();
    
    double quantize;
    
    int meter_seq() const;
    float meter_filter() const;
    
private:
    
    pdsp::ScoreSection * section;
    
    GrainDrone cloud;
    
    float x;
    float x_speed;
    float r0;
    float r1;
    float r2;
    float r3;
    
    ofxPDSPTrigger trig;
    vector<pdsp::Sequence> cloud_seqs;
    
    void move_x(){
        x += x_speed;
        if(x > 0.95 || x < 0.05f ){
            x_speed *= -1.0f;
        }
    }
};


}
