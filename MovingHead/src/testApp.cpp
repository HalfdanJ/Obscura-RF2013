#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
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

}

//--------------------------------------------------------------
void testApp::update(){

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
    
    
    ofSetColor(255, 0, 0);
    for(int i=0;i<lamps.size();i++){
        ofPushMatrix();
        
        ofTranslate(lamps[i].position.x, lamps[i].position.y, lamps[i].position.z);
        ofBox(5);
        
        
        ofPopMatrix();
        
    }
	
	ofPopMatrix();
	cam.end();
    
    
    ofSetColor(240, 240, 240);
	TTF.drawString(message, 170, 12);
}

void testApp::loadXml(){
    
    lamps.clear();
    
    message = "loading lamps.xml";
    
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFile("lamps.xml") ){
		message = "lamps.xml loaded!";
	}else{
		message = "unable to load lamps.xml ";
	}
    
    
    XML.pushTag("LAMPS");
    int numLamps = XML.getNumTags("LAMP");
    cout<<numLamps<<" lamps"<<endl;
    
    for(int i=0;i<numLamps ;i++){
        XML.pushTag("LAMP",i);
        
        Lamp lamp = Lamp();
        
        lamp.position.x = XML.getAttribute("POSITION", "x", 0.0);
        lamp.position.x = XML.getAttribute("POSITION", "y", 0.0);
        lamp.position.x = XML.getAttribute("POSITION", "z", 0.0);
        
        lamps.push_back(lamp);
        
        //        XML.
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