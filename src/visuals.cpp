
// Nicola Pisanti, GPLv3 License, 2016

#include "visuals.h"

void UnoVisuals::setup( pdsp::ScoreSection & masterSection, Ensemble & assignedEnsemble ) {
    ensemble = & assignedEnsemble;
    section = & masterSection;
    
    drawBorder = true;
    
    g_points.resize(4);
    g_points[0].x = 0.5f;
    g_points[0].y = 0.0f;
    g_points[1].x = 1.0f;
    g_points[1].y = 0.5f;
    g_points[2].x = 0.5f;
    g_points[2].y = 1.0f;
    g_points[3].x = 0.0f;
    g_points[3].y = 0.5f;

    visualsStepFade = 10;
    
}


void UnoVisuals::position(int x, int y, int w, int h){
    
    visualsX = x;
    visualsY = y;
    visualsWidth = w-40;
    visualsHeight = h-40;
    
    visualBox.allocate(visualsWidth, visualsHeight, GL_RGBA );
    visualBox.begin();
        ofClear(255,255,255, 0);    
    visualBox.end();
    
    visualStep = -1; // so it will draw the first step
    // still not working???
    
    gDistance = visualsWidth / 32;

    gwBase    = gDistance * 5 / 8;
    ghBase    = gDistance * 5 / 8;    
    //ghBaseSub = gDistance * 4 / 8;    
    ghBaseSub = gDistance / 8;    
    ghBaseHi  = gDistance * 2 / 8;    
    
    gy_master = (visualsHeight/2 - ghBase -(ghBaseSub*1.5f));

    gwOffset  = (gDistance - gwBase) / 2;

    if(visualsWidth > 1080 ){
        gliph_separation = 3;
    }else{
        gliph_separation = 2;
    }
    

    
    gw_master = gwBase;
    
    #ifdef SUBSONICS
    gh_master = ghBase+ghBaseSub*4+ghBaseHi;
    #else
    gh_master = ghBase+ghBaseSub+ghBaseHi;
    #endif

    gh_separation = gDistance * 1 / 8;
}


void UnoVisuals::setColor( ofColor color ){
    mainColor = color;
    mainColorDim =  color.getLerped(ofColor::black, 0.4); 
}


void UnoVisuals::update() {
    
    visualBox.begin();
    
    ofNoFill();

    
    int newStep = section->meter_playhead() * 16.0f;
    if (newStep == 32) newStep = 31;

    while(visualStep!=newStep){
        visualStep++;
        if(visualStep==32) visualStep = 0;
        
        drawGlyph(gwOffset + gDistance*visualStep, gy_master, visualStep);
        ofFill();         
        ofSetColor(0, 0, 0, visualsStepFade);
        ofDrawRectangle(0, 0, visualsWidth, visualsHeight);    
    
    }    

    //ofTranslate( 0, gh_master + gy_master);
    ofTranslate( 0, gh_master + gh_separation + gy_master);


    float playhead = section->meter_playhead() * 0.5;
   
    float interp = ofMap(ensemble->cloud1.meter_filter(), 70.0f, 120.0f, 0.8f, 0.0f, true);
    ofColor col = mainColor.getLerped(ofColor::black, interp); 
    ofSetColor(col);
    scriber1.draw( ensemble->cloud1.getCloud(), ensemble->cloud1.meter_seq(), playhead, visualsWidth, ghBase);
    
    interp = ofMap(ensemble->cloud2.meter_filter(), 70.0f, 120.0f, 0.8f, 0.0f, true);
    col = mainColor.getLerped(ofColor::black, interp); 
    ofSetColor(col);
    scriber2.draw( ensemble->cloud2.getCloud(), ensemble->cloud2.meter_seq(), playhead, visualsWidth, ghBase);


    ofFill();
    ofSetColor(0, 0, 0, visualsStepFade/3);
    ofDrawRectangle(-3, -3, visualsWidth+6, 46);    
    
    visualBox.end();    
    
}


