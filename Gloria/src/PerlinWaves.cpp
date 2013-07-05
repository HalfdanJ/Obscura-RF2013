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
    
    minSpeed = 0;
    maxSpeed = 20;

}

void PerlinWaves::update(){
    

    

    
    if(crazySpeed) {
        _ySpeed = ySpeed*4;
        _xSpeed = xSpeed*4;
    } else {
        _ySpeed = ySpeed;
        _xSpeed = xSpeed;
    }
    
    
    if(reverse) {
        _ySpeed = ySpeed *-1;
        _xSpeed = xSpeed *-1;
    }

}

void PerlinWaves::draw(){;
    
    glEnable(GL_LINES);
     ofSetLineWidth(4);
     // waves going across red
    
    
    //y[1] = 60*3 +ofNoise(t*freq.getValue())*yFactor.getValue();
    //y[2] = 60*4 +ofSignedNoise(t*freq.getValue())*yFactor.getValue();
    
    float t = ofGetElapsedTimef();
    //float val = ofNoise((mapping->triangles[i]->centroid.x/fbo.getWidth() - t)*speed*100)*255; // 255 is amplitude
    

    
    //float x = abs(ofNoise(t*frequency)*fbo.getWidth());
    //float y = ofNoise(t*frequency)*fbo.getHeight();
    
     for(int i =0; i<mapping->triangles.size();i++) {

       /* float delta = x - mapping->triangles[i]->centroid.x;
        //float opacity = ofMap(delta, 0, fbo.getWidth(), 0, 255);
         
         if(delta < fbo.getWidth()/5) {
             ofSetColor( 255, 255, 255, 255);
         } else {
             ofSetColor( 255, 255, 255, delta/5);
         }
        */
         
         ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.y/yScatter - ofGetElapsedTimef() * _xSpeed) *255 );
         mapping->triangles[i]->mesh.drawWireframe();
         
         
         ofSetColor( 255, 255, 255, ofNoise(mapping->triangles[i]->centroid.x/xScatter - ofGetElapsedTimef()*_ySpeed) *255 );
         
         mapping->triangles[i]->mesh.draw();
         
     }
}

void PerlinWaves::parseOscMessage(ofxOscMessage *m){
    ContentScene::parseOscMessage(m);
    
 	vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
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

    }
}

void PerlinWaves::setGui(ofxUICanvas *gui, float width){    
    string i = "["+ ofToString(index) + "] ";
    
    gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
    
    gui->addSpacer(width, 1);
    gui->addToggle(i+"Enabled", &enabled);
    gui->addToggle(i+"Solo", &solo);
    gui->addSlider(i+"opacity", 0., 1., &opacity);
    
    gui->addSpacer(width, 1);
    
    gui->addSlider(i+"X Speed", minSpeed, maxSpeed, &xSpeed);
    gui->addSlider(i+"Y Speed", minSpeed, maxSpeed, &ySpeed);
    
    
    gui->addSlider(i+"X Scatter", 0, 6000, &xScatter);
    gui->addSlider(i+"Y Scatter", 0, 6000, &yScatter);
    
    gui->addToggle(i+"Reverse", &reverse);
    gui->addToggle(i+"Crazy speed", &crazySpeed);
    

    
    
}
