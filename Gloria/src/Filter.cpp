//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "Filter.h"

void Filter::setup(){
    
    name = "Filter";
    oscAddress = "/plw";
    
    drawWidth = OUTWIDTH/8;
	drawHeight = OUTHEIGHT/8;
	
    fluid.allocate(drawWidth, drawHeight, 0.5);

    
    // Seting the gravity set up & injecting the background image
    //
    fluid.dissipation = 0.99;
    fluid.velocityDissipation = 0.99;
    
	fluid.setGravity(ofVec2f(0.0,0.0));
    //    fluid.setGravity(ofVec2f(0.0,0.0098));
    
    
    //  Set obstacle
    //
    fluid.begin();
    
    
    
    ofSetColor(0,0);
    ofSetColor(255);
    //ofCircle(drawWidth*0.5, drawHeight*0.35, 200);
    fluid.end();
    
    obstacles.allocate(drawWidth, drawHeight);
    
    obstacles.begin();
    ofBackground(0, 0, 0, 0);
    
    for(int i =0; i<mapping->triangles.size();i+=2) {
        
        ofSetColor(255);
        Corner * c1;
        c1 = mapping->triangles[i]->corners[(int)ofRandom(2)];
        
        ofLine(c1->pos/8, mapping->triangles[i]->corners[2]->pos/8);
        
    }
    
    obstacles.end();
    
    
    fluid.setObstacles(obstacles);
    
    fluid.setUseObstacles(true);
    
    // Adding constant forces
    //
    //fluid.addConstantForce(ofPoint(drawWidth*0.5,drawHeight*0.85), ofPoint(0,-2), ofFloatColor(1,0.0,0.0), 5.5f);

}

void Filter::update(){
    // Adding temporal Force
    //
    
    ofPoint m = ofPoint(ofGetMouseX(),ofGetMouseY());
    ofPoint d = (m - oldM)*10.0;
    oldM = m;

    fluid.addTemporalForce(m, d, ofFloatColor(1, 1, 1),3.0f);
    
    //fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),3.0f);
    //  Update
    //
    fluid.update();

    
}

void Filter::draw(){;
    ofBackground(0,0,100);
    ofSetColor(255,255,255,255);
    fluid.draw(0,0,OUTWIDTH,OUTHEIGHT);
    //syphonIn->getTexture().draw(0,0,OUTWIDTH,OUTHEIGHT);
    ofSetColor(255,255,255,100);
    obstacles.draw(0,0,OUTWIDTH, OUTHEIGHT);
}

void Filter::parseOscMessage(ofxOscMessage *m){
    
}

void Filter::setGui(){
    
    
}
