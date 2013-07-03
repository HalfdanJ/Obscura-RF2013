//
//  Triangles.cpp
//  Gloria
//
//  Created by Jonas Jongejan on 01/07/13.
//
//

#include "Triangles.h"

static float transitionTime = 8;

void Triangles::setup(){
    
    name = "Triangles";
    oscAddress = "/triangles";
    
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        
        SubTriangle * subTriangle =  new SubTriangle();
        
        for(int j=0;j<3;j++){
            subTriangle->corners[j] = new Corner();
            subTriangle->corners[j]->pos = triangle->corners[j]->pos;
        }
        
        subTriangle->normal = ofVec3f(ofRandom(-1,1),ofRandom(-1,1),1).normalized();
        
        
        //   subTriangle->corners[0]->pos.x += 20;
        subTriangles[mapping->triangles[i]] =  subTriangle ;
        
        for(int k=0;k<1;k++){
            divide(subTriangle);
        }
        
        //    cout<<"NUM "<<subTriangle->numTriangles()<<endl;
        
        //        cout<<"NUM TRIANGLES "<<subTriangles[mapping->triangles[i]].size()<<endl;
        //   collapse(mapping->triangles[i]);
        //        collapse(mapping->triangles[i]);
        //   cout<<"NUM TRIANGLES "<<subTriangles[mapping->triangles[i]].size()<<endl;
        
        center = ofVec2f(1500,500);
        divideRadius = 0;
    }
}



void Triangles::divide(SubTriangle * triangle){
    int size = triangle->subTriangles.size();
    if(size > 0){
        for(int i=0;i<size;i++){
            divide(triangle->subTriangles[i]);
        }
    } else {
        SubTriangle * subTriangle = triangle;
        
        ofVec2f center = subTriangle->center();
        ofVec2f dir = center - subTriangle->corners[0]->pos;
        
        ofVec2f newP = dir * ofRandom(0.5,1.5) + subTriangle->corners[0]->pos;
        
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->corners[0] = new Corner();
            newTriangle->corners[1] = new Corner();
            newTriangle->corners[2] = new Corner();
            
            newTriangle->corners[0]->pos = subTriangle->corners[1]->pos;
            newTriangle->corners[1]->pos = subTriangle->corners[0]->pos;
            newTriangle->corners[2]->pos = newP;
            
            ofVec2f d = (newP - newTriangle->corners[0]->pos) + (newP - newTriangle->corners[1]->pos);
            d.normalize();
            newTriangle->normal = ofVec3f(d.x,d.y,1).normalized();
            newTriangle->parentNormal = subTriangle->normal;
            
            subTriangle->subTriangles.push_back(newTriangle);
        }
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->corners[0] = new Corner();
            newTriangle->corners[1] = new Corner();
            newTriangle->corners[2] = new Corner();
            
            newTriangle->corners[0]->pos = subTriangle->corners[2]->pos;
            newTriangle->corners[1]->pos = subTriangle->corners[0]->pos;
            newTriangle->corners[2]->pos = newP;
            
            ofVec2f d = (newP - newTriangle->corners[0]->pos) + (newP - newTriangle->corners[1]->pos);
            d.normalize();
            newTriangle->normal = ofVec3f(d.x,d.y,1).normalized();
            newTriangle->parentNormal = subTriangle->normal;
            
            subTriangle->subTriangles.push_back(newTriangle);
        }
        {
            SubTriangle * newTriangle = new SubTriangle();
            newTriangle->corners[0] = new Corner();
            newTriangle->corners[1] = new Corner();
            newTriangle->corners[2] = new Corner();
            
            newTriangle->corners[0]->pos = subTriangle->corners[2]->pos;
            newTriangle->corners[1]->pos = subTriangle->corners[1]->pos;
            newTriangle->corners[2]->pos = newP;
            
            ofVec2f d = (newP - newTriangle->corners[0]->pos) + (newP - newTriangle->corners[1]->pos);
            newTriangle->normal = ofVec3f(d.x,d.y,100).normalized();
            newTriangle->parentNormal = subTriangle->normal;
            
            subTriangle->subTriangles.push_back(newTriangle);
        }
        
    }
    
}

