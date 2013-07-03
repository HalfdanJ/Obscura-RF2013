//
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "GTess.h"

void GTess::setup(){

    name = "Tesselator";
    oscAddress = "/tlat";

}

void GTess::update(){

}

void GTess::draw(){;

    float speedV = ofMap(speed, 0, 1, -2, 2);
    
    ofSetLineWidth(4);
    // waves going across red
    for(int i =0; i<mapping->triangles.size();i++) {
        ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.y/600 - ofGetElapsedTimef()/speed)*255);
        
        //mapping->triangles[i]->
        
        ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.x/600 - ofGetElapsedTimef()/speed) *255 );
        mapping->triangles[i]->mesh.drawWireframe();
    }
    
}