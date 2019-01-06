
#pragma once

#include "ofxPDSP.h"
#include "ofMain.h"

namespace np {
    
    
class VideoFader : public pdsp::Patchable {
    
public:
    
    VideoFader();

    void setFadePoint(  float starting_fade, bool rising ); 
    
    void setFadeTime( float ms );
    
    void setDBRange ( float dBrange );
    
    void fadeIn();
    void fadeOut();
    
    void update();
    void draw();

    
    pdsp::Patchable& in_L();
    pdsp::Patchable& in_R();
    pdsp::Patchable& out_L();
    pdsp::Patchable& out_R();
    
private:
    
    
    pdsp::Amp ampL;
    pdsp::Amp ampR;
    
    bool rising;
    bool fading;
    
    ofxPDSPValue audio_fade_ctrl;
    float        audio_fade_value;
    float        audio_fade_step;
    
    float        video_fade_alpha;
    float        video_fade_step;

    uint64_t     last_millis;
    
    float        dBrange;
    float        fadeMs;
};

    
}
