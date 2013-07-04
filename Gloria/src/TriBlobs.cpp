//
//  TriMasker.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 03/07/2013.
//
//

#include "TriBlobs.h"

void TriBlobs::setup(){
    
    name = "Triangle Blobs";
    oscAddress = "/triblobs";
    
}

void TriBlobs::update(){
    
}

void TriBlobs::draw(){;
    
    float speedV = ofMap(speed, 0, 1, -2, 2);
    
    ofSetLineWidth(6);
    // waves going across red
    for(int i =0; i<mapping->triangles.size();i++) {
        ofSetColor( 255, 255, 255, 255);
	mapping->triangles[i]->polyline.getResampledBySpacing(ofNoise(mapping->triangles[i]->centroid.y/600 - ofGetElapsedTimef()/speed)*120).getSmoothed(ofNoise(mapping->triangles[i]->centroid.y/600 - ofGetElapsedTimef()/speed)*6.0).draw();
        
        //ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.x/600 - ofGetElapsedTimef()/speed) *255 );
        //mapping->triangles[i]->polyline.draw();
    }
    
}