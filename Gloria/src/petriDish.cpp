
#include "PetriDish.h"

void Qt::qtsetup(){
    
    time = 0;
    
    walkers.resize(8);
    lastadded = ofGetElapsedTimeMillis();
}

void Qt::qtupdate(float speed, Mapping * mapping){
    
    time += speed;
    animSpeed = speed;
    
    for(int i=0;i<walkers.size();i++) {
        
        if(clear) {
            walkers[i].corners.clear();
            clear = false;
        }
        
        if(walkers[i].corners.empty()) {
            walkers[i].dst = mapping->corners[ofRandom(mapping->corners.size())];
            walkers[i].corners.push_back(mapping->corners[ofRandom(mapping->corners.size())]);
        }
    }
    
    for(int i=0;i<walkers.size() && i<numtrails; i++) {
        if(i<numtrails) {
            
            if(walkers[i].corners.size() < length) {
                if(random) {
                    walkers[i].dst = mapping->corners[ofRandom(mapping->corners.size())];
                } else {
                    walkers[i].dst = walkers[i].dst->joinedCorners[ofRandom(walkers[i].dst->joinedCorners.size())];
                }
                
                walkers[i].corners.push_back(walkers[i].dst);
                
                lastadded = time;
            } else {
                
                if(time - lastadded > interval ) {
                    
                    walkers[i].corners.erase(walkers[i].corners.begin());
                    
                }
            }
        }
    }
}

void Qt::qtdraw(){;
    ofClear(0,0);
    
    //ofSetLineWidth(10);
    ofSetLineWidth(10);
    ofNoFill();
    
    float pct = ofClamp((time - lastadded) / (interval * 1.), 0,1);
    
    for(int i=0;i<walkers.size() && i <numtrails;i++) {
        
        for(int p=1; p<walkers[i].corners.size() && p<length; p++) {
            
            //ofSetColor(255,255,255,(p/length*1.0*255));
            ofSetColor(255,255,255,255);
            
            if(p > length-1) {
                
                
                ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                ofVec2f dstPt =walkers[i].corners[p]->pos;
                
                ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                
                ofLine(interpolate, dstPt);
                ofCircle(interpolate.x, interpolate.y, circleRadius);
                //ofVertex(walkers[i].points[p].x,w alkers[i].points[p].y);
                
            } else {
        
                ofCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, (p/length*1.0*circleRadius));
                float randColor = ofRandom(255);
                ofSetColor(randColor,randColor,randColor,255);

                
                ofLine(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, walkers[i].corners[p-1]->pos.x, walkers[i].corners[p-1]->pos.y);
            }
        }
        
        if (length == 0) {
            if (fmod(ofGetElapsedTimef(),animSpeed) < 0.01) {
                ofCircle(walkers[i].corners[ofRandom(walkers[i].corners.size())]->pos.x, walkers[i].corners[ofRandom(walkers[i].corners.size())]->pos.y, circleRadius);
            }
        }
    }
}



//
//  Gloria
//
//  Created by vibber on 02/07/14.
//
//


void PetriDish::setup(){
    name = "PetriDish";
    oscAddress = "/petridish";

    outWidth 		= OUTWIDTH;
	outHeight 		= OUTHEIGHT;

    gray.allocate(outWidth, outHeight);
    
    initOutputPlane(gray.getTextureReference());

    qtScene.qtsetup();
    trailLength = 0;
}

void PetriDish::initOutputPlane(ofTexture tex) {
    
    //Gray-Scott
    //Output plane
    plane.set(outWidth, outHeight, 10, 10, OF_PRIMITIVE_TRIANGLES);
    plane.mapTexCoordsFromTexture(tex);
    
    shader.load("shaders/petriDishPostFX");

    //Quicktrails
    qtScene = *new Qt();
    qtScene.qtsetup();
}

void PetriDish::update(){
    
    //Gray-Scott
    gray.update();
    if (preset1) {
        eatingCells = 0.064;
        growingCells = 0.035;
    }
    if (preset2) {
        eatingCells = 0.06;
        growingCells = 0.035;
    }
    if (preset3) {
        eatingCells = 0.06304;
        growingCells = 0.04604;
    }
    if (eraseAll) {
        growingCells = 0.0;
    }
    gray.setK(eatingCells);
    gray.setF(growingCells);
    
    //Quicktrails
    qtScene.qtupdate(speed, mapping);
    qtScene.length = trailLength;
}

void PetriDish::draw(){
    
    //Gray-Scott
    float x = ofRandom(outWidth);
    float y = ofRandom(outHeight);
    gray.begin();
    //ofSetColor(ofNoise( ofGetElapsedTimef() )*255);
    //ofCircle(x, y, 3);
    qtScene.qtdraw();
    gray.end();
    
    drawOutput(gray.getTextureReference());
}

void PetriDish::setGui(){

    gui->addSlider("/growingCells/x", 0.005,0.095, &growingCells);
    gui->addSlider("/eatingCells/x", 0.01, 0.035, &eatingCells);
    gui->addSlider("/birthSpeed/x", 0.0001, 0.4, &speed);
    gui->addSlider("/trailLength/x", 0, 10, &trailLength);
    gui->addButton("/preset1/x", &preset1);
    gui->addButton("/preset1/x", &preset2);
    gui->addButton("/preset1/x", &preset3);
    gui->addButton("/eraseAll/x", &eraseAll);
}

void PetriDish::drawOutput(ofTexture inputTex) {
    ofPushMatrix();
    // translate plane into center screen.
    float tx = outWidth / 2;
    float ty = outHeight / 2;
    ofTranslate(tx, ty);
    
    
    ofSetColor(255);
    //Bind the texture to tex0
    inputTex.bind();
    
    shader.begin();
    shader.setUniform1f("mouseX", 1);
    plane.draw();
    shader.end();
    
    inputTex.unbind();
    ofPopMatrix();
}