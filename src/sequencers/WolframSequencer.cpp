
// Nicola Pisanti, GPLv3 License, 2016

#include "WolframSequencer.h"

void np::WolframSequencer::position(int x, int y, int w, int h) {
    uiX = x;
    uiY = y;
    uiWidth = w;
    uiHeight = h;
    
    sideSize = (float)uiWidth / (float)(steps*outputs);

    meterHeight = 20;

    barHeight = (float)uiHeight - ((sideSize*0.5f)*generations) - sideSize*0.5f - meterHeight*2.0; 
}

void np::WolframSequencer::setColor( ofColor color ){
    this->color = color;
    colorDim = color.getLerped(ofColor::black, 0.4); 
}



void np::WolframSequencer::setup( int steps, double division, int outputs, int rule, int generations, int threshold, string label, pdsp::ScoreSection & assignedSection ) {

    section = &assignedSection;

    this->steps = steps; 
    this->outputs = outputs;   
    
    stepDivision = division;
    
    this->rule        = rule;  
    this->newRule.store( rule );
    this->generations = generations;
    this->threshold   = threshold;
    newThreshold = threshold;

    scaling = ((float)generations) / float(generations-threshold);

    setLabel(label);
          
    linearMax = 2.0f;

    //barHeight = uiHeight - ((sideSize/2)*generations) - sideSize/2 - meterHeight*3;
    values.resize(outputs);
    initKeys.resize(outputs);
    //int first = '1';
    int first = 1024;
    for(int i=0; i<(int)initKeys.size();++i){
        initKeys[i] = first;
        first++;
    }
    
    
    // ---- set up cellular automaton ----------
    blink = 0;

    newActiveOuts = 0;
    
    activeOuts = 0;

    wrules.resize(8);

    initRule();
    
    bars.resize(steps*outputs);
        
    caGenerationIndex = 0;
    wolframCA.resize(generations); // 8 different generatiosn
    for( vector<int> & v: wolframCA ){
        v.resize(steps*outputs);  // 64 pixels, we nee 16 steps for each generators + 2 guard points, 4 generator
    }
    
    initCode = -2;

    sequenceLength = ((double) steps) / stepDivision ;
    
}

    
void np::WolframSequencer::setLabel(string newLabel){
    newLabel += "  ///  ";
    newLabel += to_string(generations); 
    newLabel += " generazioni";  

    this->label = newLabel;
}



//--------------------------------------------------------------
void np::WolframSequencer::initRule() noexcept { // init wolfram rules from number bits
    int rulebits = rule;
    for(int i=0; i<8; ++i){ // inits the rules
        wrules[i] = rulebits & 0x0001;  // take just the lowest bit
        rulebits = rulebits>>1;             // bitshift by one
    }
    //ruleChanged = false;
}

//--------------------------------------------------------------
void np::WolframSequencer::newGeneration() noexcept{ // update the automaton
    
    int oldGeneration = caGenerationIndex;
    caGenerationIndex--;
    if(caGenerationIndex < 0){
        caGenerationIndex = generations - 1;
    }
    
    int max = (int) (wolframCA[caGenerationIndex].size());
    for(int x = 0; x < max; ++x){
        int xprev = x-1;
        if (xprev==-1) xprev = max-1;
        int xnext = x+1;
        if(xnext==max) xnext = 0;
        
        int cellRule = wolframCA[oldGeneration][xprev] * 4 
                    + wolframCA[oldGeneration][x] * 2 
                    + wolframCA[oldGeneration][xnext];
                    
        wolframCA[caGenerationIndex][x] = wrules[cellRule]; 
    }     

}

//--------------------------------------------------------------
void np::WolframSequencer::init( int code) noexcept{ // init the automaton with just an active cell in the top left


    switch(code){
        case -3: // INIT FIRST
            for( vector<int> & v: wolframCA ){
                for( int i=0; i<(int)v.size(); ++i){
                    v[i] = 0;
                }
            }        

            wolframCA[caGenerationIndex][0] = 1;

            for(int i=0; i<threshold; ++i) { newGeneration(); } 
        break;
        
        case -2: // INIT RANDOM
            for( vector<int> & v: wolframCA ){
                for( int i=0; i < (int)v.size(); ++i){
                    v[i] = 0;
                }
            }        
            
            for( int x=0; x < (int)(wolframCA[caGenerationIndex].size()); ++x ){
                // pdspChange(float value) controls the chance of having an alive cell
                if(x%2==0){
                    wolframCA[caGenerationIndex][x] = pdspChance(0.25f) ? 1 : 0;                     
                }else{
                    wolframCA[caGenerationIndex][x] = 0;          
                }

            }
            
            // always also make the first active
            wolframCA[caGenerationIndex][0] = 1;   
            
            for(int i=0; i<generations; ++i) { newGeneration(); } 
  
        break;
        
        case -1: // CLEAR
            for( vector<int> & v: wolframCA ){
                for( int i=0; i < (int)v.size(); ++i){
                    v[i] = 0;
                }
            }        
        break;

        default:
        if(code>=0 && code<outputs){
            
            int min = steps*code;
            int max = min + steps;
            
            for( vector<int> & v: wolframCA ){
                for( int i =min;  i < max; ++i){
                    v[i] = 0;
                }
            }        
            
            for( int x=min; x < max; ++x ){
                // pdspChange(float value) controls the chance of having an alive cell
                if(x%2==0){
                    wolframCA[caGenerationIndex][x] = pdspChance(0.25f) ? 1 : 0;                     
                }else{
                    wolframCA[caGenerationIndex][x] = 0;          
                }

            }
            
            // always also make the first active
            wolframCA[caGenerationIndex][min] = 1;   
            
        }
        break;
        
    }
}

