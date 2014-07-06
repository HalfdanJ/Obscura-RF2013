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
    float age;
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
    void reassignParticle();
    void updateParticleSettings(BasicParticle& p);
    
    bool trace;
        
    //Particles
    vector<BasicParticle> particles;
    int iterate;
    
    //syphon to fbo
    ofFbo syphonFbo;
    ofPixels pixels;
    
    //Scene to fbo
    ofFbo pingPongFbo1;
    ofFbo pingPongFbo2;
    bool toggleFbo;
    
    //particle
    float pspeed;
    float psize;
    float pFadeWithAge;
    float pkill;
    float totalCount;
    float fade;
};

