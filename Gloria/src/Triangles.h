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
    SubTriangle * parentTriangle;
    bool die;
    
    float ageDifference;
    
    SubTriangle(){
        ageDifference = ofRandom(0.9,1.0);
        age = 0;
    }
    
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
        age += ageDifference*1.0/ofGetFrameRate();
    }
    
    float getLowestAge(){
        float _age = age;
        for(int i=0;i<subTriangles.size();i++){
            if(_age > subTriangles[i]->getLowestAge()){
                _age = subTriangles[i]->getLowestAge();
            }
        }
        return _age;
    }
    
};

class Triangles : public ContentScene {
    
public:
    
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

