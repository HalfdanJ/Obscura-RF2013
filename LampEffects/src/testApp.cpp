#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(0,0,0);
	ofSetFrameRate(30);
    
    osc.setup("127.0.0.1", 6000);
    oscReceiver.setup(9001);
    circleRadius = 0;
    circleSpeed = 0;
    
    lampPositions.assign(numLamps, ofVec3f(0,0,0));
    
    time = 0;
    
    for(int i=0;i<13;i++){
        flock.addBoid(ofRandom(200),ofRandom(200));
    }
    
    clientOne = new ofxOscSender();
    clientTwo = new ofxOscSender();
    
    clientOne->setup(OSCCLIENTONE, OSCSENDPORT);
    clientTwo->setup(OSCCLIENTTWO, OSCSENDPORT);
    
    clients.push_back(clientOne);
    clients.push_back(clientTwo);
    
}

bool isMouseMoving() {
    static ofPoint pmouse;
    ofPoint mouse(ofGetMouseX(),ofGetMouseY());
    bool mouseIsMoving = (mouse!=pmouse);
    pmouse = mouse;
    return mouseIsMoving;
}

//--------------------------------------------------------------
void testApp::update() {
    
    
    if (isMouseMoving()) {
        for(int i=0; i<flock.boids.size(); i++) {
            flock.boids[i].avoid(ofPoint(mouseX,mouseY));
        }
    }
    
    
    for(int i=0; i<flock.boids.size(); i++) {
        //flock.boids[i].maxspeed = circleSpeed;
        
        flock.boids[i].maxspeed = circleSpeed*10;
        flock.boids[i].maxforce = circleSpeed/4;
    }
	flock.update();
    
    
    while(oscReceiver.hasWaitingMessages() > 0){
        ofxOscMessage  m;
        oscReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/Center/x"){
            center.x = (1-m.getArgAsFloat(0)-0.5)*2;
            
        }
        if(m.getAddress() == "/Center/y"){
            center.y = (1-m.getArgAsFloat(0)-0.5)*2;
            
        }
        if(m.getAddress() == "/Height/x"){
            center.z = ((m.getArgAsFloat(0))-0.5)*2;
            //center.z = ((1-m.getArgAsFloat(0)));//*2;
            
        }
        if(m.getAddress() == "/Radius/x"){
            circleRadius = m.getArgAsFloat(0)*1.2;
        }
        if(m.getAddress() == "/Speed/x"){
            circleSpeed = m.getArgAsFloat(0);
        }
        
        if(m.getAddress() == "/Type/x"){
            type = m.getArgAsInt32(0);
        }
        
        /*if(m.getAddress() == "/Dim/x"){
         dim = m.getArgAsInt32(0);
         }*/
        
        if(m.getAddress() == "/Flocking/x"){
            flockEnable = (m.getArgAsInt32(0) == 1);
        }
        
        if(m.getAddress() == "/enable/x"){
            masterEnable = (m.getArgAsInt32(0) == 1);
        }
        
        for(int o=0; o<clients.size(); o++) {
            clients[o]->sendMessage(m);
        }
        
        
    }
    
    time += circleSpeed;
    
    //Circle 1
    
    if(type==2) {
        for(int i=0;i<lampPositions.size();i++){
            float t = 5*time/100.0;//sin(ofGetElapsedTimeMillis()/2000.0)*3;
            float x = center.x + sin(i*TWO_PI/12+t)* (circleRadius);
            float y = center.y + cos(i*TWO_PI/12+t)* (circleRadius);
            
            lampPositions[i].x = x;
            lampPositions[i].y = y;
            lampPositions[i].z = center.z;
            
        }
        
        //Circle 2
    } else if(type==1) {
        for(int i=0;i<lampPositions.size();i++){
            
            float t = floor(5*time/100.0);
            
            float x = center.x + sin(i*TWO_PI/8+t)*circleRadius;
            float y = center.z + cos(i*TWO_PI/8+t)*circleRadius;
            
            lampPositions[i].x = x;
            lampPositions[i].y = y;
            lampPositions[i].z = center.z;
        }
        
    } else if(type == 0) {
        
        //Square
        for(int i=0;i<lampPositions.size();i++){
            
            float t = 5* time/100.0;//sin(ofGetElapsedTimeMillis()/2000.0)*3;
            
            float x, y;
            float phase = fmod(t+4*i/9.0 , 4);
            //cout<<phase<<endl;
            
            
            if(phase < 1){
                x = (phase ) * circleRadius - circleRadius/2;
                y = -circleRadius/2;
                
            }
            else if(phase < 2){
                y = ( (phase-1)) * circleRadius - circleRadius/2;
                x = circleRadius/2;
                
            }
            else if(phase < 3){
                x = (1-(phase -2)) * circleRadius - circleRadius/2;
                y = circleRadius/2;
            }
            else if(phase < 4){
                y = (1-(phase -3)) * circleRadius - circleRadius/2;
                x = -circleRadius/2;
            }
        }
    }
    
    if(flockEnable) {
        
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
    } else {
        
        for(int i=0;i<lampPositions.size();i++){
            ofxOscMessage m;
            m.setAddress("/sharpy");
            m.addIntArg(i+1); // device number
            m.addFloatArg(lampPositions[i].y*14);
            m.addFloatArg(lampPositions[i].z*5);
            m.addFloatArg(lampPositions[i].x*14);
            osc.sendMessage(m);
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw() {
    //    glScaled(ofGetWidth(), ofGetHeight(), 1);
    
    ofPushMatrix();
    
    ofNoFill();
    ofTranslate(0.5*ofGetWidth(), 0.5*ofGetHeight());
    
    ofScale(ofGetWidth()/2, ofGetHeight()/2);
    ofRect(-0.99, -0.99, 1.98, 1.98);
    
    ofCircle(center.x, center.y, 0.04);
    
    //  cout<<center<<endl;
    
    ofNoFill();
    
    ofPushMatrix();
    
    ofFill();
    for(int i=0;i<lampPositions.size();i++){
        ofCircle(lampPositions[i].x, lampPositions[i].y, 0.02);
    }
    ofPopMatrix();
    
    ofPopMatrix();
    
    flock.draw();
    
    ofDrawBitmapString("Flocking: " + ofToString(flockEnable), 40,40);
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
    //	flock.addBoid(x,y);
}




