#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//ofSetVerticalSync(true);
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
	
	// this sets the camera's distance from the object
	cam.setDistance(100);
	
	ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
    loadXml();
    
//    message = ofToDataPath("lamps.xml",true);
    TTF.loadFont("mono.ttf", 7);
    
    dmx.connect(0);
    dmx.setChannels(512);
    
    ofSetFrameRate(30);
    
    osc = new ofxOscReceiver();
    osc->setup(6000);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    while(osc->hasWaitingMessages() > 0){
        ofxOscMessage  m;
        osc->getNextMessage(&m);
        
        if(m.getAddress() == "/sharpy"){
            int lamp = m.getArgAsInt32(0);
            ofVec3f p = ofVec3f(m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3));
            if(lamp == 0){
                for(int i=0;i<lamps.size();i++){
                    lamps[i].pointAt(p);
                }
            }
            if(lamps.size() > lamp-1){
                lamps[lamp-1].pointAt(p);
            }
        } else if(m.getAddress() == "/sharpy/x"){
            int lamp = 0;
            ofVec3f p = lamps[0].dest;
            p.x = (m.getArgAsFloat(0)-0.5)*50;
            if(lamp == 0){
                for(int i=0;i<lamps.size();i++){
                    lamps[i].pointAt(p);
                }
            }
        } else if(m.getAddress() == "/sharpy/y"){
            int lamp = 0;
            ofVec3f p = lamps[0].dest;
            p.y = ((m.getArgAsFloat(0))-0.5)*50;
            if(lamp == 0){
                for(int i=0;i<lamps.size();i++){
                    lamps[i].pointAt(p);
                }
            }
        } else if(m.getAddress() == "/sharpy/z"){
            int lamp = 0;
            ofVec3f p = lamps[0].dest;
            p.z = (1-m.getArgAsFloat(0)-0.5)*30;
            if(lamp == 0){
                for(int i=0;i<lamps.size();i++){
                    lamps[i].pointAt(p);
                }
            }
        } else {
            cout<<m.getAddress()<<endl;
        }

        
    }
    
    if(ofGetFrameNum() % 30 ==0){
        loadXml();
    }
    
    for(int i=0;i<lamps.size();i++){
        
        if(lamps[i].coldCountdown > 0){
            lamps[i].coldCountdown -= 1.0/ofGetFrameRate();
        }
        
        float pan = ofClamp(lamps[i].getDmxPan(), 0, 255);
        float tilt = ofClamp(lamps[i].getDmxTilt(),0,255);
        
        int majorPan = floor(pan);
        int majorTilt = floor(tilt);
        
        int minorPan = (pan - majorPan)* 255 ;
        int minorTilt = (tilt - majorTilt)* 255 ;

        dmx.setLevel(lamps[i].dmxAddress+lamps[i].dmxOffset, majorPan);
        dmx.setLevel(lamps[i].dmxAddress+lamps[i].dmxOffset+1, minorPan);
        
        dmx.setLevel(lamps[i].dmxAddress+lamps[i].dmxOffset+2, majorTilt);
        dmx.setLevel(lamps[i].dmxAddress+lamps[i].dmxOffset+3, minorTilt);
        
/*        dmx.setLevel(lamps[i].dmxAddress+lamps[i].dmxOffset+2, 128);
        dmx.setLevel(lamps[i].dmxAddress+lamps[i].dmxOffset+3, 0);
  */      
        //dmx.setLevel(lamps[i].dmxAddress+0, 0);
        //dmx.setLevel(lamps[i].dmxAddress+1, 0);
        if(lamps[i].coldCountdown > 0){
            dmx.setLevel(lamps[i].dmxAddress+17, 0);
        } else {
            dmx.setLevel(lamps[i].dmxAddress+17, lamps[i].dim);
            
        }
        dmx.setLevel(lamps[i].dmxAddress+3, 0);
        dmx.setLevel(lamps[i].dmxAddress+4, 0);
        dmx.setLevel(lamps[i].dmxAddress+5, 0);
        dmx.setLevel(lamps[i].dmxAddress+6, 0);
        dmx.setLevel(lamps[i].dmxAddress+7, 0);
        dmx.setLevel(lamps[i].dmxAddress+8, 0);
        dmx.setLevel(lamps[i].dmxAddress+14, 0);//reset
        dmx.setLevel(lamps[i].dmxAddress+15, 0);//lamp
        
        dmx.setLevel(lamps[i].dmxAddress+16, 10);
        
    }
    
//      dmx.setLevel(13, 128+ sin(ofGetElapsedTimeMillis()/5000.)*128);
    dmx.update();

}

