
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "nplib.h"
#include "sub.h"
#include "flags.h"

class Ensemble {
    
public:

    void setup ( ofxPDSPEngine & engine, ofColor mainColor);
    
    void masterplan();
    
    int masterplan_counter;
    int sub_counter;
    
    pdsp::DimensionChorus ambientChorus;

    pdsp::BasiVerb verb;
    
    pdsp::Compressor1 percussionBus;
    pdsp::Compressor1 ambientBus;
    
    pdsp::KneeClipper percussionLimit;
    
    pdsp::Saturator1  percussionSatL;
    pdsp::Saturator1  percussionSatR;
    
    pdsp::LowCut      ambientCut;
    
    
    np::perMutante1   noise;
    np::WolframZaps   wolfram;
    np::SeqGDrone     cloud1;
    np::SeqGDrone     cloud2;

#ifdef SUBSONICS
    Sub               sub;
#endif

    pdsp::ScoreProcessor * score;
    
    pdsp::Sequence masterSequence;
  
  
    ofxPDSPValue key;
 
    np::VideoFader fadeToBlack;    
};
