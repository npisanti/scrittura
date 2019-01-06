
// Nicola Pisanti, GPLv3 License, 2016

#include "ensemble.h"


void Ensemble::setup ( ofxPDSPEngine & engine, ofColor mainColor){

    key.setv(5.0f); // F

    score = & engine.score;

    // ------------------------------- Setting up score ----------------------------
    //engine.score.stop();    // decomment if you want to start the app from stopped state
    engine.score.setTempo(168.0f);
    engine.score.stop();    
    engine.score.sections.resize(6);
    engine.score.setMaxBars(32.0);

    masterplan_counter = 0;
    sub_counter = 0;
    
    // UI ifdefs 

    //  ------------------------------ MASTER ------------------------------
    masterSequence.setLength(2.0);
    engine.score.sections[0].setCell(0, &masterSequence, pdsp::Behavior::Loop);
    engine.score.sections[0].enableQuantization( 0, 2.0 );
    engine.score.sections[0].launchCell(0);
    masterSequence.code = [&] () noexcept {
        masterplan();
    };

    // wolfram -----------------------------
    wolfram.setup(  32, 16.0, //steps, division,
                    60, 10, 4, //rule, generations, threshold
                    "RITMI DI WOLFRAM", engine.score.sections[1], // section
                    key, {48.0f, 53.0f, 58.0f, 63.0f} );
    wolfram.enable();
                    
    // mutante  -----------------------------  
    noise.setup("perMutante", key, engine.score.sections[2]);
    noise.permutator.newFill = 12;
    noise.enable();
    
    // sub -----------------------------    
    #ifdef SUBSONICS  
    sub.setup( "SUBsonico", key, engine.score.sections[3], wolfram.seq, noise.permutator );
    sub.enable();
    #endif
    
    // bowed ride cloud -----------------------------    
    //cloud1.setup( "/home/nick/oF/apps/works/scrittura_uno/bin/data/bowed_ride.wav", key, engine.score.sections[4] );
    cloud1.setup( ofToDataPath("bowed_ride.wav"), key, engine.score.sections[4] );
    -12.0f >> cloud1.getCloud().in_pitch();
    0.f >> cloud1.getCloud().in_pitch_jitter();
    cloud1.enable();

    // ritual metals cloud -----------------------------      
    //cloud2.setup( "/home/nick/oF/apps/works/scrittura_uno/bin/data/coffee.wav", key, engine.score.sections[5] );
    cloud2.setup( ofToDataPath("coffee.wav"), key, engine.score.sections[5] );
    cloud2.enable();


    // ---------------------------- BUS ROUTINGS ---------------------------------------
    // ---------------------------- GAINS----------------------
    
    wolfram.setGain(-15.0f);  
    noise.setGain(-15.0f);
    cloud1.setGain( 3.0f);  
    cloud2.setGain( -6.0f);      
    
    #ifdef SUBSONICS
    sub.amp * dB(-14.0f) >> fadeToBlack.in_L();
    sub.amp * dB(-14.0f) >> fadeToBlack.in_R();  
    #endif
        
    //-------------------------- routings--------------------------  
    
    wolfram.outL  >>  fadeToBlack.in_L();
    wolfram.outR  >>  fadeToBlack.in_R(); 
  
    noise.click_muteL >> fadeToBlack.in_L();
    noise.click_muteR >> fadeToBlack.in_R(); 

    cloud1.outL() >> ambientCut.in_0();
    cloud1.outR() >> ambientCut.in_1();
    cloud2.outL() >> ambientCut.in_0();
    cloud2.outR() >> ambientCut.in_1();

    //---------------------------- reverb bus setup ----------------------
    wolfram.outL * dB(-48.0f) >> verb;
    wolfram.outR * dB(-48.0f) >> verb; 
    0.5f >> verb.in_damping();
    //3.0f >> verb.in_time();
    verb.out_0() >> fadeToBlack.in_L();
    verb.out_1() >> fadeToBlack.in_R();    
    
    // --------------------------- ambient bus setup ----------------------------    
    
    ambientCut.out_0() >> ambientChorus.in_0();
    ambientCut.out_1() >> ambientChorus.in_1();
    ambientChorus.out_0() >> fadeToBlack.in_L(); 
    ambientChorus.out_1() >> fadeToBlack.in_R(); 
    
    100.0f >> ambientCut.in_freq();
    -18.0f >> ambientBus.in_threshold();
    4.0f >> ambientBus.in_ratio();
    50.0f >> ambientBus.in_attack();
    200.f >> ambientBus.in_release();

    fadeToBlack.out_L() * dB(3.0f) >> engine.audio_out(0);
    fadeToBlack.out_R() * dB(3.0f)  >> engine.audio_out(1);
    
    // --------------------------- fires up the engine ----------------------------    

#ifdef PULSE
    int max = ofSoundStreamListDevices().size() - 1;
    cout << "selected "<< max << "\n";
    engine.setDeviceID(max); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
#else
    engine.listDevices();
    engine.setDeviceID(0);
#endif
    
    engine.setup( 44100, 512, 3);     
}

