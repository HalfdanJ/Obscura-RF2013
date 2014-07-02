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
    
    time.x = 0;
    time.y = 0;
}

void PerlinWaves::update(){
    time.x += speed.x;
    time.y += speed.y;
}

void PerlinWaves::draw(){;
    
     for(int i =0; i<mapping->triangles.size();i++) {
         
         ofSetColor( 255, 255, 255,
                    ofNoise( (time.y/10) - mapping->triangles[i]->centroid.y
                            / ( scatter.y*OUTHEIGHT) ) *255 );
         ofSetLineWidth(2);
         glEnable(GL_LINES);
         mapping->triangles[i]->mesh.drawWireframe();
         
         
         ofSetColor( 255, 255, 255,
                    ofNoise( (time.x/10) - mapping->triangles[i]->centroid.x
                            / ( scatter.x*OUTWIDTH) ) *255 );
         
         mapping->triangles[i]->mesh.draw();
         
     }
}

void PerlinWaves::parseOscMessage(ofxOscMessage *m){
    
        
}

void PerlinWaves::setGui(){
    
    gui->addSlider("speed/x", -1, 1, &speed.x);
    gui->addSlider("speed/y", -1, 1, &speed.y);
    
    gui->addSlider("scatter/x", 0, 1, &scatter.x);
    gui->addSlider("scatter/y", 0, 1, &scatter.y);
    
    
    
    
}
