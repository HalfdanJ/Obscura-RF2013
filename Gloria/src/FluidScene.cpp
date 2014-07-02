//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "FluidScene.h"

void FluidScene::setup(){
    
    name = "FluidScene";
    oscAddress = "/fluid";
    
    float scaleFactor = 1.0;
    
    drawWidth = OUTWIDTH/scaleFactor;
	drawHeight = OUTHEIGHT/scaleFactor;
	
    fluid.allocate(drawWidth, drawHeight, 0.2);
    
    // Seting the gravity set up & injecting the background image
    //
    fluid.dissipation = 0.998;
    fluid.velocityDissipation = 0.97;
    
	//fluid.setGravity(ofVec2f(0.0,0.0));
    
    //  Set obstacle
    //
    fluid.begin();
    
    ofSetColor(0,0);
    ofSetColor(255);
    //ofCircle(drawWidth*0.5, drawHeight*0.35, 200);
    fluid.end();
    
    obstacles.allocate(drawWidth, drawHeight);
    
    obstacles.begin();
    
    ofScale(1.0/scaleFactor, 1.0/scaleFactor);
    ofBackground(0, 0, 0, 0);
    
    for(int i =0; i<mapping->triangles.size();i++) {
        
        ofSetColor(255);
        //Corner * c1;
        //c1 = mapping->triangles[i]->corners[(int)ofRandom(2)];
        
        ofSetLineWidth(10);
        
        for(int c=0; c<3; c++) {
            
            
            ofVec3f pos = mapping->triangles[i]->corners[c]->pos;
            ofVec3f nPos;
            
            if(c==2) {
                nPos = mapping->triangles[i]->corners[0]->pos;
            } else {
                nPos = mapping->triangles[i]->corners[c+1]->pos;
            }
            
            ofLine(pos.getInterpolated(nPos, 0.1), nPos.getInterpolated(pos, 0.1));
        }
        
        //ofFill();
        //mapping->triangles[i]->mesh.drawFaces();
        
    }
    
    obstacles.end();
    
    fluid.setObstacles(obstacles);
    fluid.setUseObstacles(true);
    
    //fluid.setPasses(8);

    // Adding constant forces
    //
    //fluid.addConstantForce(ofPoint(drawWidth*0.4,drawHeight*0.95), ofPoint(0,-2), ofFloatColor(1.0,0.1,0.0), 15.5f);
}

void FluidScene::update(){
    // Adding temporal Force
    //
    
    fluid.setGravity(gravity);

    ofPoint m = emitPos;
    ofPoint d = (m - oldM)*10.0;
    oldM = m;
    
    fluid.addTemporalForce(m, d, ofFloatColor(1, 1, 1),16.0f);
    
    //fluid.addTemporalForce(m, d, ofFloatColor(1, 1, 1),16.0f);
    
    //fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),3.0f);
    //  Update
    //
    
    
    
    if(clear) {
        fluid.clear();
        fluid.setObstacles(obstacles);
    } clear = false;
    
    
    //if(ofGetFrameNum() % 2 == 1) {
        fluid.update();
    //}
    
    
    
}

void FluidScene::draw(){;
    
    ofClear(0,0,0);
    
    ofSetColor(255,255,255,255);
    fluid.draw(0,0,OUTWIDTH,OUTHEIGHT);
    //syphonIn->getTexture().draw(0,0,OUTWIDTH,OUTHEIGHT);
    
    if(drawObstacles) {
        ofSetColor(255,255,255,255);
        obstacles.draw(0,0,OUTWIDTH, OUTHEIGHT);
        //fluid.drawVelocity();
    }
}

void FluidScene::parseOscMessage(ofxOscMessage *m){
}

void FluidScene::setGui(){
    
    gui->addSlider("/gravity/x", -1, 1, &gravity.x);
    gui->addSlider("/gravity/y", -1, 1, &gravity.y);
    
    gui->addSlider("/emit/x", 10, drawWidth-10, &emitPos.x);
    gui->addSlider("/emit/y", 10, drawHeight-10, &emitPos.y);
    
    gui->addButton("/clear/x", &clear);
    
    gui->addToggle("/drawobstacles/x", &drawObstacles);
    drawObstacles = false;
    
    
}
