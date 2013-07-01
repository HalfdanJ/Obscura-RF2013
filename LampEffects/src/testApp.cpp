#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(0,0,0);
	ofSetFrameRate(30);
    
    osc.setup("halfdanjOld.local", 6000);
    oscReceiver.setup(5000);
    circleRadius = 4;
    circleSpeed = 0.1;
}

//--------------------------------------------------------------
void testApp::update() {
    
    
    while(oscReceiver.hasWaitingMessages() > 0){
        ofxOscMessage  m;
        oscReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/center/x"){
            center.x = (m.getArgAsFloat(0)-0.5)*2;
            
        }
        if(m.getAddress() == "/center/y"){
            center.y = (m.getArgAsFloat(0)-0.5)*10;
            
        }
        if(m.getAddress() == "/center/z"){
            center.z = ((1-m.getArgAsFloat(0))-0.5)*2;
            
        }
        if(m.getAddress() == "/circle"){
            circleRadius = m.getArgAsFloat(0)*7;
        }
        if(m.getAddress() == "/circleSpeed"){
            circleSpeed = m.getArgAsFloat(0)*3;
        }
        
        
    }

    
    //Circle 1    
  /*
   for(int i=0;i<9;i++){
       
       float t = circleSpeed * ofGetElapsedTimeMillis()/1000.0;//sin(ofGetElapsedTimeMillis()/2000.0)*3;
       float x = center.x*10 + sin(i*TWO_PI/8+t)*circleRadius;
       float y = center.z*10 + cos(i*TWO_PI/8+t)*circleRadius;
       
        ofxOscMessage m;
        
        m.setAddress("/sharpy");
        m.addIntArg(i+1); // device number
        m.addFloatArg(x); // x
        m.addFloatArg(center.y); // y
        m.addFloatArg(y); // x
        osc.sendMessage(m);        
    }
   */
    
    
    //Circle 2
    /*
    for(int i=0;i<9;i++){
        
        float t = floor(circleSpeed * ofGetElapsedTimeMillis()/1000.0);
        
        float x = center.x*10 + sin(i*TWO_PI/8+t)*circleRadius;
        float y = center.z*10 + cos(i*TWO_PI/8+t)*circleRadius;
        
        ofxOscMessage m;
        
        m.setAddress("/sharpy");
        m.addIntArg(i+1); // device number
        m.addFloatArg(x); // x
        m.addFloatArg(center.y); // y
        m.addFloatArg(y); // x
        osc.sendMessage(m);
    }*/
    
    
    //Square
    for(int i=0;i<9;i++){
        
        float t = 5* circleSpeed * ofGetElapsedTimeMillis()/1000.0;//sin(ofGetElapsedTimeMillis()/2000.0)*3;
        

        float x, y;
        float phase = fmod(t+4*i/9.0 , 4);
        cout<<phase<<endl;

        if(phase < 1){
            x = (phase ) * circleRadius;
            y = 0;
            
        }
        else if(phase < 2){
            y = ( (phase-1)) * circleRadius;
            x = circleRadius;
            
        }
        else if(phase < 3){
            x = (1-(phase -2)) * circleRadius;
            y = circleRadius;
        }
        else if(phase < 4){
            y = (1-(phase -3)) * circleRadius;
            x = 0;
        }

        
        
        ofxOscMessage m;
        
        m.setAddress("/sharpy");
        m.addIntArg(i+1); // device number
        m.addFloatArg(x); // x
        m.addFloatArg(center.y); // y
        m.addFloatArg(y); // x
        osc.sendMessage(m);
    }

    
    
  }

//--------------------------------------------------------------
void testApp::draw() {
//    glScaled(ofGetWidth(), ofGetHeight(), 1);
    
    ofCircle(center.x*ofGetWidth(), center.z*ofGetHeight(), 10);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
//	flock.addBoid(x,y);
}






