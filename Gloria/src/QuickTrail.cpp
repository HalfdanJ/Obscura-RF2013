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
    
    walkers.resize(8);
    for(int i=0;i<walkers.size();i++) {
        walkers[i].corner = mapping->corners[0];
    }
}

void QuickTrail::update(){
}

void QuickTrail::draw(){;
    
	for(int i=0;i<walkers.size();i++) {
        
        if(i<numtrails) {
            
            walkers[i].points.push_back(walkers[i].corner->pos);
        
        if(walkers[i].points.size() > length) {
            walkers[i].points.erase(walkers[i].points.begin());
        }
        
        ofBeginShape();
        ofNoFill();
        ofSetLineWidth(4);
        
        for(int p=0; p<walkers[i].points.size(); p++) {
            ofSetColor(255,100,100,250);
            ofCircle(walkers[i].points[p].x, walkers[i].points[p].y, 4);
            ofVertex(walkers[i].points[p].x,walkers[i].points[p].y);
        }
        ofEndShape();
        
        InputTriangle * nt;
        nt = walkers[i].corner->triangles[ofRandom(walkers[i].corner->triangles.size())];
        
        walkers[i].corner = nt->corners[(int)ofRandom(0,3)];
     
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
	    }
        
    }
}

void QuickTrail::setGui(ofxUICanvas *gui, float width){
    
    ContentScene::setGui(gui, width);
    
    string i = "["+ ofToString(index) + "] ";
     
    //gui->addSlider(i+"length", 0, 8, &length);
    //gui->addSlider(i+"length", 0, 8, &num);
    
    /*gui->addSlider(i+"Y Speed", minSpeed, maxSpeed, &ySpeed);
     
     
     gui->addSlider(i+"X Scatter", 0, 6000, &xScatter);
     gui->addSlider(i+"Y Scatter", 0, 6000, &yScatter);
     
     gui->addToggle(i+"Reverse", &reverse);
     gui->addToggle(i+"Crazy speed", &crazySpeed);
     */
    
    
    
}
