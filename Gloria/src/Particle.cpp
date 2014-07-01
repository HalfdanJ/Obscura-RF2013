//
//  Particle.cpp
//  emptyExample
//
//  Created by vibber on 26/06/14.
// source:
// https://sites.google.com/site/ofauckland/examples/curly-moving-particles

#include "Particle.h"

void Particle::setup() {
    vel.x = 2*ofRandomf();
    vel.y = 2*ofRandomf();
    angle = ofRandomf()*TWO_PI;
    age = 0;
}

void Particle::draw() {
    int mapFactor = 20;
    ofSetColor(color,ofMap(age,0,mapFactor,255,0,true));
    ofLine(oldpos,pos);
}

void Particle::update() {
    angle += ofSignedNoise(pos.x, pos.y)*TWO_PI;
    vel.rotate(angle);
    oldpos = pos;
    pos += vel;
    age++;
}