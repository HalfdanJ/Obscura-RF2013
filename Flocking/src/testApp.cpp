#include "testApp.h"

//--------------------------------------------------------------
bool isMouseMoving() {
    static ofPoint pmouse;
    ofPoint mouse(ofGetMouseX(),ofGetMouseY());
    bool mouseIsMoving = (mouse!=pmouse);
    pmouse = mouse;
    return mouseIsMoving;
}

//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(0,0,0);
	ofSetFrameRate(60);
    
    for(int i=0;i<13;i++){
        flock.addBoid(ofRandom(200),ofRandom(200));
    }
    
    osc.setup("halfdanjOld.local", 6000);
}

//--------------------------------------------------------------
void testApp::update() {
    
    if (isMouseMoving()) {
        for(int i=0; i<flock.boids.size(); i++) {            
            flock.boids[i].avoid(ofPoint(mouseX,mouseY));
        }
    }
    
	flock.update();
    
    for(int i=0;i<flock.boids.size();i++){
        
        float x = 15 * (flock.boids[i].loc.x / ofGetWidth() - 0.5);
        float y = 30 * (flock.boids[i].loc.y / ofGetHeight() - 0.5);
        
        ofxOscMessage m;
        
        m.setAddress("/sharpy");
        m.addIntArg(i+1); // device number
        m.addFloatArg(x); // x
        m.addFloatArg(0); // y
        m.addFloatArg(y); // x
        osc.sendMessage(m);


        
    }
    
    
  }

//--------------------------------------------------------------
void testApp::draw() {
//    glScaled(ofGetWidth(), ofGetHeight(), 1);
	flock.draw();
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
//	flock.addBoid(x,y);
}






