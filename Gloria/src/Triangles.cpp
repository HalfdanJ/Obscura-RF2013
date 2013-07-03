//
//  Triangles.cpp
//  Gloria
//
//  Created by Jonas Jongejan on 01/07/13.
//
//

#include "Triangles.h"


void Triangles::setup(){
    
    name = "Triangles";
    oscAddress = "/triangles";
    
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        
        InputTriangle * subTriangle = new InputTriangle();
        
        for(int j=0;j<3;j++){
            subTriangle->corners[j] = new Corner();
            subTriangle->corners[j]->pos = triangle->corners[j]->pos;
        }
        
     //   subTriangle->corners[0]->pos.x += 20;
        subTriangles[mapping->triangles[i]].push_back( subTriangle );
        
        for(int k=0;k<1;k++){
        divide(mapping->triangles[i]);
        }
    }
}



void Triangles::divide(InputTriangle * triangle){
    int i=ofRandom(subTriangles[triangle].size()*0.2);
    
    int size = subTriangles[triangle].size();
    for(i=0;i<size;i++){
         InputTriangle* subTriangle = subTriangles[triangle][i];
        
        ofVec2f center = subTriangle->center();
        ofVec2f dir = center - subTriangle->corners[0]->pos;
        
        ofVec2f origP = subTriangle->corners[0]->pos;
        
        subTriangle->corners[0]->pos += dir * ofRandom(0.5,1.5);
        
        InputTriangle * newTriangle = new InputTriangle();
        
        newTriangle->corners[0] = new Corner();
        newTriangle->corners[1] = new Corner();
        newTriangle->corners[2] = new Corner();
        
        newTriangle->corners[0]->pos = subTriangle->corners[0]->pos;
        newTriangle->corners[1]->pos = origP;
        newTriangle->corners[2]->pos = subTriangle->corners[1]->pos;
        
        subTriangles[triangle].push_back(newTriangle);
        
        
        newTriangle = new InputTriangle();
        
        newTriangle->corners[0] = new Corner();
        newTriangle->corners[1] = new Corner();
        newTriangle->corners[2] = new Corner();
        
        newTriangle->corners[0]->pos = subTriangle->corners[0]->pos;
        newTriangle->corners[1]->pos = origP;
        newTriangle->corners[2]->pos = subTriangle->corners[2]->pos;
        
        subTriangles[triangle].push_back(newTriangle);
    }
}

void Triangles::collapse(InputTriangle * triangle){
    int start = subTriangles[triangle].size()-3;
    
    
}

void Triangles::draw(){
    
    ofSetColor(255,255,255);
    syphon->bind();
    

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for(int i=0;i<mapping->triangles.size();i++){
        for(int j=0;j<subTriangles[mapping->triangles[i]].size();j++){
            
            InputTriangle * subTriangle = subTriangles[mapping->triangles[i]][j];
            ofVec2f center = subTriangle->center();
            
            
            glBegin(GL_TRIANGLES);
            for(int u=0;u<3;u++){
                glTexCoord2d(center.x/3, 500-center.y/2);
                glVertex2d(subTriangle->corners[u]->pos.x, subTriangle->corners[u]->pos.y);
            }
            glEnd();
            
        }

    }

    
    /*ofSetColor(0, 0, 0);
    ofSetLineWidth(1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(int i=0;i<mapping->triangles.size();i++){
        for(int j=0;j<subTriangles[mapping->triangles[i]].size();j++){
            
            InputTriangle * subTriangle = subTriangles[mapping->triangles[i]][j];
            glBegin(GL_TRIANGLES);
            for(int u=0;u<3;u++){
                glVertex2d(subTriangle->corners[u]->pos.x, subTriangle->corners[u]->pos.y);
            }
            glEnd();
            
        }
        
    }*/

    syphon->unbind();
    
    }

void Triangles::update(){
    
}