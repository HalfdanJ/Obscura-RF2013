//
//  TriMasker.h
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 03/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"


class TriBlobs : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    void setGui();
    
    void parseOscMessage(ofxOscMessage * m);
    
    float smoothness;
    float spikeyness;
    
    bool animate;
    //float fillgradient; - maybe new scene - mask scene
    
};
