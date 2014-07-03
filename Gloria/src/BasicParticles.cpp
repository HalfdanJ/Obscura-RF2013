//
//  BasicParticles.cpp
//  Gloria
//
//  Created by vibber on 02/07/14.
//
//

#include "BasicParticles.h"

void BasicParticle::setup() {
    SetVelocity(1);
    angle = ofRandomf()*TWO_PI;
    age = 0;
    SetSize(20);
    fadeWithAge = 1;
}

void BasicParticle::SetVelocity(float baseVelocity) {
    vel.x = baseVelocity * ofRandomf();
    vel.y = baseVelocity * ofRandomf();
}

void BasicParticle::SetSize(float size) {
    radius = ofRandomf() * size;
}

void BasicParticle::draw() {
    int mapFactor = 20;
    ofSetColor(color,ofMap(age*fadeWithAge,0,mapFactor,255,0,true));
    
    ofFill();
    ofCircle(pos.x,pos.y,radius);
}

void BasicParticle::update() {
    angle += ofSignedNoise(pos.x, pos.y)*TWO_PI;
    vel.rotate(angle);
    oldpos = pos;
    pos += vel;
    age++;
}

///////////////////

void BasicParticles::setup(){
    name = "BasicParticles";
    oscAddress = "/basicparticles";
    
    myfbo.allocate(OUTWIDTH/10, OUTHEIGHT/10, GL_RGBA);

    
    pspeed = 5;
    psize = 30;
    pkill = 50;
    totalCount = 5000;
    fade = 5;
    pFadeWithAge = 0.03;
    
}

void BasicParticles::update(){
 
    for (int i=0; i<particles.size(); i++) {
        particles[i].update();
    }
  
}

void BasicParticles::draw(){;
    float lineWidth = 5;
    
    if (!trace) {
        ofClear(0,0,0);
    }

    //read syphon fbo
    myfbo.readToPixels(pixels);
    
    //Draw particles
    for (int i=0; i<particles.size(); i++) {
        particles[i].draw();
    }
    
    for (int i=0; i<pkill; i++) {
        createParticle();
    }
    
    while (particles.size()>totalCount) {
        particles.erase(particles.begin());
    }
    
    if (trace) {
    
        //Fade out the old stuff
        ofSetColor(0,0,0,fade);
        ofFill();
        ofRect(0,0,OUTWIDTH,OUTHEIGHT);
    }
    
    //Syphon fbo
    myfbo.begin();
    ofSetColor(255, 255, 255, 255);
    syphonIn->draw(0, 0, OUTWIDTH/10,OUTHEIGHT/10);
    myfbo.end();
    
    //myfbo.draw(0,0);
}

void BasicParticles::setGui(){
    
    gui->addSlider("/size/x", 5, 50, &psize);
    gui->addSlider("/speed/x", 0.1, 50, &pspeed);
    gui->addSlider("/kill/x", 50, 10000, &pkill);
    gui->addSlider("/totalcount/x", 500, 20000, &totalCount);
    gui->addSlider("/fadeparticle/x", 0, 1, &pFadeWithAge);
    gui->addSlider("/fade/x", 0, 10, &fade);
    gui->addToggle("/trace/x", &trace);
    
}

void BasicParticles::createParticle() {
    BasicParticle p;
    p.setup();
    p.fadeWithAge = pFadeWithAge;
    p.SetVelocity(pspeed);
    p.SetSize(psize);
    p.pos.set(OUTWIDTH * ofRandomf(), OUTHEIGHT * ofRandomf());
    
    if(pixels.getWidth() > p.pos.x/10 && pixels.getHeight() > p.pos.y/10 && p.pos.y >= 0 && p.pos.x >= 0){
        p.color = pixels.getColor(p.pos.x/10,p.pos.y/10);
    } else {
        p.color = ofColor(0);
    }

    particles.push_back(p);
}
