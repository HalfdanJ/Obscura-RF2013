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
    
    minSpeed = 0;
    maxSpeed = 300;

    walkers.resize(4);

    lastadded = ofGetElapsedTimeMillis();
}

void QuickTrail::update(){

    float interval = speed;
    
    

    
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

                lastadded = ofGetElapsedTimeMillis();
            } else {

                if(ofGetElapsedTimeMillis() - lastadded > interval ) {

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
    float interval = maxSpeed - speed;

    float pct = (ofGetElapsedTimeMillis() - lastadded) / (interval * 1.);

	for(int i=0;i<walkers.size() && i <numtrails;i++) {

        
	for(int p=1; p<walkers[i].corners.size() && p<length; p++) {
        
	    ofSetColor(255,255,255,(p/length*255));

	    if(p > length-1) {


		ofVec2f srcPt =walkers[i].corners[p-1]->pos;
		ofVec2f dstPt =walkers[i].corners[p]->pos;

		ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;

		ofLine(interpolate, dstPt);
		ofCircle(interpolate.x, interpolate.y, 40);
		//ofVertex(walkers[i].points[p].x,w alkers[i].points[p].y);

	    } else {

		ofCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, 10+(p/length*30));

		ofLine(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, walkers[i].corners[p-1]->pos.x, walkers[i].corners[p-1]->pos.y);

	    }
        }
    }
}


void QuickTrail::parseOscMessage(ofxOscMessage *m){
    ContentScene::parseOscMessage(m);
    
 	vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
	string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
    
	if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
        
        if(rest == "/length/x" ) {
            length = m->getArgAsInt32(0);
	    } else if(rest == "/numtrails/x" ) {
            numtrails = m->getArgAsInt32(0);
	    } else if(rest == "/random/x") {
            random = m->getArgAsInt32(0);
        } else if(rest == "/clear/x") {
            clear = m->getArgAsInt32(0);
        }
        
    }
}

void QuickTrail::setGui(ofxUICanvas *gui, float width){
    
    ContentScene::setGui(gui, width);
    
    string i = "["+ ofToString(index) + "] ";
     
    gui->addSlider(i+"length", 0, 20, &length);
    gui->addSlider(i+"trails", 0, 4, &numtrails);
    
    gui->addToggle(i+"Random", &random);
    
    gui->addButton(i+"Clear", &clear);
    
    /*gui->addSlider(i+"Y Speed", minSpeed, maxSpeed, &ySpeed);
     
     
     gui->addSlider(i+"X Scatter", 0, 6000, &xScatter);
     gui->addSlider(i+"Y Scatter", 0, 6000, &yScatter);
     
     gui->addToggle(i+"Reverse", &reverse);
     gui->addToggle(i+"Crazy speed", &crazySpeed);
     */
    
    
    
}
