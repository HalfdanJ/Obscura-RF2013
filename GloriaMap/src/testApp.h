#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxSvg.h"
#include "ofxOpenCv.h"

#include "ofxUI.h"

#define PORT 9999
#define OUTHEIGHT 1080
#define OUTWIDTH 1920


struct InputTriangle;

struct WorldPoint {
    ofVec3f pos;
};

struct Corner {
    
    vector<InputTriangle*> triangles;
    
    vector<Corner*> joinedCorners;

    ofVec2f pos;
    ofVec3f worldPos;


    void addTriangleReference(InputTriangle* triangle) {


	bool found = false;
	for(int i=0;i<triangles.size();i++) {
	    if(triangle == triangles[i]) {
		found = true;
	    }
	}

	if(!found) {
	    triangles.push_back(triangle);
	}


    }
};


struct InputTriangle {
    
    int index;
    void debugDraw();
    ofPoint centroid;
    ofPolyline polyline;
    
    Corner * corners[3];
    
    ofMesh mesh;
    
    ofColor color;

};


struct Walker {
    ofPolyline line;
    vector<ofVec2f> points;

    Corner * corner;

};


class testApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    void debugDraw();
    
    void exit();
        
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;

    ofxSyphonServer syphonOut;
    
    void setGUI(); 
    ofxUICanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
    ofCamera cam;
    ofColor bg;
    ofFbo fboOut;
    
    
    ofxSVG svg;
    vector<InputTriangle*> triangles;
    vector<Corner*> corners;
    
    ofxXmlSettings XML;
    
    vector<WorldPoint*> worldPoints;
    
    // stuff for specifc scenes

    vector<Walker> walkers;
    
};
