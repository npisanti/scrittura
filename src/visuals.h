
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "nplib.h"
#include "sub.h"
#include "ensemble.h"
#include "cloudscriber.h"

class UnoVisuals : public np::Element {
    
public:

    void setup( pdsp::ScoreSection & masterSection, Ensemble & assignedEnsemble  );

    void position(int x, int y, int w, int h) override;
    
    void setColor( ofColor color );
    
    void update() override;
    void draw() override;

    bool drawBorder;

private:

    void drawGlyph(int x, int y, int index);
    
    Ensemble * ensemble;
    pdsp::ScoreSection * section;


    ofFbo visualBox;
    
    int visualsWidth; 
    int visualsHeight;
    int visualsX;
    int visualsY;
    
    int gy_master;
    
    int gwBase;
    int ghBaseSub;
    int ghBaseHi;
    int ghBase;
    int gDistance;
    int gwOffset;
    
    float gw_master;
    float gh_master;
    float gh_separation;

    int gliph_separation;
    
    vector<ofPoint> g_points;
    
    int visualStep;

    int visualsStepFade;
    
    ofColor mainColor;
    ofColor mainColorDim;

    CloudScriber scriber1;
    CloudScriber scriber2;

    
};
