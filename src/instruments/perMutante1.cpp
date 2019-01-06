
// Nicola Pisanti, GPLv3 License, 2016

#include "perMutante1.h"

void np::perMutante1::position(int x, int y, int w, int h) {
    uiX = x;
    uiY = y;
    uiWidth = w;
    uiHeight = h;
}

void np::perMutante1::setColor( ofColor color ){
    this->color = color;
    colorDim = color.getLerped(ofColor::black, 0.4); 
}

void np::perMutante1::setGain(float dBvalue) {
    gain = dB(dBvalue);
    gain >> click_ctrl;
}

void np::perMutante1::setup( string name, ofxPDSPValue & key_control, pdsp::ScoreSection & assignedSection ){

    label = name;
    quantize = 2.0;
    
    setMinDimensions(300, 30);
    string instr = " 1-9 set clicsk numbers ";
    setInstructions(instr);
    
    section = &assignedSection;
    
    key = &key_control;
    
    // set up section
    section->setCell(0, &sequence, pdsp::Behavior::Loop);
    section->enableQuantization( 0, 2.0 );
 

    pitches.resize(5);
    
    pitches[0] = 12.0f;
    pitches[1] = 24.0f;    
    pitches[2] = 36.0f;
    pitches[3] = 48.0f;
    pitches[4] = 53.0f;
    

    //SCORING

    permutator.symbolChances.resize(7);
    permutator.symbolChances[0] = 0.0f;
    permutator.symbolChances[1] = 0.42f;
    permutator.symbolChances[2] = 0.34f;
    permutator.symbolChances[3] = 0.07f;
    permutator.symbolChances[4] = 0.07f;
    permutator.symbolChances[5] = 0.06f;
    permutator.symbolChances[6] = 0.06f;
    
    permutator.setup(0, 32, 0.33f);

    sequence.code = [&] (){  
        permutator.update();
        // assign glyphs
        
        sequence.begin(16.0, 2.0);
        
        float sym_dyn;
        bool accent = true;
        for(int i=0; i<(int)permutator.steps.size(); ++i){
            if(accent){
                accent = false;
                sym_dyn = 0.95f;
            }else{
                accent = true;
                sym_dyn = 0.55f;
            }
            switch (permutator.steps[i]){
                case 1:
                    sequence.message( (double)i, 0.75f, 0); // gate
                    sequence.message( (double)i, 0.0,   1); // hold time
                    sequence.message( (double)i, 30.0,  2); // pitch 
                    sequence.message( (double)i, 124.0, 3); // filter
                    sequence.message( (double)i, 0.0f,  4); // reso
                break;
                    
                case 2:
                    sequence.message( (double)i, 1.0f, 0);  // gate
                    sequence.message( (double)i, 0.0,   1); // hold time
                    sequence.message( (double)i, 30.0,  2); // pitch 
                    sequence.message( (double)i, 124.0, 3); // filter
                    sequence.message( (double)i, 0.0f,  4); // reso
                break;
                
                case 3:
                    sequence.message( (double)i, sym_dyn, 0);  // gate
                    sequence.message( (double)i, 50.0,  1); // hold time
                    sequence.message( (double)i, pitches[0]+key->get(),  2); // pitch 
                    sequence.message( (double)i, 110.0, 3); // filter
                    sequence.message( (double)i, 0.0f,  4); // reso
                break;

                case 4:
                    sequence.message( (double)i, sym_dyn, 0);  // gate
                    sequence.message( (double)i, 50.0,  1); // hold time
                    sequence.message( (double)i, pitches[1]+key->get(),  2); // pitch 
                    sequence.message( (double)i, 115.0, 3); // filter
                    sequence.message( (double)i, 0.0f,  4); // reso
                break;
                
                case 5:
                    sequence.message( (double)i, sym_dyn, 0);  // gate
                    sequence.message( (double)i, 50.0,  1); // hold time
                    sequence.message( (double)i, pitches[2]+key->get(),  2); // pitch 
                    sequence.message( (double)i, 120.0, 3); // filter
                    sequence.message( (double)i, 0.5f,   4); // reso
                break;

                case 6:
                    sequence.message( (double)i, sym_dyn, 0);  // gate
                    sequence.message( (double)i, 50.0,  1); // hold time
                    sequence.message( (double)i, pitches[3]+key->get(),  2); // pitch 
                    sequence.message( (double)i, 125.0, 3); // filter
                    sequence.message( (double)i, 0.6f,   4); // reso
                break;

                case 7:
                
                    sequence.message( (double)i, sym_dyn, 0);  // gate
                    sequence.message( (double)i, 50.0,  1); // hold time
                    sequence.message( (double)i, pitches[4],  2); // pitch 
                    sequence.message( (double)i, 130.0, 3); // filter
                    sequence.message( (double)i, 0.6f,   4); // reso
                break;
                
                default: break;
            }
        }
        sequence.end();  
    };

    // ENDED SEQUENCE CODE -----------------------------------------------

    
    // patch up synth
    ampEnv.enableDBTriggering(-24.0f, 0.0f);
    
    section->out_value(3).setInitValue(150.0f);
    section->out_value(2).setInitValue(36.0f);
    section->out_value(1).setInitValue(0.0f);
    
    section->out_trig(0)  >> ampEnv;
    section->out_trig(0)  >> filterEnv;
    section->out_trig(0)  >> noiseL.in_reseed();
    section->out_trig(0)  >> noiseR.in_reseed();

    section->out_value(1) >> ampEnv.in_hold();
    section->out_value(1) >> filterEnv.in_hold();

    section->out_value(2) >> p2f >> clock.in_freq();
                                    clock.out_trig() >> noiseL.in_clock();
                                    clock.out_trig() >> noiseR.in_clock();
    section->out_value(3) >> p2filter;
    section->out_value(4) >> filterL.in_reso();
    section->out_value(4) >> filterR.in_reso();

                        p2filter >> filterL.in_cutoff();
                        p2filter >> filterR.in_cutoff();
    
                          noiseL >> filterL.out_lpf4() >> ampL;
    ampEnv.set(0.0f, 0.0f, 5.0f) >> ampL.in_mod();
                          
                          noiseR >> filterR.out_lpf4() >> ampR;
                          ampEnv >> ampR.in_mod();

    
    click_ctrl >> click_muteL.in_mod();
    click_ctrl >> click_muteR.in_mod();
    click_ctrl.setSlewTime(20.0f);

    ampL >> click_muteL;
    ampR >> click_muteR;

}


void np::perMutante1::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(uiX, uiY);
        ofSetColor(color); //cyan
        
        ofNoFill();
        ofDrawRectangle(0, 0, uiWidth, uiHeight);   
        string seq = label;
        seq += "  ///  ";
     
        for (int i=0; i<(int)permutator.steps.size(); i++){
            switch(permutator.steps[i]){
                case 0: 
                    seq+= " ";
                break;
                default:
                    seq+= to_string(permutator.steps[i]);
                break;
            }
        }
        
        seq+= "  ///  ";
        seq+= to_string(permutator.fill);
        seq+= " simboli";
        ofDrawBitmapString(seq, 5, (uiHeight/2) + 4);
        
    ofPopMatrix();
    ofPopStyle();
    
}

void np::perMutante1::enable() {
    section->launchCell(0, true, quantize);  
    setState(true);
}

void np::perMutante1::disable() {
    section->launchCell(-1, true, quantize);   
    setState(false);
}

bool np::perMutante1::keyPressed( int key ) {
    switch(key){
        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            int number = key + 1 - (int)'1' ;
            permutator.newFill = number*3 + pdspDice(3);  
            return true;
    }
    return false;
}




