//
//  HardNoise.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 04/07/2013.
//
//

#include "HardNoise.h"

void HardNoise::setup(){
    
    name = "Hard Noise";
    oscAddress = "/hnoise";
    
}

void HardNoise::update(){
    
}

void HardNoise::draw(){;
    
    float speedV = ofMap(speed, 0, 1, -2, 2);
    
    ofSetLineWidth(20);
    // waves going across red
    for(int i =0; i<mapping->triangles.size();i++) {
        ofSetColor( 255, 255, 255, 255);
        mapping->triangles[i]->polyline.getResampledBySpacing(ofRandom(40)).getSmoothed(ofRandom(mapping->triangles[i]->centroid.y/600 - ofGetElapsedTimef()/speed)*6.0).draw();
        
        // Random lines across 
        
        //ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.x/600 - ofGetElapsedTimef()/speed) *255 );
        //mapping->triangles[i]->polyline.draw();
    }
    
}