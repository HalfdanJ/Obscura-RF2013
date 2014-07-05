//
//  mapping.h
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#pragma once

#include "ofMain.h"
#include "Defines.h"
#include "ofxSvg.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

struct InputTriangle;

class Corner {
public:
    
    Corner(){
        anchor1 = nil;
        anchor2 = nil;
        
        randomSeed = ofVec3f(ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1));

    }
    
    ofVec3f getPos(float f){
        ofVec3f anchor;
        if(anchor1){
            anchor = anchor1->getPos(1.0) * anchorRatio;
            anchor += anchor2->getPos(1.0) * (1.-anchorRatio);
            anchor *= ofVec3f(0,0,1);
        }
        ofVec3f orig = origPos + anchor;
        
        ofVec3f goal = pos;// + anchor;
        
        return orig * (1.-f) + goal *f;
    }

    
    int uid;
    vector<InputTriangle*> triangles;
    vector<Corner*> joinedCorners;
    
    map<Corner*, Corner*> division;
    ofVec3f pos;
    ofVec3f origPos;
    ofVec3f randomSeed;
    
    Corner * anchor1, *anchor2;
    float anchorRatio;
    
    //todo: ofVec3f worldPos; for using with real lamp posisitons
    void addTriangleReference(InputTriangle* triangle);
    
    void createDivisionCorners();
};

class InputTriangle {
public:
    int uid;
    void debugDraw();
    
    ofPoint centroid;
    ofPolyline polyline;
    
    Corner * corners[3];
    ofMesh mesh;
    ofColor color;
    
    InputTriangle();
    ~InputTriangle();
    
    ofVec3f getCenter();
};

class Mapping {	
public:
    Mapping() {};
    virtual ~Mapping() {};
    
    vector<InputTriangle*> triangles;
    vector<Corner*> corners;
    
    void load(string xmlfile, string svgfile);
    void generate();
    void save();
    void exit();
    void debugDraw();
    void drawGuide();
    
    //InputTriangle * selectedTriangle;
    Corner * selectedCorner;
    int selectedCornerId = 0;
    
    void nextCorner();
    void prevCorner();
    void selectCornerAt(int x, int y, int r);
    void updateMeshes();
    
    void drawMask();
    void generateMask();
    ofFbo outputMask;

    
protected:
    string svgFilename;
    string xmlFilename;
    ofxXmlSettings settings;
    ofxSVG svg;
};