// -------------------------------------------- MASTERPLAN -------------------------------------
void Ensemble::masterplan(){
    
    if(masterplan_counter == 0 ){

        // graiiiins
        float choice = pdspURan();
        if(choice <0.45f){
            score->sections[4].launchCell(0, true, 2.0);            
        }else if(choice <0.80f){
            score->sections[4].launchCell(1, true, 2.0);
        }else{
            score->sections[4].launchCell(2, true, 2.0);            
        }
        
        choice = pdspURan();
        if(choice <0.45f){
            score->sections[5].launchCell(0, true, 2.0);               
        }else if(choice <0.80f){
            score->sections[5].launchCell(1, true, 2.0);               
        }else{
            score->sections[5].launchCell(2, true, 2.0);   
        }


        // SUB
        #ifdef SUBSONICS
        if(sub_counter==0){
            if(pdspChance(0.21f)){
                sub_counter = 2 + pdspDice(3); 
            }else{
                score->sections[3].launchCell(0, true, 2.0); 
            }
        }
        if(sub_counter>0){
            sub_counter--;
            
            choice = pdspURan();
            if(choice <0.33f){
                score->sections[3].launchCell(1, true, 2.0);               
            }else if(choice <0.66f){
                score->sections[3].launchCell(2, true, 2.0);               
            }else{
                score->sections[3].launchCell(3, true, 2.0);   
            }
        }
        #endif
        
        // perMutante
        noise.permutator.newFill = 6 + pdspDice(22);
    
        // wolfram
        wolfram.seq.randomAll();
        
        choice = pdspURan();
        if(choice<0.10f){   // 10%
            wolfram.seq.newThreshold = 4;
            wolfram.seq.newActiveOuts = 1; 
        }else 
        if(choice<0.30f){ // 20%
            wolfram.seq.newThreshold = 4;
            wolfram.seq.newActiveOuts = 2;            
        }else 
        if(choice<0.40f){ //10%
            wolfram.seq.newThreshold = 4;
            wolfram.seq.newActiveOuts = 4;            
        }else 
        if(choice<0.55f){ //15%
            wolfram.seq.newThreshold = 2;
            wolfram.seq.newActiveOuts = 1; 
        }else 
        if(choice<0.65f){ //10%
            wolfram.seq.newThreshold = 2;
            wolfram.seq.newActiveOuts = 2;               
        }else 
        if(choice<0.70f){ //5%
            wolfram.seq.newThreshold = 0;
            wolfram.seq.newActiveOuts = 1;            
        }else {
            wolfram.seq.newActiveOuts = 0;
            wolfram.seq.clearAll();
        }
        
    }
    
    masterplan_counter++;
    if(masterplan_counter == 8) masterplan_counter = 0;
}
