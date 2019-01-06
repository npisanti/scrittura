
// Nicola Pisanti, GPLv3 License, 2016

#include "cloudscriber.h"

CloudScriber::CloudScriber(){
    
    last_envs.resize(8);
    for( float & value : last_envs){
        value = 0.0f;
    }

    last_point.x = 0;
    last_point.y = 0.0f;
    last_playhead = 0.0f;
    for (int i=0; i<(int)last_envs.size(); ++i){
        last_envs[i] = 1.0f;
    }
}


void CloudScriber::draw(const pdsp::GrainCloud & cloud, int mode, float playhead_pct, float w, float h){
    
    ofNoFill();
    
    for(int i=0; i < 8; ++i){
        
        float x1 = last_playhead * w;
        float x2  = playhead_pct * w;
        if(x1 > x2 ) x1 = 0.0f;
        
        float env = cloud.meter_env(i);
        float jit = cloud.meter_jitter(i);
        jit = (jit+1.0f ) * 0.5f;
        float y1 = (1.0f - last_envs[i]) * h;
        float y2 = (1.0f - jit) * h;
        
        float roundness = cloud.meter_position(i);
        

        
        if(env > 0.0f ){
            if(jit != last_envs[i]){
                //ofDrawLine(x1, y1, x2, y2); 
                if(last_point.x > x1) last_point.x -= w;
                float mdist = abs(x2-x1)*2.0f;
                if (abs(x1-last_point.x) > mdist) { last_point.x = x1 - mdist*0.5f; }
                
                bool clock = (last_point.y<y1) ? false : true;

                np::arcpipe(last_point.x, last_point.y, x1, y1, 1.0f, roundness, clock);                
                
                clock = (y1<y2) ? true : false;
                np::arcpipe(x1, y1, x2, y2, 0.9f, roundness, clock);
                //if (square ) { ofDrawRectangle(x2-3, y2-3, 6, 6); }
                last_point.x = x2;
                last_point.y = y2;
            }    
            
                    
        }
        last_envs[i] = jit;
        
    }
    
    last_playhead = playhead_pct;
    
}
