/*
 *  ofxFlocking.cpp
 *  flock
 *
 *  Created by Jeffrey Crouse on 3/30/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *  Modified by Rick Companje
 */

#include "ofxFlocking.h"

void ofxFlocking::update() {
	for(int i=0; i<boids.size(); i++) {
		boids[i].update(boids);
	}
}

void ofxFlocking::draw() {
	for(int i=0; i<boids.size(); i++) {
		boids[i].draw();
	}
}

void ofxFlocking::addBoid() {
	boids.push_back(Boid());
}

void ofxFlocking::addBoid(int x, int y) {
	boids.push_back(Boid(x,y));
}