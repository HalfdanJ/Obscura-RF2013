//
//  Gloria
//
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "Particle.h"

class CurlyFur : public ContentScene {

public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    void initCam();
    void createParticle();
    
    //Particles
    vector<Particle> particles;
    
    //Camera
    ofVideoGrabber 		vidGrabber;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    
    //misc
    ofFbo myfbo;
    
};
