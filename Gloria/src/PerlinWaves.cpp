//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "PerlinWaves.h"

void PerlinWaves::setup(){

    name = "PerlinWaves";
    oscAddress = "/plw";

}

void PerlinWaves::update(){

}

void PerlinWaves::draw(){;

    float speedV = ofMap(speed, 0, 1, -2, 40);
    
     ofSetLineWidth(4);
     // waves going across red
     for(int i =0; i<mapping->triangles.size();i++) {
        
         
         //y[1] = 60*3+ofNoise(t*freq.getValue())*yFactor.getValue();
         //y[2] = 60*4+ofSignedNoise(t*freq.getValue())*yFactor.getValue();
         
         
         
     ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.y/600 - ofGetElapsedTimef()/2)*255);
     
         
         mapping->triangles[i]->mesh.draw();
         
         

     ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.x/600 - ofGetElapsedTimef()/speed) *255 );
     mapping->triangles[i]->mesh.drawWireframe();
     }

}

void PerlinWaves::parseOscMessage(ofxOscMessage *m){
    ContentScene::parseOscMessage(m);
    
 	vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
	string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
    
	if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
        
	    if( rest == "/frequency/x" ) {
            frequency = m->getArgAsFloat(0);
	    }/* else if( rest == "/frequency/x" ) {
            frequency = m->getArgAsFloat(0);
	    }*/

        
        
    }
    
}
