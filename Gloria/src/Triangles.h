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
#include "ofxAutoReloadedShader.h"

class SubTriangle : public InputTriangle{
public:
    vector<SubTriangle*> subTriangles;
    float age;
    SubTriangle * parentTriangle;
    bool die;
    
    float ageDifference;
    
    SubTriangle(){
        ageDifference = ofRandom(0.5,1.0);
        age = 0;
    }
    
    ofVec3f normal(){
        ofVec3f n = (corners[1]->pos - corners[0]->pos).cross(( corners[2]->pos - corners[0]->pos )).normalized();
        if(n.z < 0){
            n *= -1.;
        }
        return  n;
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
    
    int getLevels(){
        int ret = 1;
        for(int i=subTriangles.size()-1;i>=0;i--){
            if(ret < 1+subTriangles[i]->getLevels()){
                ret = 1+subTriangles[i]->getLevels();
            }
        }
        return ret;
    }
    
    float triangleSize(){
        float a = (corners[1]->pos - corners[0]->pos).length();
        float b = (corners[2]->pos - corners[0]->pos).length();
        float c = (corners[2]->pos - corners[1]->pos).length();
        
        float s = (a+b+c)/2.0;
        return sqrt(s*(s-a)*(s-b)*(s-c));
//       return  0.5 * (corners[1]->pos - corners[0]->pos).length() * (corners[2]->pos - corners[0]->pos).length();
    }
    
    float getSmallestSize(){
        float _size = triangleSize();
        for(int i=0;i<subTriangles.size();i++){
            if(_size > subTriangles[i]->triangleSize()){
                _size = subTriangles[i]->triangleSize();
            }
        }
        return _size;
    }
    
};

class Triangles : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();    
    float pct;
    
    map<InputTriangle*, SubTriangle* > subTriangles;
    
    void divide(SubTriangle * triangle, float sizeGoal);
    
    void collapse(SubTriangle * triangle);
    
    void drawTriangle(SubTriangle * triangle, float opacity);
    
    ofVec2f center;
    
    float syphonOpacity;
    float directTextureOpacity;

    float transitionTime;
    float divideTriangleSize;
    float divideRadius;
    bool divideInvert;
    
    float light;
    float lightSpeed;
    ofVec3f lightPos;
    
    float colorR, colorG, colorB;
    
    ofxAutoReloadedShader debugShader;
    
    void setGui();
    void parseOscMessage(ofxOscMessage *m);
    
    
private:
    
    float _lightPhase;
};

