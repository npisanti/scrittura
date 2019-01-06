
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "../framework/Element.h"

namespace np{

class WolframSequencer : public Element {
    
public:

    void setup( int steps, double division, int outputs, int rule, int generations, int threshold, string label, pdsp::ScoreSection & assignedSection );
    
    void draw() override;
    
    void position(int x, int y, int w, int h);
    void setColor( ofColor color );
    

    void randomAll();
    void clearAll();

    vector<int> initKeys;

    atomic<int> newThreshold;
    atomic<int> newRule;
    atomic<int> newActiveOuts;
   
    vector<float> bars;    
    
    string commands_info;

    void setLabel(string newLabel);

    int   threshold;

    atomic<int>  initCode;

    
    
    
    atomic<int> activeOuts;
    
    void processSequence(pdsp::Sequence & sequence) noexcept;
    
private:

    void newGeneration() noexcept;
    void wolframToPatterns() noexcept;
    void init(int code) noexcept;
    void initRule() noexcept;

    ofColor color;
    ofColor colorDim;
    
    // settings 
    int steps;
    int outputs;
    int generations;
    double stepDivision;
    double sequenceLength;
    float scaling;

    int uiX;
    int uiY;
    int uiWidth;
    int uiHeight;

    // internal vars
    int rule;
    vector<int> wrules;
    
    vector<vector<int>> wolframCA;
    int caGenerationIndex;

    string label;    
    
    int blink;

    float sideSize;
    float barHeight;
    float meterHeight;
    
    vector<float> values;
    
    float linearMax;
    
    pdsp::ScoreSection * section;
};

}
