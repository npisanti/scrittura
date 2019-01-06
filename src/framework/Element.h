
#pragma once
#include "ofMain.h"
#include "ofxPDSP.h"
#include <atomic>

// an element rapresents a sequence, an instrument, a controller or a complete audio-visual generator
// potentially more element can be drawn one on the top of the other, so you can think also of "controller" elements

namespace np{

class Element {
    friend class ElementTWM;
    
public:

    Element();

    virtual void update();
    virtual void draw();
    virtual void position(int x, int y, int w, int h);
    virtual void setColor( ofColor color);
    
    virtual void controlDown(int x, int y);
    virtual void controlMoved(int x, int y);
    virtual void controlUp(int x, int y);
    virtual bool keyPressed( int key );

    virtual void enable();
    virtual void disable();
    
    void setMinDimensions(int w, int h);
    
    bool    getState() const;
    string  getInstructions() const;
    int     getMinWidth() const;
    int     getMinHeight() const;
    
        
protected:

    void setState(bool enabled);
    void setInstructions(string instructions);

private:

    void wmEnable();
    void wmDisable();

    bool state;
    string instructions;
    
    int minWidth;
    int minHeight;
    
};

}
