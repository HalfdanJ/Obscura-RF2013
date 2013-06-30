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
    
    
    ofSetFrameRate(30);
    
}

//--------------------------------------------------------------
void testApp::update(){
    if(ofGetFrameNum() % 30 ==0){
        loadXml();
    }
    
    for(int i=0;i<lamps.size();i++){
        
        float pan = lamps[i].getPan();
        float tilt = lamps[i].getTilt();
        
        int majorPan = floor(pan);
        int majorTilt = floor(tilt);
        
        int minorPan = (pan - majorPan)* 255 ;
        int minorTilt = (tilt - majorTilt)* 255 ;

/*        dmx.setLevel(dmxAddress, majorPan);
        dmx.setLevel(2, 255 - level);*/
        
    }
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
        ofBox(5);
        
        

        ofNoFill();
        ofSetColor(255);
        ofBox(5);
        
        
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

    float dmxTiltMin = XML.getAttribute("TILTDMX", "min", 0.0);
    float dmxTiltMax = XML.getAttribute("TILTDMX", "max", 0.0);

    float panMin = XML.getAttribute("PAN", "min", 0.0);
    float panMax = XML.getAttribute("PAN", "max", 0.0);
    
    float tiltMin = XML.getAttribute("TILT", "min", 0.0);
    float tiltMax = XML.getAttribute("TILT", "max", 0.0);

    

    XML.pushTag("LAMPS");
    int numLamps = XML.getNumTags("LAMP");
    
    for(int i=0;i<numLamps ;i++){
        XML.pushTag("LAMP",i);
        
        if(lamps.size() <= i){
            lamps.push_back(Lamp());
        } 
        
        lamps[i].position.x = XML.getAttribute("POSITION", "x", 0.0);
        lamps[i].position.y = XML.getAttribute("POSITION", "y", 0.0);
        lamps[i].position.z = XML.getAttribute("POSITION", "z", 0.0);
        
        lamps[i].dmxPanMin = dmxPanMin;
        lamps[i].dmxPanMax = dmxPanMax;

        lamps[i].dmxTiltMin = dmxTiltMin;
        lamps[i].dmxTiltMax = dmxTiltMax;
        
        lamps[i].panMin = panMin;
        lamps[i].panMax = panMax;
        
        lamps[i].tiltMin = tiltMin;
        lamps[i].tiltMax = tiltMax;
        
        lamps[i].dmxAddress = XML.getAttribute("DMX", "address", 1);

        
        XML.popTag();
        
        //        XML.
    }
    XML.popTag();
    
    
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