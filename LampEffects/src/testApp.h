/*
 *  testApp.h
 *  openFrameworks
 *
 *  ofxFlocking created by Jeffrey Crouse on 3/30/10.
 *
 */
 
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxFlocking.h"

#define numLamps 14

#define OSCSENDPORT 8000

#define OSCCLIENTONE "Rumba.local"
#define OSCCLIENTTWO "HalfdanJ-iPad.local"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void mouseDragged(int x, int y, int button);

    ofxOscSender osc;
    ofxOscReceiver oscReceiver;
    
    ofxOscSender * clientOne;
    ofxOscSender * clientTwo;
    
    vector<ofxOscSender *> clients;
    
    ofVec3f center;
    float circleRadius = 0;
    float circleSpeed = 0;
    
    int type = 2;
    
    vector<ofVec3f> lampPositions;
    
    float time = 0;
    float dim = 0;
    
    bool masterEnable = false;
    bool flockEnable = false;
    ofxFlocking flock;
};