void np::WolframSequencer::wolframToPatterns() noexcept{ // convert the automaton generations sum to float values
    int nextThreshold = newThreshold;
    if(nextThreshold != threshold){
        threshold = nextThreshold;
        scaling = ((float)generations) / float(generations-threshold);
    }
    
    
    for(int x =0; x< (int) bars.size(); ++x){
        int sum = -threshold;
        for(int y = 0; y<generations; ++y){
            sum += wolframCA[y][x];
        }
        if(sum<0) sum = 0;
        
        // ADD THRESHOLD PARAMETERS
        
        float intensity = (float)sum / (float)generations; 
        intensity *= scaling;
        if(intensity > 1.0f) intensity = 1.0f;
        bars[x] = intensity;
    }

}

//--------------------------------------------------------------
void np::WolframSequencer::processSequence( pdsp::Sequence & sequence ) noexcept{
        
    // code for sequencing wolfram CA values
    // this code is running in the audio thread
    // we are also updating the wolfram automaton inside this code
   
    // update rules
    if(rule != newRule.load() ){
        rule = newRule.load();
        initRule(); 
    }
    
    if(activeOuts != newActiveOuts){
        activeOuts.store(newActiveOuts.load());
    } 
    
    // update or init automaton
    if(initCode > -4){
        init(initCode);
        initCode = -4;
    }else{
        newGeneration();        
    }
    
    // convert the sums of the automaton generations to float values
    wolframToPatterns();
    // we use those float values to set the sequences steps
    // this sequence has 4 outputs, we have patched to the zaps
    sequence.begin( stepDivision, sequenceLength );
   
    for(int x=0; x<steps; ++x){
        
        float sum = 0.0f;
        for(int out=0; out < activeOuts; ++out){
            int bar = x + out*steps;
            if(bars[bar] > 0.0f ){
                values[out] = dB( - 18.0f + 18.0f * bars[bar] );
            }else{
                values[out] = 0.0f;
            }
            sum+= values[out];
        }

        if( sum > linearMax ){
            float multiplier = linearMax / sum;
            for(int out=0; out < activeOuts; ++out){
                values[out] *= multiplier;
            }
        }
        
        for(int out=0; out < activeOuts; ++out){
            sequence.message( (double)(x), values[out], out);
        }

        
    }
    sequence.end();
}

void np::WolframSequencer::draw(){
        
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(uiX, uiY);

        ofPushMatrix();
        ofSetColor(color); 


        string info = label;
        info += "  ///  soglia = ";
        info += to_string(threshold);
        info += "  ///  canali attivi = ";
        info += to_string(activeOuts);

        ofDrawBitmapString( info, 5, 14 );
        ofTranslate(0, meterHeight);
        
        // draw the automaton        
        int genIndex = caGenerationIndex;
        float sideHeight = sideSize*0.5f;
        for(int i=0; i<generations; ++i){
            
            for(int x = 0; x<(int)(wolframCA[caGenerationIndex].size()); ++x){
                if(wolframCA[genIndex][x] == 1){
                    ofSetColor(color);
                    ofFill();
                    ofDrawRectangle(sideSize*x, 0, sideSize, sideHeight);    
                }
                ofSetColor(colorDim);
                ofNoFill();
                ofDrawRectangle(sideSize*x, 0, sideSize, sideHeight);  
            } 
                   
            genIndex++; 
            if(genIndex==generations) genIndex = 0;
            ofTranslate(0, sideHeight);
            
        }
        
        // draw the bars
        ofTranslate(0, sideHeight);    

        float playhead = section->meter_playhead() / sequenceLength;
        int activeStep = playhead * steps;

        ofFill();
        for(int i=0; i<activeOuts; ++i){
            for(int x = 0; x<steps; x++){
                if ( x == activeStep ){
                    ofSetColor(color);
                }else{
                    ofSetColor(colorDim);
                }
                int bar = x+i*steps;
                float height = bars[bar] * barHeight;
                float yy = barHeight - height;
                ofDrawRectangle(sideSize*bar, yy, sideSize, height); 
            }
        }

        ofSetColor(color);
        ofNoFill();    
        for(int x = 0; x<(int)bars.size(); ++x){
            ofDrawRectangle(sideSize*x, 0, sideSize, barHeight); 
        }
        
        ofTranslate(0, barHeight);  

        // draw the playhead and envelope meters
        
        ofPushMatrix();    
        for(int i=0; i<outputs; ++i){

            float x = playhead * (float)(sideSize*steps);
            
            if(i<activeOuts){
                ofSetColor(colorDim);
                ofFill();
                ofDrawRectangle(0, 0, x, meterHeight);
                ofSetColor(color);               
                ofDrawLine(x, 0, x, meterHeight); 
            }


            ofSetColor(color);   
         
            string label = "canale ";
            label+= to_string(i);
            
            ofDrawBitmapString(label, 5, meterHeight-5);    

            ofNoFill();

            ofDrawRectangle( 0, 0, sideSize*steps, meterHeight );
            
            ofTranslate(sideSize*steps,0);
            
        }
        ofPopMatrix();

        blink++;
        if(blink==8){ blink = 0; }

        ofPopMatrix();
        ofSetColor(color);
        
        ofNoFill();
        ofDrawRectangle(0, 0, uiWidth, uiHeight);

    ofPopMatrix();
    ofPopStyle();
    
    
}

void np::WolframSequencer::randomAll(){
    initCode = -2;   
}

void np::WolframSequencer::clearAll(){
    initCode = -1;
}