void Triangles::collapse(SubTriangle * triangle){
    int subsubtriangles = -1;
    for(int i=0;i<triangle->subTriangles.size();i++){
        if(triangle->subTriangles[i]->subTriangles.size()){
            subsubtriangles = i;
            break;
        }
    }
    
    if(subsubtriangles != -1){
        collapse(triangle->subTriangles[subsubtriangles]);
    } else {
        float highestAge = 0;
        for(int i=0;i<triangle->subTriangles.size();i++){
            if(triangle->subTriangles[i]->age > transitionTime){
                triangle->subTriangles[i]->age = transitionTime;
            }
            if(triangle->subTriangles[i]->age > 0){
                triangle->subTriangles[i]->age -= 2 * 1.0/ofGetFrameRate();
            }
            
            if(highestAge < triangle->age){
                highestAge = triangle->age;
            }
        }
        
        
        if(highestAge <= 0){
            for(int i=0;i<triangle->subTriangles.size();i++){
                
                if(triangle->subTriangles[i]->age <= 0){
                    delete triangle->subTriangles[i];
                }
            }
            triangle->subTriangles.clear();
        }
      
        //
    }
    
    
    
    /*  int start = subTriangles[triangle].size()-3;
     
     subTriangles[triangle][start]->corners[0]->pos = subTriangles[triangle][start+1]->corners[1]->pos;
     
     // delete subTriangles[triangle][start+2];
     subTriangles[triangle].erase( subTriangles[triangle].begin() + start+2);
     
     // delete subTriangles[triangle][start+1];
     subTriangles[triangle].erase( subTriangles[triangle].begin() + start+1);
     */
}


float ease(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
};

void Triangles::drawTriangle(SubTriangle * triangle){
    if(triangle->subTriangles.size() > 0){
        for(int j=0;j<triangle->subTriangles.size();j++){
            drawTriangle(triangle->subTriangles[j]);
        }
    } else {
        ofVec2f center = triangle->center();
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        
        ofVec3f lightPos = ofVec3f(1500+1000*sin(ofGetElapsedTimeMillis()/5000.),200,-1000);
        ofVec3f trianglePos = ofVec3f(triangle->center().x, triangle->center().y, 0);
        ofVec3f lightDir = ( trianglePos- lightPos);
        
        
        ofVec3f ambient = ofVec3f(0);
        
        float aaa = MIN(1,MAX(0,(transitionTime-triangle->age)/transitionTime));
        //aaa = ease(aaa,0,1,1);
        
        //float aaa =( sin(ofGetElapsedTimeMillis()/2000.)+1)*0.5;
        
        ofVec3f normal = (1-aaa) * triangle->normal + (aaa)*triangle->parentNormal;
        if(triangle->parentNormal.length() == 0){
            normal = triangle->normal;
        }
        
        normal.normalize();
        
        float angle = lightDir.angle(normal);
        //if(angle < 90){
        //            cout<<angle<<endl;
        
        //          cout<<triangle->normal.x<<"  "<<triangle->normal.y<<"  "<<triangle->normal.z<<endl;
        
        float dist = lightDir.length();
        float intensity = 10000000* 1.0/(4*PI*dist*dist);
        
        ofVec3f color = intensity*ofVec3f(255)*fabs(90-angle)/90.0;
        
        ofSetColor(MAX(ambient.x, color.x), MAX(ambient.y, color.y), MAX(ambient.z, color.z));
        
        /*   } else {
         ofSetColor(ambient.x,ambient.y,ambient.z);
         }
         */
        glBegin(GL_TRIANGLES);
        for(int u=0;u<3;u++){
            glTexCoord2d(center.x/3, 500-center.y/2);
            glVertex2d(triangle->corners[u]->pos.x, triangle->corners[u]->pos.y);
        }
        glEnd();
        
        
        
        
        /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         ofSetColor(0, 0, 0, 50);
         glBegin(GL_TRIANGLES);
         for(int u=0;u<3;u++){
         glVertex2d(triangle->corners[u]->pos.x, triangle->corners[u]->pos.y);
         }
         glEnd();*/
        
        
    }
    
    
    
    
}

void Triangles::draw(){
    
    ofSetColor(255,255,255);
    // syphon->bind();
    
    ofSetLineWidth(1);
    
    for(int i=0;i<mapping->triangles.size();i++){
        drawTriangle(subTriangles[mapping->triangles[i]]);
    }
    
    
    // syphon->unbind();
    
    
    ofSetColor(255, 255, 255, 10);
    ofCircle(center.x, center.y, divideRadius);
    
}

void Triangles::update(){
    
    center.x = 1200*(sin(ofGetElapsedTimeMillis()/5000.)+1);
    divideRadius = 500*(cos(PI+ofGetElapsedTimeMillis()/2000.)+1);
    
    for(int i=0;i<mapping->triangles.size();i++){
        SubTriangle * triangle = subTriangles[mapping->triangles[i]];
        triangle->update();
        
        if(triangle->center().distance(center) < divideRadius){
            float a = (divideRadius - triangle->center().distance(center)) / divideRadius;
            if(triangle->numTriangles() < 9 && triangle->getLowestAge() > transitionTime){
                divide(triangle);
            }
        } else {
            if(triangle->numTriangles() > 1){
                collapse(triangle);
            }
        }
        
    }
}