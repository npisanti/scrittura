#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "nplib.h"
#include "sub.h"
#include "cloudscriber.h"
#include "ensemble.h"
#include "flags.h"
#include "visuals.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void layout();
        // pdsp modules
        
        ofxPDSPEngine   engine; 

        Ensemble ensemble;

        float chronoStart;
        float targetSeconds;

        bool ffmpeg_recording;
        
        int offsetY;
        int offsetX;
        int uiH;
        
        bool drawElements;
    

        ofColor mainColor;
        ofColor mainColorDim;
        
        ofColor altColor1;
        ofColor altColor2;

        int visualsWidth; 
        int visualsHeight;
        
        UnoVisuals visuals;


};



