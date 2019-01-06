
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    np::seed_oracle();

    ffmpeg_recording = false;

    drawElements = false;

    // COLORS ------------------------------

    mainColor = ofColor(255, 80, 80);
    altColor1 = ofColor(255, 30, 30);
    altColor2 = ofColor(255, 100, 70);

    mainColorDim =  mainColor.getLerped(ofColor::black, 0.4); 

    //------------------- graphics --------------
    ofBackground(0);
    ofSetFrameRate(30);
    ofDisableAntiAliasing();
    np::linux_no_cursor_fix();

    // -------------------------VISUALS SETUP--------------------------
    
    ensemble.fadeToBlack.setFadeTime( 6200.0f); 
    ensemble.fadeToBlack.setFadePoint( 0.0f, true );
        
#ifndef SCREENCAST     
    ensemble.fadeToBlack.fadeIn();
#endif
    
    visuals.setup( engine.score.sections[0], ensemble );    
    ensemble.setup ( engine, mainColor );    

    visuals.setColor( mainColor );    
    ensemble.wolfram.setColor(mainColor);  
    ensemble.noise.setColor(mainColor);
    ensemble.sub.setColor(mainColor);
    ensemble.cloud1.setColor(mainColor);
    ensemble.cloud2.setColor(mainColor);

    layout();

    engine.score.play();
}

//--------------------------------------------------------------
void ofApp::layout(){

    if (drawElements){

        visuals.drawBorder = true;

        int y = 20;

#ifdef SCREENCAST  
        visualsHeight = 720 - (40 + 40 + 160 + 30 + 90 + 90 );
        visualsWidth = 1280 - 40; 
        offsetX = 0;   
        
        visuals.position( 20, y, visualsWidth, visualsHeight);

#elif defined( USE_PROJECTOR )

        visualsWidth = PROJECTOR_WIDTH;
        visualsHeight = PROJECTOR_HEIGHT;
        offsetX = ( WIDTH- visualsWidth - 40 ) / 2;
        
        visuals.drawBorder = false;
        
        y = 0;
        visuals.position( 0, y, visualsWidth, visualsHeight);
        
#else 
        
        if( ofGetWidth() > 1366){
            visualsWidth = 1326;
        }else{
            visualsWidth = ofGetWidth() - 40;   
        }
        visualsHeight = ofGetHeight() - (40 + 40 + 160 + 30 + 90 + 90 );
        offsetX = ( ofGetWidth() - visualsWidth - 40 ) / 2;

        visuals.position( 20, y, visualsWidth, visualsHeight);
#endif

        y += 10 + visualsHeight; 

        // --------------- elements---------------------------

        ensemble.wolfram.position( 20, y, visualsWidth, 160);
        y+=170;
        
        #ifdef SUBSONICS
        int two_width = (visualsWidth - 10) / 2;   
        ensemble.noise.position( 20, y, two_width, 30);
        ensemble.sub.position( two_width + 30, y, two_width, 30);     
        #else
        ensemble.noise.position( 20, y, visualsWidth, 30);
        #endif

        y+=40;
        
        ensemble.cloud1.position( 20, y, visualsWidth, 90 );

        y+=100;
        
        ensemble.cloud2.position( 20, y, visualsWidth, 90 );

    }else{
        
        visuals.drawBorder = false;

#ifdef SCREENCAST  
    
        visualsHeight = 680;
        visualsWidth = 1240;   
        offsetX = 0; 

        visuals.position( 20, 20, visualsWidth, visualsHeight);

        
#elif defined( USE_PROJECTOR )        
    
        visualsWidth = PROJECTOR_WIDTH;
        visualsHeight = PROJECTOR_HEIGHT;
        offsetX = (( WIDTH - visualsWidth - 40 ) / 2) -1;

        visuals.position( 0, 0, visualsWidth, visualsHeight);

    
#else
        if( ofGetWidth() > 1366){
            visualsWidth = 1326;
        }else{
            visualsWidth = ofGetWidth() - 40;   
        }
        visualsHeight = ofGetHeight() - 40;
        offsetX = (( ofGetWidth() - visualsWidth - 40 ) / 2) -1;

        visuals.position( 20, 20, visualsWidth, visualsHeight);

#endif


    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    ensemble.fadeToBlack.update();
    visuals.update();
}




//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    //ofSetColor( mainColor );
    
    ofPushMatrix();
    ofTranslate(offsetX, 0);
    
    visuals.draw();
    
    if(drawElements){
        ofSetColor(mainColor);
        ofDrawBitmapString( "scrittura::uno", 25, 35 );
        
        ensemble.cloud1.draw();
        ensemble.cloud2.draw();
        ensemble.wolfram.draw();
        ensemble.noise.draw();
        #ifdef SUBSONICS
        ensemble.sub.draw();    
        #endif    
    }
    
    ensemble.fadeToBlack.draw();

    ofPopMatrix();
    
#ifdef SCREENCAST 
    ofSetColor(mainColor);
    int y = ofGetHeight() - 20;
    if(ffmpeg_recording) ofDrawBitmapString( "REGISTRO!!!", 20, y-7);
#endif

#ifdef USE_PROJECTOR
    ofTranslate(1920, -20);
    visuals.draw();

#endif

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'i':
            ensemble.fadeToBlack.fadeIn();
        break;
        
        case 'o':
            ensemble.fadeToBlack.fadeOut();
        break;
        
        case OF_KEY_RETURN: // pause / play
            if(engine.score.isPlaying()){
                engine.score.stop();
            }else{
                engine.score.play();
            }
        break;
        
        case 'e':
            drawElements = drawElements ? false : true;
            layout();
        break;
        
#ifdef SCREENCAST        
        case 'r':
            np::ffmpeg_record(1280, 720);
            ffmpeg_recording = true;
        break;

        case 's':
            np::ffmpeg_stop();
            ffmpeg_recording = false;
        break;
#endif

        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


