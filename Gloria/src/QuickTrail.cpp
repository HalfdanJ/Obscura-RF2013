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
    
    walkers.resize(1);
    for(int i=0;i<walkers.size();i++) {
        walkers[i].corner = mapping->corners[0];
    }
}

void QuickTrail::update(){
}

void QuickTrail::draw(){;
    
	for(int i=0;i<walkers.size();i++) {
        
        walkers[i].points.push_back(walkers[i].corner->pos);
        
        if(walkers[i].points.size() > 40) {
            walkers[i].points.erase(walkers[i].points.begin());
        }
        
        ofBeginShape();
        ofNoFill();
        ofSetLineWidth(20);
        
        for(int p=0; p<walkers[i].points.size(); p++) {
            ofSetColor(100,100,255,200);
            ofCircle(walkers[i].points[p].x, walkers[i].points[p].y, 4);
            ofVertex(walkers[i].points[p].x,walkers[i].points[p].y);
        }
        ofEndShape();
        
        InputTriangle * nt;
        nt = walkers[i].corner->triangles[ofRandom(walkers[i].corner->triangles.size())];
        
        walkers[i].corner = nt->corners[(int)ofRandom(0,3)];
        
    }
}