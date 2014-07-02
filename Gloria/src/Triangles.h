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
    int level;
    
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
    
    vector<SubTriangle*> getLowestLevelTriangles(){
        vector<SubTriangle*> ret;
        
        if(subTriangles.size() > 0){
            for(int i=0;i<subTriangles.size();i++){
                vector<SubTriangle*> sub = subTriangles[i]->getLowestLevelTriangles();
                ret.insert(ret.end(), sub.begin(), sub.end());
            }
        } else {
            ret.push_back(this);
        }
        
        return ret;
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
    
    void divide(){
        //Division corners
//        cout<<"Div "<<corners[1]<<"  "<<corners[2]<<"  "<<corners[1]->division[corners[2]]<<endl;
        Corner * d1 = corners[0]->division[corners[1]];
        Corner * d2 = corners[0]->division[corners[2]];
        Corner * d3 = corners[1]->division[corners[2]];
        

        //Z
        d1->pos.z = ofRandom(-100,100);
        d2->pos.z = ofRandom(-100,100);
        d3->pos.z = ofRandom(-100,100);
        
        //Create the 4 new triangles
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->level = level+1;
            newTriangle->corners[0] = corners[0];
            newTriangle->corners[1] = corners[0]->division[corners[1]];
            newTriangle->corners[2] = corners[0]->division[corners[2]];
            
            newTriangle->parentTriangle = this;
            
            
            
            subTriangles.push_back(newTriangle);
        }
        
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->level = level+1;
            newTriangle->corners[0] = corners[1];
            newTriangle->corners[1] = corners[1]->division[corners[2]];
            newTriangle->corners[2] = corners[1]->division[corners[0]];
            
            newTriangle->parentTriangle = this;
            
            
            
            subTriangles.push_back(newTriangle);
        }
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->level = level+1;
            newTriangle->corners[0] = corners[2];
            newTriangle->corners[1] = corners[2]->division[corners[0]];
            newTriangle->corners[2] = corners[2]->division[corners[1]];
            
            newTriangle->parentTriangle = this;
            
            
            
            subTriangles.push_back(newTriangle);
        }
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->level = level+1;
            newTriangle->corners[0] = corners[1]->division[corners[2]];
            newTriangle->corners[1] = corners[0]->division[corners[1]];
            newTriangle->corners[2] = corners[0]->division[corners[2]];
            
            newTriangle->parentTriangle = this;
            
            
            
            subTriangles.push_back(newTriangle);
        }
        
        
        //Update joined corners
        for(int i=0;i<3;i++){
            for(int j=0;j<2;j++){
                corners[i]->joinedCorners.erase(std::remove(corners[i]->joinedCorners.begin(),
                                                            corners[i]->joinedCorners.end(),
                                                            corners[(int)ofWrap(j+i+1,0,3)]),
                                                corners[i]->joinedCorners.end());
            }
            
            corners[i]->joinedCorners.push_back(corners[i]->division[corners[(int)ofWrap(i+1,0,3)]]);
            corners[i]->joinedCorners.push_back(corners[i]->division[corners[(int)ofWrap(i+2,0,3)]]);
        }
        
        d1->joinedCorners.push_back(d2);
        d1->joinedCorners.push_back(d3);

        d2->joinedCorners.push_back(d3);
        d2->joinedCorners.push_back(d1);
        
        d3->joinedCorners.push_back(d1);
        d3->joinedCorners.push_back(d2);
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
    void drawTriangleWireframe(SubTriangle * triangle);
    
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

