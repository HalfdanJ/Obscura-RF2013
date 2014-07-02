//
//  Particle.cpp
//  emptyExample
//
//  Created by vibber on 26/06/14.
// source:
// https://sites.google.com/site/ofauckland/examples/curly-moving-particles

#include "Particle.h"

void Particle::setup(float length) {
    vel.x = length * ofRandomf();
    vel.y = length * ofRandomf();
    angle = ofRandomf()*TWO_PI;
    age = 0;
}

void Particle::draw(float lineWidth) {
    int mapFactor = 20;
    ofSetColor(color,ofMap(age,0,mapFactor,255,0,true));

    ofSetLineWidth(lineWidth*(20-age)*0.1);
    ofLine(oldpos,pos);
}

void Particle::update() {
    angle += ofSignedNoise(pos.x, pos.y)*TWO_PI;
    vel.rotate(angle);
    oldpos = pos;
    pos += vel;
    age++;
}