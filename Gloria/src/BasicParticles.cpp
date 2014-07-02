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
    radius = ofRandomf() * 50;
}

void BasicParticle::SetVelocity(float baseVelocity) {
    vel.x = baseVelocity * ofRandomf();
    vel.y = baseVelocity * ofRandomf();
}

void BasicParticle::draw() {
    int mapFactor = 20;
    ofSetColor(color,ofMap(age,0,mapFactor,255,0,true));
    
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
    
    pspeed = 1;
    density = 50;
    totalCount = 1000;
    fade = 5;
}

void BasicParticles::update(){
 
    for (int i=0; i<particles.size(); i++) {
        particles[i].update();
    }
  
}

void BasicParticles::draw(){;
    float lineWidth = 5;
    
    ofClear(0, 0);
    
    //read syphon fbo
    myfbo.readToPixels(pixels);
    
    //Draw particles
    for (int i=0; i<particles.size(); i++) {
        BasicParticle &p = particles[i];
        p.draw();
    }
    
    for (int i=0; i<density; i++) {
        createParticle();
    }
    
    while (particles.size()>totalCount) {
        particles.erase(particles.begin());
    }
    
    myfbo.begin();
    ofSetColor(255, 255, 255, 255);
    syphonIn->draw(0, 0, OUTWIDTH/10,OUTHEIGHT/10);
    myfbo.end();
    
    myfbo.draw(0,0);
}

void BasicParticles::setGui(){
    
    gui->addSlider("/speed/x", 0.1, 10, &pspeed);
    gui->addSlider("/density/x", 50, 500, &density);
    gui->addSlider("/totalcount/x", 500, 5000, &totalCount);
    gui->addSlider("/fade/x", 1, 10, &fade);
    
}

void BasicParticles::createParticle() {
    BasicParticle p;
    p.setup();
    p.SetVelocity(pspeed);
    p.pos.set(OUTWIDTH * ofRandomf(), OUTHEIGHT * ofRandomf());

    p.color = pixels.getColor(p.pos.x/10,p.pos.y/10);
    
    particles.push_back(p);
}
