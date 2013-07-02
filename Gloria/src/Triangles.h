//
//  LampWalker.h
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "ofxSyphon.h"

class Triangles : public ContentScene {
    
public:
    
    // all scenes must draw to an fbo called fbo
    ofFbo fbo;
    bool enabled;
    
    void setup();
    void draw();
    
    void debugDraw();
    
    void update();    
    float pct;
    
    Mapping * mapping;
    ofxSyphonClient * syphon;
    
    map<InputTriangle*, vector<InputTriangle*>  > subTriangles;
    
    void divide(InputTriangle * triangle);
    
    void collapse(InputTriangle * triangle);
};

