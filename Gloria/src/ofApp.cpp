#include "ofApp.h"

void ofApp::setup() {
    
    //oscSender.setup("HalfdanJ.local", 6745);
    oscReceiver.setup(OSCRECEIVEPORT);
    
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
        
    ofSetWindowTitle("Obscure Glorious Control");
    
    syphonOut.setName("Gloria Main");
    fboOut.allocate(OUTWIDTH, OUTHEIGHT);
    
    syphonIn.setApplicationName("Millumin");
    syphonIn.setServerName("");
    syphonIn.setup();
    
    //register for our directory's callbacks
    ofAddListener(directory.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(directory.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(directory.events.serverRetired, this, &ofApp::serverRetired);
    dirIdx = -1;
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    // load the worldpoints
    
    XML.loadFile("calibration.xml");
    
    int worldPointTags = XML.getNumTags("WORLDPOINT:PT");
    
    if(worldPointTags > 0){
        
        for(int i = 0; i < worldPointTags; i++){
            int x = XML.getValue("PT:X", 0, i);
            int y = XML.getValue("PT:Y", 0, i);
            int z = XML.getValue("PT:Z", 0, i);
            
            WorldPoint * wp = new WorldPoint();
            wp->pos.set(ofVec3f(x,y,z));
            mapping->worldPoints.push_back(wp);
        }
        
    }
    
    // load the svg (transforms should be flattened before)
    // todo - try to load this data from xml files
    
    mapping = new Mapping();
    
    mapping->svg.load("input1.svg");
    
    int numTriangles = 0;
    int maxTriangleSize = 1000000;
    float cornerThreshold = 20;
    
    for (int i = 0; i < mapping->svg.getNumPath(); i++){
		ofPath p = mapping->svg.getPathAt(i);
		// svg defaults to non zero winding which doesn't look so good as contours
		//p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		vector<ofPolyline>& lines = p.getOutline();
        
		for(int j=0;j<(int)lines.size();j++){
            
			if(lines[j].getArea() < maxTriangleSize) {
                
                InputTriangle * triangle = new InputTriangle;
                
                triangle->index = numTriangles;
                triangle->polyline = lines[j];
                triangle->centroid = lines[j].getCentroid2D();
                
                //cout<<"Creating triangle..."<<endl;
                
                // For each of 3 vertices in triangle create a corner pointer
                for(int vi=0; vi<3; vi++) {
                    
                    bool set = false;
                    ofVec2f vert = triangle->polyline.getVertices()[vi];
                    
                    // Loop through all corners in all triangels and set a pointer if the corner already exists
                    for(int ti=0; ti<mapping->triangles.size(); ti++) {
                        
                        for(int cti = 0; cti < 3; cti++) {
                            
                            if (vert.distance(mapping->triangles[ti]->corners[cti]->pos) < cornerThreshold) {
                                //cout<<"Setting corner: "<<vert.x<<", "<<vert.y<<endl;
                                triangle->corners[vi] = mapping->triangles[ti]->corners[cti];
                                
                                triangle->corners[vi]->addTriangleReference(triangle);
                                
                                set = true;
                            }
                        }
                    }
                    
                    if(!set) {
                        //cout<<"Creating corner: "<<vert.x<<", "<<vert.y<<endl;
                        
                        triangle->corners[vi] = new Corner;
                        triangle->corners[vi]->pos = vert;
                        
                        mapping->corners.push_back(triangle->corners[vi]);
                        
                        triangle->corners[vi]->addTriangleReference(triangle);
                    }
                    
                }
                
                triangle->color.set(ofRandom(100,255));
                
                // add as a mesh - maybe add a normal pointing out for light effects
                for(int c=0; c<3; c++) {
                    triangle->mesh.addVertex(triangle->corners[c]->pos);
                }
                triangle->mesh.addTriangle(0, 1, 2);
                
                mapping->triangles.push_back(triangle);
                numTriangles += 1;
                
            }
        }
	}
    
    
    for(int i=0;i<mapping->corners.size();i++){
        cout<<mapping->corners[i]->pos.x<<endl;
        for(int u=0 ; u<mapping->corners[i]->triangles.size() ; u++){
            for(int j=0;j<3;j++){
                if(mapping->corners[i]->triangles[u]->corners[j] != mapping->corners[i]){
                    bool alreadyAdded = false;
                    for(int k=0;k<mapping->corners[i]->joinedCorners.size();k++){
                        if(mapping->corners[i]->joinedCorners[k] == mapping->corners[i]->triangles[u]->corners[j]){
                            alreadyAdded = true;
                            //cout<<"Already added"<<endl;
                        }
                    }
                    if(!alreadyAdded){
                        mapping->corners[i]->joinedCorners.push_back(mapping->corners[i]->triangles[u]->corners[j]);
                    }
                }
            }
        }
    }
    
    cout<<endl<<"Created: "<<mapping->triangles.size()<<" triangles with "<<mapping->corners.size()<<" unique corners"<<endl;
    
    // effects scenes
    
    // Set up the scenes, all scenes is a subclass of SceneContent, don't call draw, setup and update directly it is taken care of thorugh the scene.
    
    //triangles.mapping = &mapping;
    //triangles.syphon = &syphonIn;
    //scenes.push_back(&triangles);
    
    perlinWaves = new PerlinWaves();
    scenes.push_back(perlinWaves);

    //tesselator.mapping = &mapping;
    //scenes.push_back(&tesselator);
    
    //triBlobs.mapping = &mapping;
    //scenes.push_back(&triBlobs);
    
    //hardNoise.mapping = &mapping;
    //scenes.push_back(&hardNoise);
    
    //quickTrail.mapping = &mapping;
    //scenes.push_back(&quickTrail);
    
    ofFbo::Settings fboSettings;
    fboSettings.height = OUTHEIGHT;
    fboSettings.width = OUTWIDTH;
    fboSettings.numSamples = 8;
    fboSettings.useDepth = false;
    //fboSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    
    fboOut.allocate(fboSettings);

    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->mapping = mapping;
        scenes[i]->setupScene(OUTWIDTH, OUTHEIGHT, i);
    }
    
    setGUI();
    guiTabBar->setDrawBack(true);
    //guiTabBar->setScrollAreaToScreenHeight();
    
    guiTabBar->loadSettings("GUI/guiSettings.xml", "ui-");
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
    
}


void ofApp::draw() {
    
    
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->drawScene();
    }
    
    
    fboOut.begin();
    
    ofClear(0, 0);
    
    for(int i=0; i<scenes.size(); i++) {
        ofSetColor(255,255,255,scenes[i]->opacity*255);
        
        if(scenes[i]->enabled) {
            scenes[i]->fbo.draw(0,0);
        }
    }
    fboOut.end();
    
    ofPushMatrix();
    ofTranslate(300, 40);
    
    ofScale(0.2, 0.2);
    ofBackground(0);
    
    ofSetColor(255,255,255,255);
    ofNoFill();
    ofSetLineWidth(1);
    ofRect(-1, -1, fboOut.getWidth()+2, fboOut.getHeight()+2);
    fboOut.draw(0, 0);
    
    if(drawGuide) {
        ofSetColor(255,255,255,96);
        drawGrid();
    }

    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(300, 320);
    
    
    if(syphonIn.isSetup()){
        
        ofSetColor(255);
        ofSetLineWidth(1);
        ofRect(-1, -1, 260* syphonIn.getWidth()/syphonIn.getHeight()+2, 260+2);
        syphonIn.draw(0, 0, 260* syphonIn.getWidth()/syphonIn.getHeight(), 260);
        
        ofDrawBitmapString("Syphon input - (Press 'i' to change)", 10,18);
        ofDrawBitmapString(syphonIn.getApplicationName(), 10,34);
    }
    
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth()-200, 20);
    
    syphonOut.publishTexture(&fboOut.getTextureReference());

}