void UnoVisuals::draw() {
    ofPushMatrix();
        ofTranslate(visualsX, visualsY);
        ofSetColor(255);
        visualBox.draw(20, 20);
        if(drawBorder){
            ofNoFill();
            ofSetColor(mainColor); 
            ofDrawRectangle(0,0, visualsWidth+40, visualsHeight+40);
        }
    ofPopMatrix();
}


//--------------------------------------------------------------
void UnoVisuals::drawGlyph(int x, int y, int index){
    
        ofPushMatrix();
        ofTranslate(x, y);
        
        ofSetColor(0);
        ofFill();
        ofDrawRectangle(-1, -1, gw_master+2, gh_master+2); // erase
        
        ofSetColor(mainColor);
    
        ofNoFill();
        
        // draw clicks serifs here...........................................

        int switcher = ensemble->noise.permutator.steps[index];
        int clickw = gwBase/5;
        
        switch(switcher){
            case 0: break;
            
            case 1: // light click
                ofDrawLine(0, clickw/2, gwBase/2, clickw/2);
            break;
            
            case 2: // hard click
                ofDrawLine(0, clickw/2, gwBase, clickw/2);                    
            break;
            
            default:
                ofPushMatrix();
                for(int i=0; i<(switcher-2); i++){
                    ofDrawRectangle(0, 0, clickw, clickw);
                    ofTranslate(clickw, 0);
                }
                ofPopMatrix();
            break;
        }            

        ofTranslate(0, ghBaseHi);
        
        
        // zaps body .............................(add checks for knowing if zaps are running)
        for(int out=0; out<4; out++){
 
            float roundness = 0.1f;
            //float roundness = 0.0f;
                            
            float val = ensemble->wolfram.seq.bars[index + out*32];
            
            if(val!=0.0f && out<ensemble->wolfram.seq.activeOuts){

                int gw = gwBase - (gliph_separation*out*2);
                int gh = ghBase - (gliph_separation*out*2);
                
                int gx = gliph_separation*out;
                int gy = gliph_separation*out;      
      
                ofPushMatrix();
                ofTranslate(gx, gy);                
                
                float v4   = val*4.0f;
                int   v4_i = (int) v4;
                float v4_fract = v4 - v4_i;

                int gi;
                int gi_next;            
                switch(out){
                    case 0:
                        gi = 2; 
                        break;
                    case 1:
                        gi = 0; 
                        break;
                    case 2:
                        gi = 1; 
                        break;
                    case 3:
                        gi = 3; 
                        break;
                }                
                
                for (int i=0; i<v4_i; ++i){
                    gi_next = gi+1;
                    if (gi_next == 4) gi_next = 0;
                    float x1 = g_points[gi].x * gw;
                    float y1 = g_points[gi].y * gh;
                    float x2 = g_points[gi_next].x * gw;
                    float y2 = g_points[gi_next].y * gh;
                    np::arcpipe(x1, y1, x2, y2, 1.0f, roundness, true);
                    gi = gi_next;
                } 
            
                gi_next = gi+1;
                if (gi_next == 4) gi_next = 0;

                float x1 = g_points[gi].x * gw;
                float y1 = g_points[gi].y * gh;
                float x2 = g_points[gi_next].x * gw;
                float y2 = g_points[gi_next].y * gh;
                np::arcpipe(x1, y1, x2, y2, v4_fract, roundness, true);                  

                ofPopMatrix();                                   
            }

        }

        #ifdef SUBSONICS
        // sub graphic ...............................(add checks for knowing if sub is running)
        if(ensemble->sub.bars[index] > 0.0f){
            ofTranslate(0, ghBase);
            
            float dest_x = gwBase*0.25;
            float dest_y = ghBaseSub*4.0f*0.5f;
            float rect_y = ghBaseSub*4.0f*0.33333f;
            float rect_w = gwBase*0.5f;
            
            ofDrawLine(0, 0, 0, dest_y);
            ofDrawLine(0, dest_y, dest_x, dest_y);
            ofDrawRectangle(dest_x, rect_y, rect_w, rect_y);            
        }
        #endif
        
    ofPopMatrix();
}
        
