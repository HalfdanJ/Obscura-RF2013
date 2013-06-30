#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"

#include "LampWalker.h"

#include "ofxUI.h"

#include "mapping.h"

#define PORT 9999
#define OUTHEIGHT 1080
#define OUTWIDTH 1920




struct Walker {
    ofPolyline line;
    vector<ofVec2f> points;

    Corner * corner;

};


class testApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    void debugDraw();
    
    void exit();
        
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;

    ofxSyphonServer syphonOut;
    
    void setGUI(); 
    ofxUICanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
    ofCamera cam;
    ofColor bg;
    ofFbo fboOut;
    
    Mapping mapping;
    
  
    ofxXmlSettings XML;
    
    
    // stuff for specifc scenes

    vector<Walker> walkers;
    
    
    LampWalker lampWalker;
    
};
