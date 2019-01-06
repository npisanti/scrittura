
#pragma once

#include "nplib.h"

class CloudScriber{

public:
    CloudScriber();
    
    void draw(const pdsp::GrainCloud & cloud, int mode, float playhead_pct, float w, float h);

private:
    
    vector<float> last_envs;
    ofPoint last_point;
    float last_playhead;
    
};
