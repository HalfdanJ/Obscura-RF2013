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

struct Corner {
    
    vector<InputTriangle*> triangles;
    
    ofVec2f pos;
    ofVec3f worldPos;
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


/*struct Walker {

}*/


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
    
    ofImage image;
    
    vector<InputTriangle*> triangles;
    vector<Corner*> corners;
    
    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofxCvContourFinder 	contourFinder;
    
    int 				threshold;
    bool				bLearnBakground;
    
    
    ofxSVG svg;
    
    // stuff for specifc scenes


    
};
