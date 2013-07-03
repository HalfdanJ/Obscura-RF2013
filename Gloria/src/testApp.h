#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"
#include "ContentScene.h"

#include "LampWalker.h"
#include "QuickTrail.h"
#include "Triangles.h"
#include "GTess.h"
#include "PerlinWaves.h"
#include "ofxUI.h"

#include "mapping.h"

#define OSCPORT 9999

#define OUTWIDTH 6400
#define OUTHEIGHT 1200

class testApp : public ofBaseApp {
public:

    void setup();
    void update();
    void draw();
    void debugDraw();
    void drawGrid();

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
    ofxSyphonClient syphonIn;

    ofCamera cam;
    ofColor bg;
    ofFbo fboOut;

    Mapping mapping;

    ofxXmlSettings XML;
    
    // gui
    void setGUI();
    ofxUIScrollableCanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
    bool hideGUI;
    bool drawGuide;
    
    // Define all the scenes here
    LampWalker lampWalker;
    QuickTrail quickTrail;
    Triangles triangles;
    GTess tesselator;
    PerlinWaves perlinWaves;
    
    vector<ContentScene*> scenes;
    

    
};