//--------------------------------------------------------------
void testApp::draw(){
    cam.begin();
	ofRotateX(ofRadToDeg(.5));
	ofRotateY(ofRadToDeg(-.5));
	
	ofBackground(0);
	
	ofPushMatrix();
	ofTranslate(0,-20,20);

	ofSetColor(0,0,255);
	ofFill();
    
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);
    ofSetColor(255, 255, 255, 50);
    ofRect(-50, -50, 0, 100, 100);
    ofPopMatrix();
    
    message  = "";
    for(int i=0;i<lamps.size();i++){
        ofPushMatrix();
        glScaled(3,3,3);
        
        
        ofSetColor(0, 255, 0);
        ofLine(lamps[i].position.x, lamps[i].position.y, lamps[i].position.z,
               lamps[i].dest.x, lamps[i].dest.y, lamps[i].dest.z);
        
        ofTranslate(lamps[i].position.x, lamps[i].position.y, lamps[i].position.z);

        ofRotate(-lamps[i].getPan()-90, 0, 1, 0);
        ofRotate(lamps[i].getTilt(), 1, 0, 0);
        
        message += "Pan: "+ofToString(lamps[i].getPan())
                +" ("+ofToString(lamps[i].getDmxPan())
                +")  tilt: "+ofToString(lamps[i].getTilt())
                +" ("+ofToString(lamps[i].getDmxTilt())+")\n";
        
        ofSetColor(255, 0, 0,80);
        ofFill();
        ofBox(1);
        
        

        ofNoFill();
        ofSetColor(255);
        ofBox(1);
        
        
        ofPopMatrix();
        
    }
	
	ofPopMatrix();
	cam.end();
    
    
    ofSetColor(240, 240, 240);
/*	TTF.drawString(message, 170, 12);*/
    ofDrawBitmapString(message, 170,12);
}

void testApp::loadXml(){
  
   // lamps.clear();
    
    message = "loading lamps.xml";
    
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFile("lamps.xml") ){
		message = "lamps.xml loaded!";
	}else{
		message = "unable to load lamps.xml ";
	}
    
    
    float dmxPanMin = XML.getAttribute("PANDMX", "min", 0.0);
    float dmxPanMax = XML.getAttribute("PANDMX", "max", 0.0);


    float panMin = XML.getAttribute("PAN", "min", 0.0);
    float panMax = XML.getAttribute("PAN", "max", 0.0);
    
/*    float tiltRange = XML.getAttribute("TILT", "range", 0.0);
    float tiltMin = -180-(tiltRange - 180)/2;
    float tiltMax = (tiltRange - 180)/2;
  */
    
    float tiltMin = XML.getAttribute("TILT", "min", 0.0);
    float tiltMax = XML.getAttribute("TILT", "max", 0.0);
    
    float dim = XML.getAttribute("DIM", "value", 0.0);
    float floor = XML.getAttribute("FLOOR", "value", 0.0);

    

    XML.pushTag("LAMPS");
    int numLamps = XML.getNumTags("LAMP");
    
    for(int i=0;i<numLamps ;i++){
        XML.pushTag("LAMP",i);
        
        float dmxTiltMin = XML.getAttribute("TILTDMX", "min", 127.);
        float dmxTiltMax = XML.getAttribute("TILTDMX", "max", 256.);

        
        if(lamps.size() <= i){
            lamps.push_back(Lamp());
        } 
        
        lamps[i].position.x = XML.getAttribute("POSITION", "x", 0.0);
        lamps[i].position.y = XML.getAttribute("POSITION", "y", 0.0)-floor;
        lamps[i].position.z = XML.getAttribute("POSITION", "z", 0.0);
        
        lamps[i].dmxPanMin = dmxPanMin;
        lamps[i].dmxPanMax = dmxPanMax;

        lamps[i].dmxTiltMin = dmxTiltMin;
        lamps[i].dmxTiltMax = dmxTiltMax;
        
        lamps[i].panMin = panMin;
        lamps[i].panMax = panMax;
        
        lamps[i].tiltMin = tiltMin;
        lamps[i].tiltMax = tiltMax;
        
        lamps[i].dim = dim;
        
        if(XML.getNumTags("ROTATION")){
            lamps[i].rotation.x = XML.getAttribute("ROTATION", "x", 0.0);
            lamps[i].rotation.y = XML.getAttribute("ROTATION", "y", 0.0);
            lamps[i].rotation.z = XML.getAttribute("ROTATION", "z", 0.0);
        }
        
        lamps[i].dmxAddress = XML.getAttribute("DMX", "address", 1);
        lamps[i].dmxOffset = XML.getAttribute("DMX", "offset", 1);

        
        XML.popTag();
        
        //        XML.
    }
    XML.popTag();
    
    
    if (XML.getNumTags("LOCALDEST")) {
        ofVec3f dir = ofVec3f(XML.getAttribute("LOCALDEST", "x", 0.0)
                ,XML.getAttribute("LOCALDEST", "y", 0.0),
                              XML.getAttribute("LOCALDEST", "z", 0.0));
        
        
        for(int i=0;i<numLamps ;i++){
            ofVec3f p = lamps[i].position + dir;

            lamps[i].pointAt(p);
        }
    }

    
    if (XML.getNumTags("DEST")) {
        for(int i=0;i<numLamps ;i++){
            lamps[i].pointAt(ofVec3f(XML.getAttribute("DEST", "x", 0.0)
                                     ,XML.getAttribute("DEST", "y", 0.0),
                                     XML.getAttribute("DEST", "z", 0.0)));
        }
    }
    
    if (XML.getNumTags("ROT")) {
        for(int i=0;i<numLamps ;i++){
            lamps[i].setRotation(XML.getAttribute("ROT", "pan", 0.0), XML.getAttribute("ROT", "tilt", 0.0));
        }
    }

    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'l'){
        loadXml();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}