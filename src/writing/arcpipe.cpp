
// Nicola Pisanti, GPLv3 License, 2016

#include "arcpipe.h"

void np::arcpipe(int x1, int y1, int x2, int y2, float amount, float roundness, bool clockwise){
    
    float w = abs(x2 - x1);
    float h = abs(y2 - y1);
    
    amount = (amount<0.0f) ? 0.0f : amount; // input sanitizing 
    amount = (amount>1.0f) ? 1.0f : amount; // input sanitizing 
    roundness = (roundness<0.0f) ? 0.0f : roundness; // input sanitizing 
    roundness = (roundness>1.0f) ? 1.0f : roundness; // input sanitizing 

    int orientation = 0;
    if(x2 < x1 ){
        orientation+=1;
    }
    if(y2 < y1 ){
        orientation+=2;
    }
    
    switch(orientation){
        case 1 : case 2:
        {
            float temp = h;
            h = w;
            w = temp;         
        }
        break;
    }

    float lesser = (w<h) ? w : h;
    
    float radius = lesser * roundness;
    
    float arcLen = M_PI_2 * radius ; // as the angle is always 90,  r * pi * 2 * 90 / 360  
  
    float len = w + h - radius - radius + arcLen;

    float destLen = len*amount;
    
    float horizlen = w - radius;
    float vertlen = h - radius;
 
    float lenNow = 0.0f;

    float centerX;
    float centerY;
    float nextLen;
    int state;
    
    if(clockwise){
        nextLen = horizlen;   
        state = 0;   
        centerX = w - radius;
        centerY = radius;
    }else{
        nextLen = vertlen;   
        state = 3;   
        centerX = radius;
        centerY = h-radius;     
    }


    ofPolyline path;
    path.clear();
    
    //ofDrawRectangle(centerX+x1, centerY+y1 , 10, 10);
    
    while ( nextLen < destLen && state!=2 && state!=5 ){
        switch(state){
            
            // clockwise
            case 0:
            path.addVertex(0, 0);
            path.lineTo(centerX, 0);
            lenNow = nextLen;
            nextLen += arcLen;
            break;
            
            case 1:
            path.arc(centerX, centerY, radius, radius, -90, 0);
            lenNow = nextLen;
            nextLen += vertlen;            
            break;

            // counterclockwise
            case 3:
            path.addVertex(0, 0);
            path.lineTo(0, centerY);
            lenNow = nextLen;
            nextLen += arcLen;
            break;
            
            case 4:
            path.arcNegative(centerX, centerY, radius, radius, 180, 90);
            lenNow = nextLen;
            nextLen += horizlen;            
            break;
            
        }
        
        state++;
    }

    float pct = (destLen-lenNow) / (nextLen-lenNow);

    switch(state){
        
        // clockwise
        case 0:
            path.addVertex(0, 0);
            path.lineTo( centerX*pct, 0);
        break;
        
        case 1:
            path.arc(centerX, centerY, radius, radius, -90, (pct*90.f) -90.0f);        
        break;
        
        case 2:
            path.lineTo( w, centerY + (h-centerY)*pct );
        break;
        
        // counter-clockwise
        case 3:
            path.addVertex(0, 0);
            path.lineTo(0, centerY*pct);
        break;
        
        case 4:
            path.arcNegative(centerX, centerY, radius, radius, 180, 180 - (pct*90.f));        
        break;
        
        case 5:
            path.lineTo( centerX + (w-centerX)*pct, h );
        break;
        
    }    
    
    ofPushMatrix();
    ofTranslate(x1, y1);
    
    switch(orientation){
        case 1:  
            ofRotate(90);
        break;
        
        case 2:  
            ofRotate(-90);
        break;
        
        case 3: 
            ofRotate(180);
        break;
        
        default: break;
    }
    
    path.draw();    
    
    ofPopMatrix();
    
}
