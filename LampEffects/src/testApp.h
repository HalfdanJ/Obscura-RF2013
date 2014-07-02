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

#define numLamps 14

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void mouseDragged(int x, int y, int button);

    ofxOscSender osc;
    ofxOscReceiver oscReceiver;
    
    ofVec3f center;
    float circleRadius;
    float circleSpeed;
    
    int type = 2;
    
    vector<ofVec3f> lampPositions;
    
    float time;
    
};

