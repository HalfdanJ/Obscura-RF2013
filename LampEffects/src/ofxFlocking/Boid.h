/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef BOID_H
#define BOID_H

#include "ofMain.h"

class Boid {
public:
	Boid();
	Boid(int x, int y);
	
	void update(vector<Boid> &boids);
	void draw();
	
    void seek(ofVec2f target);
    void avoid(ofVec2f target);
    void arrive(ofVec2f target);
	
    void flock(vector<Boid> &boids);
    
	ofVec2f steer(ofVec2f target, bool slowdown);	
	
	ofVec2f separate(vector<Boid> &boids);
	ofVec2f align(vector<Boid> &boids);
	ofVec2f cohesion(vector<Boid> &boids);
	
	ofVec2f loc,vel,acc;
    
	float r;
	float maxforce;
	float maxspeed;
};

#endif