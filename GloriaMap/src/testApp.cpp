#include "testApp.h"


void testApp::setup() {
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofSetFrameRate(60);
    
    ofSetWindowTitle("Obscure Glorious Control");
    
    syphonOut.setName("Gloria");
    
    //fboOut.allocate(OUTWIDTH, OUTHEIGHT);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    // load the svg (transforms should be flattened before)
    
    svg.load("input1.svg");
    
    int numTriangles = 0;
    int maxTriangleSize = 1000000;
    float cornerThreshold = 20;

    for (int i = 0; i < svg.getNumPath(); i++){
		ofPath p = svg.getPathAt(i);
		// svg defaults to non zero winding which doesn't look so good as contours
		//p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		vector<ofPolyline>& lines = p.getOutline();
        
		for(int j=0;j<(int)lines.size();j++){
                        
			if(lines[j].getArea() < maxTriangleSize) {
                
                InputTriangle * triangle = new InputTriangle;
                
                triangle->index = numTriangles;
                triangle->polyline = lines[j];
                triangle->centroid = lines[j].getCentroid2D();
                
                cout<<"Creating triangle..."<<endl;
                
                // For each of 3 vertices in triangle create a corner pointer
                for(int vi=0; vi<3; vi++) {
                    
                    bool set = false;
                    ofVec2f vert = triangle->polyline.getVertices()[vi];
                    
                    // Loop through all corners in all triangels and set a pointer if the corner already exists
                    for(int ti=0; ti<triangles.size(); ti++) {
                        
                        for(int cti = 0; cti < 3; cti++) {
                            
                            if (vert.distance(triangles[ti]->corners[cti]->pos) < cornerThreshold) {
                                cout<<"Setting corner: "<<vert.x<<", "<<vert.y<<endl;
                                triangle->corners[vi] = triangles[ti]->corners[cti];
                                set = true;
                            }
                        }
                    }
                    
                    if(!set) {
                        cout<<"Creating corner: "<<vert.x<<", "<<vert.y<<endl;
                        
                        triangle->corners[vi] = new Corner;
                        triangle->corners[vi]->pos = vert;
                        
                        triangle->corners[vi]->triangles.push_back(triangle);
                        
                        corners.push_back(triangle->corners[vi]);
                    }
                    
                }
                
                
                triangle->color.set(ofRandom(100,255));
                
                // add as a mesh - maybe add a normal pointing out for light effects
                for(int c=0; c<3; c++) {
                    triangle->mesh.addVertex(triangle->corners[c]->pos);
                }
                triangle->mesh.addTriangle(0, 1, 2);
                
                
                triangles.push_back(triangle);
                
                numTriangles += 1;
                
            }
        }
	}
    
    
    cout<<endl<<"Created: "<<triangles.size()<<" triangles with "<<corners.size()<<" unique corners"<<endl;
    
    //setGUI();
    //gui->setDrawBack(true);
    //gui->loadSettings("GUI/guiSettings.xml");
    
}

//--------------------------------------------------------------
void testApp::update() {
    
    
}




void testApp::draw() {
    
    ofPushMatrix();
    ofScale(0.4, 0.4);
    ofBackground(0);
    
    
    
    
    for(int i =0; i<triangles.size();i++) {
        ofSetColor( ofNoise(triangles[i]->polyline.getCentroid2D().x - ofGetElapsedTimef())*255 );
        
        
        
        
        triangles[i]->polyline.draw();
        
    }
    
    
    //debugDraw();
    
    ofPopMatrix();
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
    ofDrawBitmapString(ofToString(index), polyline.getCentroid2D());
    
    ofSetColor(255, 0, 0, 60);
    for(int i=0; i<polyline.getVertices().size(); i++) {
        ofCircle(polyline.getVertices()[i].x, polyline.getVertices()[i].y, 20);
    }
    

    
}


//------------------------------------------------------------
void testApp::debugDraw() {
    
    for(int i =0; i<triangles.size();i++) {
        triangles[i]->debugDraw();
    }
    
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
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

//--------------------------------------------------------------
void testApp::exit()
{
    
    //gui->saveSettings("GUI/guiSettings.xml");
    //delete gui;
    
}

void testApp::setGUI()
{
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
}