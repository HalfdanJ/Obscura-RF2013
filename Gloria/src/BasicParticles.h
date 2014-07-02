//
//  BasicParticles.h
//  Gloria
//
//  Created by vibber on 02/07/14.
//
//


#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"

class BasicParticle {
public:
    
    void setup();
    void draw();
    void update();
    void SetVelocity(float baseVelocity);
    void SetSize(float size);
    
    float baseVelocity;
    ofVec2f pos,vel,oldpos;
    float angle;
    ofColor color;
    int age;
    float fadeWithAge;
    float radius;
};

/////////////////////////

class BasicParticles : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void setGui();
    
    void createParticle();
    
    bool trace;
        
    //Particles
    vector<BasicParticle> particles;
    
    //syphon to fbo
    ofFbo myfbo;
    ofPixels pixels;
    
    //particle
    float pspeed;
    float psize;
    float pFadeWithAge;
    float pkill;
    float totalCount;
    float fade;
};

