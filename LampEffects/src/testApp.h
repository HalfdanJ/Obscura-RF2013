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
};

