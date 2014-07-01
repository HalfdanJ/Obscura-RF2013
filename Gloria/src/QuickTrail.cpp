//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "QuickTrail.h"

void QuickTrail::setup(){
    
    name = "QuickTrail";
    oscAddress = "/quicktrail";
    
    time = 0;

    walkers.resize(8);
    lastadded = ofGetElapsedTimeMillis();
}

void QuickTrail::update(){
    
    time += speed;
    
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

void QuickTrail::draw(){;
    
    
    
    ofClear(0,0);
    
    ofSetLineWidth(10);
    ofNoFill();

    float pct = ofClamp((time - lastadded) / (interval * 1.), 0,1);

        for(int i=0;i<walkers.size() && i <numtrails;i++) {

            for(int p=1; p<walkers[i].corners.size() && p<length; p++) {
                
                ofSetColor(255,255,255,(p/length*1.0*255));

                if(p > length-1) {


                    ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                    ofVec2f dstPt =walkers[i].corners[p]->pos;

                    ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;

                    ofLine(interpolate, dstPt);
                    ofCircle(interpolate.x, interpolate.y, circleRadius);
                    //ofVertex(walkers[i].points[p].x,w alkers[i].points[p].y);

                } else {

                    ofCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, (p/length*1.0*circleRadius));

                    ofLine(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, walkers[i].corners[p-1]->pos.x, walkers[i].corners[p-1]->pos.y);

            }
        }
    }
}


void QuickTrail::parseOscMessage(ofxOscMessage *m){

}

void QuickTrail::setGui(){
    
    
    gui->addSlider("/speed/x", 0, 1, &speed);
    
    gui->addSlider("/length/x", 0, 20, &length);
    gui->addIntSlider("/trails/x", 0, 8, &numtrails);
    
    gui->addToggle("/random/x", &random);
    
    gui->addButton("/clear/x", &clear);
    
    gui->addSlider("/circleradius/x", 0, 200, &circleRadius);
    
}
