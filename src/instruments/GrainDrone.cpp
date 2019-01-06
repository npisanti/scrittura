
// Nicola Pisanti, GPLv3 License, 2016

#include "GrainDrone.h"


void np::GrainDrone::position(int x, int y, int w, int h) {
    uiX = x;
    uiY = y;
    uiWidth = w;
    uiHeight = h;
    grainW = 10;
    grainH = (h*7)/10;

}

void np::GrainDrone::setColor( ofColor color ){
    this->color = color;
    colorDim = color.getLerped(ofColor::black, 0.4); 
}



void np::GrainDrone::setup(string filePath, ofxPDSPValue & key_control ){

    position(0, 0, 512, 256);
  

    sampleData.load(filePath); 

    string newLabel = "cloud : ";
    if(sampleData.loaded()){
        string base_filename = filePath.substr(filePath.find_last_of('/') + 1);
        newLabel+=base_filename;
    }else{
        newLabel+= "no wave!";
    }

    label = newLabel;
    
    //--------GRAINCLOUD-----------------------
    grainVoices = cloud.getVoicesNum();
    
    
    cloud.setWindowType(pdsp::Tukey); // select the shape of the grain envelope
            // available windows: Rectangular, Triangular, Hann, Hamming, Blackman, BlackmanHarris, SineWindow, Welch
    cloud.setSample(&sampleData); // give to the pdsp::GrainCloud the reference to the sample


    0.02f  >> cloud.in_position_jitter();  
    500.0f >> cloud.in_length();
    0.95   >> cloud.in_density();
    20.0f  >> cloud.in_distance_jitter();
     0.05f >> cloud.in_pitch_jitter();
    // 0.0f >> cloud.in_pitch();  
  
    gain = dB(-12.0f);

    ampEnv * gain >> voiceAmpL.in_mod(); 
    ampEnv * gain >> voiceAmpR.in_mod(); 
    //1.0f >> voiceAmpR.in_mod(); 
    
    ampEnv.set(50.0f, 50.0f, 1.0f, 5000.0f);

    // SET UP RESONATORS----------------------------------------
    resonators.resize(8);
    resonators_sends.resize(8);
    resonators_ctrl.resize(4);
    
    for(int i=0; i<8; ++i){
        0.95f >> resonators[i].in_fb();
        if(i%2==0){
            cloud.out_L() >> resonators_sends[i] >> resonators[i] >> filter_stage_1L;
        }else{
            cloud.out_R() >> resonators_sends[i] >> resonators[i] >> filter_stage_1R;
        }
    }
    
    float spread = 0.05f;
    
    for(int i=0; i<4; ++i){
        resonators_ctrl[i].set(0.25f);
        resonators_ctrl[i] >> resonators_sends[i*2].in_mod();
        resonators_ctrl[i] >> resonators_sends[i*2 + 1].in_mod();
        
        float base = 36.0f;
        float pitch;
        switch(i){
            case 0: pitch = spread + base -12.0f; break;
            case 1: pitch = spread + base; break;
            case 2: pitch = spread + base +12.0f; break;
            case 3: pitch = spread + base +24.0f; break;
        }
        pitch       >> resonators[i*2].in_pitch();
        key_control >> resonators[i*2].in_pitch();
        pitch       >> resonators[i*2 + 1].in_pitch();
        key_control >> resonators[i*2 + 1].in_pitch();
    }
    // FINISHED WITH RESONATORS ------------------------------    

    filter_p2f.set(120.0f);
    filter_p2f >> filter_stage_1L.in_cutoff();
    filter_p2f >> filter_stage_2L.in_cutoff();
    filter_p2f >> filter_stage_1R.in_cutoff();
    filter_p2f >> filter_stage_2R.in_cutoff();

    filter_stage_1L >> filter_stage_2L >> voiceAmpL;
    filter_stage_1R >> filter_stage_2R >> voiceAmpR;
 
    // create an ofFbo with the plotted waveform
    //waveformGraphics.setWaveform(sampleData, 0, ofColor(colorDim), uiWidth, uiHeight);
    waveformGraphics.setWaveform(sampleData, 0, ofColor(90), 1024, 512);

}


void np::GrainDrone::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(uiX, uiY);
    
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetColor(color);
        ofNoFill();

        ofDrawRectangle( 0, 0, uiWidth, uiHeight);
        
        ofSetRectMode(OF_RECTMODE_CORNER);        
        waveformGraphics.draw(0, 0, uiWidth, uiHeight);
        ofDrawBitmapString (sampleData.filePath, 5, 14);
    
        if( ampEnv.meter_output() > 0.0f ){
            ofSetRectMode(OF_RECTMODE_CENTER);
            float y_filter_mult = ofMap(filter_p2f.meter_input(), 50.0f, 130.f, 0.0f, grainH);
            int grainsY = uiHeight/2;
            for (int i=0; i<grainVoices; ++i){
                float xpos = uiWidth * cloud.meter_position(i);
                float dimensionX = cloud.meter_env(i)*10;
                float dimensionY = cloud.meter_env(i)*y_filter_mult;
                ofDrawRectangle(xpos, grainsY, dimensionX, dimensionY);
            }       

        }

    ofPopMatrix();
    ofPopStyle();
    
}


void np::GrainDrone::setGain(float dBvalue) {
    gain = dB(dBvalue);
    ampEnv * gain >> voiceAmpL.in_mod(); 
    ampEnv * gain >> voiceAmpR.in_mod(); 
}

pdsp::Patchable& np::GrainDrone::in_trig(){
    return ampEnv;
}

pdsp::Patchable& np::GrainDrone::in_position(){
    return cloud.in_position();
}

pdsp::Patchable& np::GrainDrone::in_cutoff(){
    return filter_p2f;
}

pdsp::Patchable& np::GrainDrone::in_resonator(int i){
    return resonators_ctrl[i];
}


void np::GrainDrone::preset( int presetTag, ofxPDSPValue & key_control ){
    
    switch (presetTag){
        
        case bowed_ride:
            setup( "/home/nick/data/live/bowed_ride_ed.wav", key_control );
            -12.0f >> cloud.in_pitch();
            0.f >> cloud.in_pitch_jitter();
        break;
        
        case coffee:
            setup( "/home/nick/data/live/coffee_ed.wav", key_control );
        break;
        
        case noise_floor:
            setup( "/home/nick/data/live/noise_floor_scale_F.wav", key_control );
        break;
    
    }
    
}

float np::GrainDrone::meter_cutoff() const{
    return filter_p2f.meter_input();
}
