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
    
    float baseVelocity;
    ofVec2f pos,vel,oldpos;
    float angle;
    ofColor color;
    int age;
    float radius;
};

class BasicParticles : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void setGui();
    
    void createParticle();
        
    //Particles
    vector<BasicParticle> particles;
    
    //syphon to fbo
    ofFbo myfbo;
    ofPixels pixels;
    
    //particle
    float pspeed;
    float density;
    float totalCount;
    float fade;
};

