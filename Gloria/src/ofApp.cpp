#include "ofApp.h"

void ofApp::setup() {
    
    //oscSender.setup("HalfdanJ.local", 6745);
    oscReceiver.setup(OSCRECEIVEPORT);
    
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    glEnable(GL_LINES);
    ofSetWindowTitle("Obscure Glorius Control 2014");
    
    syphonOut.setName("Gloria Main");
    fboOut.allocate(OUTWIDTH, OUTHEIGHT);
    
    syphonIn = new ofxSyphonClient();
    
    syphonIn->setApplicationName("Millumin");
    syphonIn->setServerName("");
    syphonIn->setup();
    
    //register for our directory's callbacks
    ofAddListener(directory.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(directory.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(directory.events.serverRetired, this, &ofApp::serverRetired);
    dirIdx = -1;
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    mapping = new Mapping();
    mapping->load("mapping.xml", "input1.svg");
    
    // effects scenes
    
    // Set up the scenes, all scenes is a subclass of SceneContent, don't call draw, setup and update directly it is taken care of thorugh the scene.
    
    triangles = new Triangles;
    scenes.push_back(triangles);
    
    perlinWaves = new PerlinWaves();
    scenes.push_back(perlinWaves);
    
    ofFbo::Settings fboSettings;
    fboSettings.height = OUTHEIGHT;
    fboSettings.width = OUTWIDTH;
    fboSettings.numSamples = 4;
    fboSettings.useDepth = false;
    
    fboOut.allocate(fboSettings);

    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->mapping = mapping;
        scenes[i]->syphonIn = syphonIn;
        scenes[i]->setupScene(OUTWIDTH, OUTHEIGHT, i);
    }
    
    setGUI();
}


void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}


//--------------------------------------------------------------
void ofApp::update() {
    
    
    // send some fun stuff to the sharpy army
    /*
    ofxOscMessage m;
    
    m.setAddress("/sharpy");
    m.addIntArg(0); // device number
    m.addFloatArg(sin(ofGetElapsedTimeMillis()/4500.) * 8.); // x
    m.addFloatArg(0); // y
    m.addFloatArg(sin(ofGetElapsedTimeMillis()/4000.) * 2.); // z
    oscSender.sendMessage(m);*/
    
    while(oscReceiver.hasWaitingMessages()){
        
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);

	//cout<<m.getAddress()<<endl;

        for(int i=0; i<scenes.size();i++) {
            scenes[i]->parseOscMessage(&m);
        }
    }

    // Scenes
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->updateScene();
    }
    
    // OSC in listen
    
    // Draw scene fbo's
    ofPushStyle();
    ofNoFill();
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->drawScene();
    }
    ofPopStyle();
    
    ofPushStyle();{
        fboOut.begin();{
            ofClear(0, 0);
            
            for(int i=0; i<scenes.size(); i++) {
                ofSetColor(255,255,255,scenes[i]->opacity*255);
                
                if(scenes[i]->enabled) {
                    scenes[i]->fbo.draw(0,0);
                }
            }
        }fboOut.end();
    } ofPopStyle();
    
    syphonOut.publishTexture(&fboOut.getTextureReference());
}


void ofApp::draw() {
    
    ofDisableDepthTest();
    ofBackground(0, 0, 0);
    ofSetColor(255,255,255,255);
    
    ofPushMatrix();{
        ofTranslate(300, 40);
        
        ofScale(0.2, 0.2);
        ofBackground(0);
        
        ofSetColor(255,255,255,255);
        ofNoFill();
        ofSetLineWidth(1);
        
        ofRect(-1, -1, fboOut.getWidth()+2, fboOut.getHeight()+2);
        fboOut.draw(0, 0);
        
        if(drawGuide) {
            //ofSetColor(255,255,255,96);
            drawGrid();
            debugDraw();
        }
        
    }ofPopMatrix();
    
    ofPushMatrix();{
        ofTranslate(300, 320);
        
        if(syphonIn->isSetup()){
            
            ofSetColor(255);
            ofSetLineWidth(1);
            ofRect(-1, -1, 260* syphonIn->getWidth()/syphonIn->getHeight()+2, 260+2);
            syphonIn->draw(0, 0, 260* syphonIn->getWidth()/syphonIn->getHeight(), 260);
            
            ofDrawBitmapString("Syphon input - (Press 'i' to change)", 10,18);
            ofDrawBitmapString(syphonIn->getApplicationName(), 10,34);
        }
    }ofPopMatrix();
    
    
    ofSetColor(255);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth()-200, 20);
    
    if(mapping->selectedCorner) {
        ofDrawBitmapString("Selected Corner: " + ofToString(mapping->selectedCorner->uid) + " pos: " + ofToString(mapping->selectedCorner->pos), ofGetWidth()-600, 20);
    }
    
}


//------------------------------------------------------------
void ofApp::debugDraw() {
    mapping->debugDraw();
}

void ofApp::drawGrid() {
    ofSetLineWidth(1);
    for(int i =0; i<mapping->triangles.size();i++) {
        mapping->triangles[i]->mesh.drawWireframe();
    }
}

void ofApp::setGUI()
{
    
    float dim = 16;
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float width = 255-xInit;
    hideGUI = false;
    
    guiTabBar = new ofxUITabBar();
    mainGui = new ofxUICanvas();
    
    mainGui->setFont("GUI/Arial.ttf");
    
    mainGui->addLabel("Gloria");
    
    mainGui->addToggle("Draw guide", &drawGuide);
    mainGui->autoSizeToFitWidgets();
    ofAddListener(mainGui->newGUIEvent,this,&ofApp::guiEvent);
    
    
    guiTabBar->setFont("GUI/Arial.ttf");
    guiTabBar->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->setSceneGui();
        guiTabBar->addCanvas(scenes[i]->gui);
        scenes[i]->gui->setColorBack(ofColor(0,100 + 20*i,0,255));
        guis.push_back(scenes[i]->gui);
    }
    
    guiTabBar->autoSizeToFitWidgets();
    ofAddListener(guiTabBar->newGUIEvent,this,&ofApp::guiEvent);
    
    guiTabBar->setPosition(0, mainGui->getRect()->height+10);
    //guiTabBar->setScrollAreaToScreenHeight();
    
    mainGui->setColorBack(ofColor(0,150,200,255));
    guiTabBar->setColorBack(ofColor(0,100,0,255));
    
    mainGui->loadSettings("GUI/guiMainSettings.xml");
    guiTabBar->loadSettings("GUI/guiSettings.xml", "ui-");
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'i') {
        dirIdx++;
        if(dirIdx > directory.size() - 1)
        dirIdx = 0;
        
        if(directory.isValidIndex(dirIdx)){
            syphonIn->setServerName(directory.getServerList()[dirIdx].serverName);
            syphonIn->setApplicationName(directory.getServerList()[dirIdx].appName);
        }
    }
    
    if(key == 'n') {
        mapping->nextCorner();
    }
    
    if(key == 'm') {
        mapping->prevCorner();
    }
    
    if(mapping->selectedCorner) {
        if(key == OF_KEY_UP) {
            mapping->selectedCorner->pos.z += 1;
            mapping->updateMeshes();
        }
    
        if(key == OF_KEY_DOWN) {
            mapping->selectedCorner->pos.z -= 1;
            mapping->updateMeshes();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit()
{
    mainGui->saveSettings("GUI/guiMainSettings.xml");
    guiTabBar->saveSettings("GUI/guiSettings.xml", "ui-");
    mapping->save();
    
    delete guiTabBar;
    delete mapping;
    
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
}
