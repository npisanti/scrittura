
// Nicola Pisanti, GPLv3 License, 2016

#include "Element.h"

np::Element::Element(){
    state = false;
    instructions = "";
    minWidth = 100;
    minHeight = 20;
}

void np::Element::update(){}
void np::Element::draw(){}
void np::Element::position(int x, int y, int w, int h){}
void np::Element::setColor( ofColor color){}
void np::Element::controlDown(int x, int y){}
void np::Element::controlMoved(int x, int y){}
void np::Element::controlUp(int x, int y){}
    
bool np::Element::keyPressed( int key ){
    return false;
}

bool np::Element::getState() const {
    return state;
}

void np::Element::setState(bool enabled){
    state = enabled;
}

void np::Element::enable(){}
void np::Element::disable(){}

string np::Element::getInstructions() const{
    return instructions;
}

void np::Element::setInstructions(string instructions){
    this->instructions = instructions;
}

void np::Element::setMinDimensions(int w, int h){
    minWidth = w;
    minHeight = h; 
}

int np::Element::getMinWidth() const {
    return minWidth;
}

int np::Element::getMinHeight() const {
    return minHeight;
}


void np::Element::wmEnable(){
    state = true;
    enable();
}

void np::Element::wmDisable(){
    state = false;
    disable();
}
