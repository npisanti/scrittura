
// Nicola Pisanti, GPLv3 License, 2016

#include "WolframZaps.h"

void np::WolframZaps::position(int x, int y, int w, int h) {
    seq.position(x, y, w, h);
}


void np::WolframZaps::setColor( ofColor color ){
    seq.setColor(color);
}


void  np::WolframZaps::setup(  int steps, double division, int rule, int generations, int threshold, string label, pdsp::ScoreSection & assignedSection,ofxPDSPValue & key_control, std::initializer_list<float> tuning ){
    
    section = & assignedSection;
    
    setMinDimensions(700, 100);
    setInstructions( "\\ : clear  ///  1-8 : patterns  ///  r : randomize again ");
    
    quantize = 2.0;
    
    int outputs = tuning.size();
    seq.setup(steps, division, outputs, rule, generations, threshold, label, assignedSection);
    
    assignedSection.setCell(0, &wolframSequence, pdsp::Behavior::Loop);

    wolframSequence.code = [&]() noexcept {
        seq.processSequence( wolframSequence );
    };
    


    zaps.resize(outputs);

    int index = 0;
    for ( auto & pitch : tuning){
        section->out_trig(index) >> zaps[index].in("trig");
        pitch         >> zaps[index].in("pitch");
        key_control   >> zaps[index].in("pitch");
        index++;
    }    
    
    
    for ( int i=0; i<outputs; ++i ) {
        
        float gainScaling = -2.0f;
        zaps[i].setGain(  i * gainScaling );
        
        float pan;
        switch(i){
            case 0:
                pan = -0.3f;
            break;
            
            case 1:
                pan =  0.3f;            
            break;
            
            case 2:
                pan = -0.6f;
            break;
            
            case 3:
                pan = 0.6f;
            break;
            
            case 4:
                pan = -1.0f;
            break;
            
            case 5:
                pan = 1.0f;
            break;
            
            default: 
                if(i%2==0){ pan = -1.0f; }else{ pan = 1.0f; }
            break;
        }

        zaps[i] *  panL(pan) >> outL;
        zaps[i] *  panR(pan) >> outR;
    }   
    
    dB(-12.0f) >> outL.in_mod();
    dB(-12.0f) >> outR.in_mod();
}


void np::WolframZaps::draw() {
    seq.draw();
}

void np::WolframZaps::setGain(float dBvalue){
    dB(dBvalue) >> outL.in_mod();
    dB(dBvalue) >> outR.in_mod();
    
}

void np::WolframZaps::enable() {
    section->launchCell(0, true, quantize);  
}

void np::WolframZaps::disable() {
    section->launchCell(-1, true, quantize);  
}


bool np::WolframZaps::keyPressed( int key ) {
    switch (key){
        case '\\':
            seq.newActiveOuts = 0;
            seq.clearAll();
        return true;
        case '1':
            seq.newThreshold = 4;
            seq.newActiveOuts = 1; 
        return true;
        case '2':
            seq.newThreshold = 4;
            seq.newActiveOuts = 2;    
        return true;
        case '3':
            seq.newThreshold = 4;
            seq.newActiveOuts = 4;    
        return true;
        case '4':
            seq.newThreshold = 2;
            seq.newActiveOuts = 1; 
        return true;
        case '5':
            seq.newThreshold = 2;
            seq.newActiveOuts = 2;      
        return true;
        case '6':
            seq.newThreshold = 0;
            seq.newActiveOuts = 1;     
        return true;
        case '7':
        
        return true;
        case '8':
        
        return true;
        case 'r':
            seq.randomAll();
        return true;
        
    }
    return false;
}
