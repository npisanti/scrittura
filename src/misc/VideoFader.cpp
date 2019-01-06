
// Nicola Pisanti, GPLv3 License, 2016

#include "VideoFader.h"

np::VideoFader::VideoFader() {
    addModuleInput( "L", ampL );
    addModuleInput( "R", ampR );
    addModuleOutput( "L", ampL );
    addModuleOutput( "R", ampR );
    
    rising = true;
    fading = false;
    
    audio_fade_value = 0.0f; // 0 dB
    audio_fade_ctrl.setv( 1.0f );
    video_fade_alpha = 0.0f;
    
    setFadeTime( 3000.0f );
    setDBRange ( 48.0f );
    
    audio_fade_ctrl.enableSmoothing(50.0f);
    audio_fade_ctrl >> ampL.in_mod();
    audio_fade_ctrl >> ampR.in_mod();
    
}

void np::VideoFader::setFadePoint(  float starting_fade, bool rising ) {
    
    if(starting_fade != 0.0f ){
        audio_fade_value = (dBrange * starting_fade) - dBrange;
        audio_fade_ctrl.setv( dB( audio_fade_value ) );
    }else{
        audio_fade_value = -dBrange ;
        audio_fade_ctrl.setv( 0.0f );
    }
    video_fade_alpha = (1.0f - starting_fade ) * 255.0f ;    

    this->rising = rising;
    
    if( (rising && starting_fade==1.0f) || (!rising && starting_fade==0.0f) ){
        fading = false;
    }

}

void np::VideoFader::setDBRange ( float dBrange ) {
    this->dBrange = dBrange;
    audio_fade_step = dBrange  / fadeMs ;
}

void np::VideoFader::fadeIn() {
    rising = true;
    fading = true;
}

void np::VideoFader::fadeOut() {
    rising = false;
    fading = true;
}

void np::VideoFader::setFadeTime( float ms ) {
    fadeMs = ms;
    video_fade_step = -255.0f / fadeMs ;
    audio_fade_step = dBrange  / fadeMs ;
}

void np::VideoFader::update() {
    
    uint64_t millis = ofGetElapsedTimeMillis();
    
    if( fading ) {
        
        float steps = millis - last_millis;

        if(rising) {
            
            video_fade_alpha += video_fade_step * steps;
            
            if(video_fade_alpha <= 0.0f){
                video_fade_alpha = 0.0f;
                audio_fade_value = 0.0f;
                audio_fade_ctrl.setv( 1.0f );
                fading = false;
            }else{
                audio_fade_value += audio_fade_step * steps;
                audio_fade_ctrl.setv( dB(audio_fade_value) );
            }
            
        }else{

            video_fade_alpha -= video_fade_step * steps;
            
            if(video_fade_alpha > 255.0f){
                video_fade_alpha = 255.0f;
                audio_fade_value = -dBrange;
                audio_fade_ctrl.setv( 0.0f );
                fading = false;
            }else{
                audio_fade_value -= audio_fade_step * steps;
                audio_fade_ctrl.setv( dB(audio_fade_value) );
            }
        }
   
    }
    
    last_millis = millis;
}

void np::VideoFader::draw() {
    if( video_fade_alpha >0.0f) {
        ofPushStyle();
            ofSetColor(0, 0, 0, video_fade_alpha);
            ofFill();
            ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );
        ofPopStyle();
    }
}


pdsp::Patchable& np::VideoFader::in_L(){
    return in("L");
}

pdsp::Patchable& np::VideoFader::in_R(){
    return in("R");
}

pdsp::Patchable& np::VideoFader::out_L(){
    return out("L");
}

pdsp::Patchable& np::VideoFader::out_R(){
    return out("R");
}
