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
    
    xTime = 0;
    yTime = 0;
}

void PerlinWaves::update(){
    xTime += xSpeed;
    yTime += ySpeed;
}

void PerlinWaves::draw(){;
    
     // waves going across red
    
    
    //y[1] = 60*3 +ofNoise(t*freq.getValue())*yFactor.getValue();
    //y[2] = 60*4 +ofSignedNoise(t*freq.getValue())*yFactor.getValue();
    
    float t = ofGetElapsedTimef();
    //float val = ofNoise((mapping->triangles[i]->centroid.x/fbo.getWidth() - t)*speed*100)*255; // 255 is amplitude
    

    
    //float x = abs(ofNoise(t*frequency)*fbo.getWidth());
    //float y = ofNoise(t*frequency)*fbo.getHeight();
    
     for(int i =0; i<mapping->triangles.size();i++) {
         
         ofSetColor( 255, 255, 255,
                    ofNoise( (yTime/10) - mapping->triangles[i]->centroid.y
                            / ( yScatter*OUTHEIGHT) ) *255 );
         ofSetLineWidth(2);
         glEnable(GL_LINES);
         mapping->triangles[i]->mesh.drawWireframe();
         
         
         ofSetColor( 255, 255, 255,
                    ofNoise( (xTime/10) - mapping->triangles[i]->centroid.x
                            / ( xScatter*OUTWIDTH) ) *255 );
         
         mapping->triangles[i]->mesh.draw();
         
     }
}

void PerlinWaves::parseOscMessage(ofxOscMessage *m){
    
 	/*vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
	string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
    
	if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
        
	    if( rest == "/crazyspeed/x" ) {
            crazySpeed = m->getArgAsInt32(0);
            
	    } else if( rest == "/reverse/x" ) {
            reverse = m->getArgAsInt32(0);
            
	    } else if( rest == "/scatter/x" ) {
            xScatter = m->getArgAsFloat(0);
            
	    } else if( rest == "/scatter/y" ) {
            yScatter = m->getArgAsFloat(0);
	    } else if( rest == "/speed/x" ) {
            xSpeed = m->getArgAsFloat(0);
            
	    } else if( rest == "/speed/y" ) {
            ySpeed = m->getArgAsFloat(0);
	    }
    }*/
}

void PerlinWaves::setGui(){
    
    gui->addSlider("speed/x", -1, 1, &xSpeed);
    gui->addSlider("speed/y", -1, 1, &ySpeed);
    
    gui->addSlider("scatter/x", 0, 1, &xScatter);
    gui->addSlider("scatter/y", 0, 1, &yScatter);
    
    
    
    
}
