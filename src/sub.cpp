
// Nicola Pisanti, GPLv3 License, 2016

#include "sub.h"

void Sub::position(int x, int y, int w, int h) {
    uiX = x;
    uiY = y;
    uiWidth = w;
    uiHeight = h;
    
}

void Sub::setColor( ofColor color ){
    subColor = color;
}


void Sub::setup(string name,  ofxPDSPValue & key_control, pdsp::ScoreSection & assignedSection, np::WolframSequencer & wtarget, np::SymbolPermutator & ptarget){
    
    label = name;
    
    setMinDimensions(300, 30);
    string instr = "1 : dumdum  ///  2 : wolfram  ///  3 : mutante";
    setInstructions(instr);

    steps = 32;
    bars.resize(steps);
    
    target = &wtarget;
    clicks = &ptarget;
    section = &assignedSection;

    seqs.resize(4);
    
    section->resizeCells(5);
    section->setCell(0, &seqs[0], pdsp::Behavior::Loop);
    section->setCell(1, &seqs[1], pdsp::Behavior::Loop);
    section->setCell(2, &seqs[2], pdsp::Behavior::Loop);
    section->setCell(3, &seqs[3], pdsp::Behavior::Loop);

    seqs[0].setLength(2.0);
    seqs[0].code = [&](){
        for(int i=0; i<steps; ++i){ bars[i] = 0.0f; }
    };

    seqs[1].code = [&](){
        float o = -1.0f;
        seqs[1].set({1.0f, 1.0f, o, o, o, o, o, o, 1.0f }, 8.0, 2.0);
        for(int i=0; i<steps; ++i){ bars[i] = 0.0f; }
        bars[0]  = 1.0f;
        bars[2]  = 1.0f;   
        bars[16] = 1.0f;   
    };
    
    
    seqs[2].code = [&](){
        
        for(int i=0; i<steps; ++i){ bars[i] = 0.0f; }
        
        float threshold = 0.70f;        
        
        for(int out = 0; out<2; out++){
            for(int i=0; i<steps; ++i){
                float value = target->bars[i+(out*steps)];
                
                if((value > threshold) && (bars[i]<value)){
                    bars[i] = value;
                }
            }
        }

        seqs[2].begin(16.0, 2.0);
        for(int i=0; i<steps; ++i){
            if(bars[i] > 0.0f){
                seqs[2].message((double)i, bars[i], 0);
            }else if(i==0){
                bars[i] = 1.0f;
                seqs[2].message((double)i, 1.0f, 0);                
            }
        }
        seqs[2].end();
    };

    seqs[3].code = [&](){
        
        for(int i=0; i<steps; ++i){ bars[i] = 0.0f; }
        
        seqs[3].begin(16.0, 2.0);
        int count = 0;
        
        for(int i=0; i<steps; ++i){
            
            if(clicks->steps[i] == 2 ){
                if(count==0 && i%2==0){
                    bars[i] = 1.0f;
                    seqs[3].message((double)i, 1.0f, 0);                    
                }
                count++;
                if(count == 2){ count = 0; }
                
            }

        }
        seqs[3].end();
    };

    // patch synth
    
    section->out_trig(0)  >> aEnv;
    section->out_trig(0)  >> pEnv;

                            aEnv.set(30.0, 200.0f, 300.0f) >> amp.in_mod();        
                             key_control >> osc.in_pitch();
                                   24.0f >> osc.in_pitch() >> amp;
    pEnv.set(30.0f, 0.0f, 500.0f) * 12.0f >> osc.in_pitch();    
 
    bar * 0.25f >> pEnv.in_release();
    bar * (0.25f * 0.6f) >> aEnv.in_hold();
    bar * (0.25f * 0.4f) >> aEnv.in_release();
 
}
    
    
void Sub::draw(){
    
    ofPushMatrix();
        ofTranslate(uiX, uiY);
        ofSetColor(subColor); //cyan
        
        string info = label;
        info += "  ///  pattern = ";
      
        switch(section->meter_current()){
         
            case 0:  info += "vuoto"; break;
            case 1:  info += "dumdum"; break;
            case 2:  info += "wolfram"; break;
            case 3:  info += "mutante"; break;
            default: info += "vuoto"; break;
            
        }

        ofDrawBitmapString(info, 5, (uiHeight/2) + 4);
        
        ofNoFill();
        ofDrawRectangle(0, 0, uiWidth, uiHeight);   
        

    ofPopMatrix();
      
}


void Sub::enable() {
    setState(true);
}

void Sub::disable() {
    section->launchCell(0, true, 2.0);   
    setState(false);
}

bool Sub::keyPressed( int key ) {
    switch(key){
        case 1: 
            section->launchCell(1, true, 2.0);      
            return true;
       
        case 2: 
            section->launchCell(2, true, 2.0);      
            return true;
       
        case 3: 
            section->launchCell(3, true, 2.0);      
            return true;
    }
    return false;
}