//------------------------------------------------------------
void ofApp::debugDraw() {
    for(int i =0; i<mapping->triangles.size();i++) {
        mapping->triangles[i]->debugDraw();
    }
}

void ofApp::drawGrid() {
    for(int i =0; i<mapping->triangles.size();i++) {
        ofSetLineWidth(1);
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
    
    guiTabBar->setFont("GUI/Arial.ttf");
    guiTabBar->setWidgetFontSize(OFX_UI_FONT_SMALL);
    guiTabBar->setColorBack(ofColor(30, 30, 30,200));
    
    guiTabBar->addToggle("Draw guide", &drawGuide);
    
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->setSceneGui();
        guiTabBar->addCanvas(scenes[i]->gui);
        guis.push_back(scenes[i]->gui);
    }
    
    
    guiTabBar->autoSizeToFitWidgets();
    
    ofAddListener(guiTabBar->newGUIEvent,this,&ofApp::guiEvent);
}

void InputTriangle::debugDraw() {
    ofFill();
    ofSetColor(255, 255, 255, 40);
    ofRect(polyline.getBoundingBox());
    
    //path.draw();
    //vector<ofPolyline>& lines = path.getOutline();
    //for(int j=0;j<(int)lines.size();j++){
    
    ofFill();
    ofSetColor(255,255,255,200);
    polyline.draw();
    
    ofSetColor(255, 255, 255, 255);
    ofDrawBitmapString(ofToString(index), centroid);
    
    ofSetColor(255, 0, 0, 60);
    for(int i=0; i<polyline.getVertices().size(); i++) {
        ofCircle(polyline.getVertices()[i].x, polyline.getVertices()[i].y, 20);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'i') {
        dirIdx++;
        if(dirIdx > directory.size() - 1)
        dirIdx = 0;
        
        if(directory.isValidIndex(dirIdx)){
            syphonIn.setServerName(directory.getServerList()[dirIdx].serverName);
            syphonIn.setApplicationName(directory.getServerList()[dirIdx].appName);
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
    guiTabBar->saveSettings("GUI/guiSettings.xml", "ui-");
    delete guiTabBar;
    
    XML.saveFile("calibration.xml");
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
}
