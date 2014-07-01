//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "CurlyFur.h"

void CurlyFur::setup(){
    name = "CurlyFur";
    oscAddress = "/curly";
    
    // vib
    ofClearAlpha();
    ofClear(0, 0, 0);
//    ofSetBackgroundAuto(false);
//    ofEnableAlphaBlending();
//    ofBackground(0);
    initCam();

    ofSetColor(0,0,0);
    ofRect(0,0,OUTWIDTH,OUTHEIGHT);
    //    cout << "width: " << OUTWIDTH << endl;
}

void CurlyFur::update(){
    
    for (int i=0; i<particles.size(); i++) {
        particles[i].update();
    }

  	vidGrabber.update();
}


void CurlyFur::initCam(){
	
	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;
	
    //we can now get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}
    
    if (devices.size() > 1) {
        vidGrabber.setDeviceID(1);
    } else {
        vidGrabber.setDeviceID(0);
    }
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);
	
	videoTexture.allocate(camWidth,camHeight, GL_RGB);
	ofSetVerticalSync(true);
}

void CurlyFur::draw(){;
/*
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
 */
    
    //Draw particles
    for (int i=0; i<particles.size(); i++) {
        Particle &p = particles[i];
        //p.color = img.getColor(p.pos.x,p.pos.y);
        p.draw();
    }
    
    for (int i=0; i<500; i++) {
        createParticle();
    }
    
    while (particles.size()>2000) {
        particles.erase(particles.begin());
    }
    //}
    
    //Fade out the old stuff
    ofSetColor(0,0,0,5);
    ofRect(0,0,OUTWIDTH,OUTHEIGHT);
    
    //Cam debug view
    ofSetHexColor(0xffffff);
    vidGrabber.draw(20,20);
    
    ofSetColor(255,255,255,0);
}

void CurlyFur::parseOscMessage(ofxOscMessage *m){

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

void CurlyFur::setGui(){
/*
    gui->addSlider("speed/x", -1, 1, &xSpeed);
    gui->addSlider("speed/y", -1, 1, &ySpeed);
    
    gui->addSlider("scatter/x", 0, 1, &xScatter);
    gui->addSlider("scatter/y", 0, 1, &yScatter);
*/
}

void CurlyFur::createParticle() {
    int camPosX, camPosY;
    float randomX = ofRandom(1.0);
    float randomY = ofRandom(1.0);
    Particle p;
    p.setup();
    //p.pos.set(ofRandomWidth(), ofRandomHeight());
    p.pos.set(ofGetWidth() * randomX, ofGetHeight() * randomY);
    camPosX = camWidth * randomX;
    camPosY = camHeight * randomY;
    //p.color = img.getColor(p.pos.x,p.pos.y);
    p.color = vidGrabber.getPixelsRef().getColor(camPosX,camPosY);
    particles.push_back(p);
}
