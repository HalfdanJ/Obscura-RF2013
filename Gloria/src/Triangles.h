//
//  LampWalker.h
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "ofxSyphon.h"


class SubTriangle : public InputTriangle{
public:
    vector<SubTriangle*> subTriangles;
    float age;
    ofVec3f normal;
    ofVec3f parentNormal;
    
    int numTriangles(){
        int ret = 1;
        for(int i=0;i<subTriangles.size();i++){
            ret += subTriangles[i]->numTriangles();
        }
        return ret;
    }
    
    void update(){
        for(int i=0;i<subTriangles.size();i++){
            subTriangles[i]->update();
        }
        age += 1.0/ofGetFrameRate();
    }
    
};

class Triangles : public ContentScene {
    
public:
    
    // all scenes must draw to an fbo called fbo
    ofFbo fbo;
    bool enabled;
    
    void setup();
    void draw();
    
    void debugDraw();
    
    void update();    
    float pct;
    
    Mapping * mapping;
    ofxSyphonClient * syphon;
    
    map<InputTriangle*, SubTriangle* > subTriangles;
    
    void divide(SubTriangle * triangle);
    
    void collapse(SubTriangle * triangle);
    
    void drawTriangle(SubTriangle * triangle);
    
    ofVec2f center;
    float divideRadius;
};

