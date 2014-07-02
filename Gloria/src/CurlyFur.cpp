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

    
    initCam();

    /*
    ofFbo::Settings fboSettings;
    fboSettings.height = OUTHEIGHT/10.0;
    fboSettings.width = OUTWIDTH/10.0;
    fboSettings.numSamples = 4;
    fboSettings.useDepth = false;
    
    myfbo.allocate(fboSettings);
    */
    
    //    cout << "width: " << OUTWIDTH << endl;
    
    myfbo.allocate(OUTWIDTH/10, OUTHEIGHT/10, GL_RGB);
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
    ofEnableAlphaBlending();
    
    //Draw particles
    for (int i=0; i<particles.size(); i++) {
        Particle &p = particles[i];
        //p.color = img.getColor(p.pos.x,p.pos.y);
        p.draw();
    }
    
    for (int i=0; i<50; i++) {
        createParticle();
    }
    
    while (particles.size()>1000) {
        particles.erase(particles.begin());
    }
    //}
    
    //Fade out the old stuff
    //ofSetColor(0,0,0,5);
    //ofFill();
    //ofRect(0,0,OUTWIDTH,OUTHEIGHT);
    
    //Cam debug view
    ofSetHexColor(0xffffff);
    vidGrabber.draw(20,20);
    
    ofSetColor(255,255,255,0);
    
    ofDisableAlphaBlending();
    
    //http://www.openframeworks.cc/documentation/gl/ofFbo.html
    myfbo.begin();
    ofSetColor(255,255,255);
    ofFill();
    ofRect(0,0,OUTWIDTH/10,OUTHEIGHT/10);
    syphonIn->draw(0, 0, OUTWIDTH/10,OUTHEIGHT/10);
    myfbo.end();
    
    myfbo.draw(0,0);
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
    ofPixels pixels;
    Particle p;
    p.setup();
    //p.pos.set(ofRandomWidth(), ofRandomHeight());
    p.pos.set(OUTWIDTH * randomX, OUTHEIGHT * randomY);
    camPosX = camWidth * randomX;
    camPosY = camHeight * randomY;
    //Not working
    //syphonIn->getTexture().readToPixels(pixels);
    //p.color = pixels.getColor(p.pos.x,p.pos.y);
    //cout << "width: " << pixels.getColor(p.pos.x,p.pos.y) << endl;
    
    //http://forum.openframeworks.cc/t/ofxsyphon-to-image/7791/3
/*    myfbo.begin();
    syphonIn->draw(0, 0, OUTWIDTH/10.0, OUTHEIGHT/10.0);
    myfbo.end();
    
    myfbo.readToPixels(pixels);
    p.color = pixels.getColor(p.pos.x/10,p.pos.y/10);
    cout << "width: " << pixels.getColor(p.pos.x/10,p.pos.y/10) << endl;
*/
    
    myfbo.readToPixels(pixels);
    p.color = pixels.getColor(p.pos.x/10,p.pos.y/10);
    
    //p.color = img.getColor(p.pos.x,p.pos.y);
    //p.color = vidGrabber.getPixelsRef().getColor(camPosX,camPosY);
    particles.push_back(p);
}
