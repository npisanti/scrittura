
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "../framework/Element.h"

namespace np{

class GrainDrone : public Element {
    
public:

    void position(int x, int y, int w, int h) override;

    void setup( string filePath, ofxPDSPValue & key_control);
    
    void setColor( ofColor color ) override;
    
    void preset(int presetTag, ofxPDSPValue & key_control );    
    
    void draw() override;

    void setGain(float dBvalue);
    
    pdsp::Patchable& in_trig();
    pdsp::Patchable& in_position();
    pdsp::Patchable& in_position_jitter();
    pdsp::Patchable& in_cutoff();
    pdsp::Patchable& in_resonator(int i);


    pdsp::GrainCloud        cloud;
    pdsp::ADSR              ampEnv;

    pdsp::Amp               voiceAmpL;
    pdsp::Amp               voiceAmpR;

    // preset tags
    
    static const int bowed_ride = 0;
    static const int coffee     = 1;
    static const int noise_floor= 2;
  
    string label;
    
    float meter_cutoff() const;
        
private:

    ofxSampleBufferPlotter  waveformGraphics;
   
    int                         grainVoices;
    pdsp::SampleBuffer          sampleData;

    pdsp::OnePole           filter_stage_1L;
    pdsp::OnePole           filter_stage_2L;
    pdsp::OnePole           filter_stage_1R;
    pdsp::OnePole           filter_stage_2R;

    pdsp::PitchToFreq       filter_p2f;
    
    vector<pdsp::CombFilter> resonators;
    vector<pdsp::Amp>        resonators_sends;
    vector<pdsp::PatchNode>  resonators_ctrl;

    ofColor color;
    ofColor colorDim;
    
    float gain;
    
    int uiX;
    int uiY;
    int uiWidth;
    int uiHeight;
  
    int grainW;
    int grainH;
    
};

}
