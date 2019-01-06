
// Nicola Pisanti, GPLv3 License, 2016

#include "SeqGDrone.h"

void np::SeqGDrone::position(int x, int y, int w, int h) {
    cloud.position(x, y, w, h);
}

void np::SeqGDrone::setColor( ofColor color ){
    cloud.setColor(color);
}

void np::SeqGDrone::setup( string filePath, ofxPDSPValue & key_control, pdsp::ScoreSection & section) {
    
    cloud.setup(filePath, key_control);
    
    setMinDimensions(300, 50);
    
    string info = "0 : random point static  ///  ";
    info += "1 : scrolling  ///  ";
    info += "2 : jittery";
    setInstructions(info);
    
    this->section = &section;
    quantize = 2.0;

    trig >> cloud.in_trig();
    
    x_speed = 0.0005f;
    x = ofRandom(0.05f, 0.95f);
    r0 = 0.25;
    r1 = 0.25;
    r2 = 0.25;
    r3 = 0.25;
    
    r0 >> cloud.in_resonator(0);
    r1 >> cloud.in_resonator(1);
    r2 >> cloud.in_resonator(2);
    r3 >> cloud.in_resonator(3);
    
    cloud_seqs.resize(3);
    section.setCell(0, &cloud_seqs[0], pdsp::Behavior::Loop);
    section.setCell(1, &cloud_seqs[1], pdsp::Behavior::Loop);
    section.setCell(2, &cloud_seqs[2], pdsp::Behavior::Loop);
    
    section.out_value(0) >> cloud.in_position();
    section.out_value(1) >> cloud.in_cutoff();
   
    // using data here could lead to straaaaaaaaaaaaaange things, lambda function is not executed here
    // but it seeems it is working
    cloud_seqs[0].code = [&] () noexcept {
        0.02f >> cloud.cloud.in_position_jitter(); 
        x = ofRandom(0.05f, 0.95f);
        x_speed = 0.0005f;        
        cloud_seqs[0].begin(16.0, 8.0);
        for(int i=0; i<16*8; ++i){
            move_x();
            cloud_seqs[0].message( (double) i, x, 0);  
        }
        cloud_seqs[0].message( 0.0, 100.0f, 1);  
        cloud_seqs[0].end();
    };
    
    
    cloud_seqs[1].code = [&] () noexcept {
        0.6f >> cloud.cloud.in_position_jitter(); 
        x = 0.5f;
        bool eight = true;
        cloud_seqs[1].begin(16.0, 8.0);
        for(int i=0; i<16*8; ++i){
            cloud_seqs[1].message( (double) i, x, 0);  
            if(eight){
                float value = 70.0f + pdspURan()*50.0f;
                //int imax = wseq.newActiveOuts * 32;
                //int ibar = i;
                //while(ibar>=imax) ibar-=imax;
                //float value = (wseq.bars[ibar] * -50.0f) + 130.0f;
                cloud_seqs[1].message( (double) i, value, 1);
            }
            eight = eight ? false : true;
        }
        cloud_seqs[1].end();
    };

    cloud_seqs[2].code = [&] () noexcept {
        0.02f >> cloud.cloud.in_position_jitter(); 
        x = ofRandom(0.05f, 0.95f);
        cloud_seqs[2].begin(16.0, 8.0);
        int max = 16*8;
        float max_div = 2.0f / (float) max;
        for(int i=0; i<max; ++i){
            float pos = x + (((float)i)*max_div) * 0.9f;
            float cut = pos * 0.5f;
            if(pos > 0.95f){
                pos -= 0.95f;
                pos = 0.95f - pos;
            }
            if (pos<0.05f){
                pos -= 0.05f;
                pos = 0.05 - pos;
            }
            cloud_seqs[2].message( (double) i, pos, 0);  
            cloud_seqs[2].message( (double) i, 90.0f + cut*30.0f, 1); 

        }
 
        cloud_seqs[2].end();
    };
    
    
    section.launchCell(-1, true, 2.0);
    
}

void np::SeqGDrone::draw() {
    cloud.draw();
}

void np::SeqGDrone::setGain(float dBvalue){
    cloud.setGain(dBvalue);
}

void np::SeqGDrone::enable() {
    trig.trigger(1.0f);
    setState(true);
}

void np::SeqGDrone::disable() {
    trig.off();
    setState(false);
}

bool np::SeqGDrone::keyPressed( int key ) {
    switch(key){
        case 0: 
            section->launchCell(0, true, quantize);      
            return true;
       
        case 1: 
            section->launchCell(1, true, quantize);      
            return true;
       
        case 2: 
            section->launchCell(2, true, quantize);      
            return true;
    }
    return false;
}

pdsp::GrainCloud & np::SeqGDrone::getCloud(){
    return cloud.cloud;
}

pdsp::Patchable & np::SeqGDrone::outL(){
    return cloud.voiceAmpL;
}
pdsp::Patchable & np::SeqGDrone::outR(){
    return cloud.voiceAmpR;    
}

int np::SeqGDrone::meter_seq() const {
    return section->meter_current();
}

float np::SeqGDrone::meter_filter() const {
    return cloud.meter_cutoff();
}
