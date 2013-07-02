/*
 *  testApp.h
 *  openFrameworks
 *
 *  ofxFlocking created by Jeffrey Crouse on 3/30/10.
 *
 */
 
#ifndef TESTAPP_H
#define TESTAPP_H

#include "ofMain.h"
#include "ofxFlocking.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void mouseDragged(int x, int y, int button);
	
	ofxFlocking flock;
    ofxOscSender osc;

};

#endif
