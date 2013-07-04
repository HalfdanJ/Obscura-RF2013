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
    vector<ofVec2f> points;
    Corner * corner;
};

class QuickTrail : public ContentScene {

public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    Mapping * mapping;
    vector<Walker> walkers;
    
    int numtrails = 3;
    int length;
    
};
