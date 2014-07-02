#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(0,0,0);
	ofSetFrameRate(30);
    
    osc.setup("halfdanjOld.local", 6000);
    oscReceiver.setup(9001);
    circleRadius = 4;
    circleSpeed = 0.1;
    
    lampPositions.assign(numLamps, ofVec3f(0,0,0));
    
    time = 0;
    
}

//--------------------------------------------------------------
void testApp::update() {
    
    
    
    while(oscReceiver.hasWaitingMessages() > 0){
        ofxOscMessage  m;
        oscReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/Center/x"){
            center.x = (m.getArgAsFloat(0)-0.5)*2;
            
        }
        if(m.getAddress() == "/Center/y"){
            center.y = (1-m.getArgAsFloat(0)-0.5)*2;
            
        }
        if(m.getAddress() == "/Height/x"){
            center.z = ((1-m.getArgAsFloat(0))-0.5)*2;
            //center.z = ((1-m.getArgAsFloat(0)));//*2;
            
        }
        if(m.getAddress() == "/Radius/x"){
            circleRadius = m.getArgAsFloat(0)*7;
        }
        if(m.getAddress() == "/Speed/x"){
            circleSpeed = m.getArgAsFloat(0);
        }
        
        if(m.getAddress() == "/Type/x"){
            type = m.getArgAsInt32(0);
        }
    }
    
    time += circleSpeed;
    
    //Circle 1
    
    if(type==2) {
        for(int i=0;i<lampPositions.size();i++){
       float t = time/100.0;//sin(ofGetElapsedTimeMillis()/2000.0)*3;
       float x = center.x + sin(i*TWO_PI/8+t)* (circleRadius);
       float y = center.y + cos(i*TWO_PI/8+t)* (circleRadius);
       
       lampPositions[i].x = x;
       lampPositions[i].y = y;
       lampPositions[i].z = center.z;
       
    }
   
    //Circle 2
    } else if(type==1) {
    for(int i=0;i<lampPositions.size();i++){
        
        float t = floor(time/100.0);
        
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
        
        lampPositions[i].x = x + center.x;
        lampPositions[i].y = y + center.y;
        lampPositions[i].z = center.z;
    }
    }

    for(int i=0;i<lampPositions.size();i++){
        ofxOscMessage m;
        m.setAddress("/sharpy");
        m.addIntArg(i+1); // device number
        m.addFloatArg(lampPositions[i].x);
        m.addFloatArg(lampPositions[i].z);
        m.addFloatArg(lampPositions[i].y);
        osc.sendMessage(m);
    }
  }

//--------------------------------------------------------------
void testApp::draw() {
//    glScaled(ofGetWidth(), ofGetHeight(), 1);
    
    
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
    
    
    

    
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
//	flock.addBoid(x,y);
}






