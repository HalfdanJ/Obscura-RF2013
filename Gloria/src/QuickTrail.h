    //
//  QuickTrail.h
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"

struct Walker {
    ofPolyline line;
    //vector<ofVec2f> points;
    vector<Corner*> corners;

    Corner * dst;
};

class QuickTrail : public ContentScene {

public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    vector<Walker> walkers;
    
    int numtrails = 3;
    float length;
    int long lastadded;
    
    float circleRadius;
    
    bool random;
    bool clear;
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    float time;
    float interval = 1;
};
