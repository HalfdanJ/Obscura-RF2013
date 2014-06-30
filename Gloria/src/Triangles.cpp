//
//  Triangles.cpp
//  Gloria
//
//  Created by Jonas Jongejan on 01/07/13.
//
//

#include "Triangles.h"


void Triangles::setGui(){
    gui->addSlider("SyphonOpacity", 0,1, &syphonOpacity);
    gui->addSlider("DivideTriangleSize", 10000,6000, &divideTriangleSize);
    gui->addSlider("DivideRadius", 0,2400, &divideRadius);
    gui->addToggle("DivideInvert", &divideInvert);
    gui->addSlider("TransitionTime", 0,10, &transitionTime);
    
    
    
    gui->addSlider("Light", 0,1, &light);
    gui->addSlider("LightSpeed", 0,1, &lightSpeed);
    
    gui->addSlider("Direct Opactiry", 0,1, &directTextureOpacity);
    
    gui->addSlider("ColorR", 0,1, &colorR);
    gui->addSlider("ColorG", 0,1, &colorG);
    gui->addSlider("ColorB", 0,1, &colorB);
}


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
        
        subTriangle->parentTriangle = subTriangle;
        subTriangle->age = 1000;
        
        subTriangles[mapping->triangles[i]] =  subTriangle ;
        
        center = ofVec2f(1500,500);
        divideRadius = 0;
    }
    
    debugShader.load("shaders/TrianglesDebug");
}




void Triangles::divide(SubTriangle * triangle, float sizeGoal){
    int size = triangle->subTriangles.size();
    if(sizeGoal > 0){
        cout<<triangle->triangleSize()<<"  "<<sizeGoal<<endl;
        if(size > 0){
            for(int i=0;i<size;i++){
                divide(triangle->subTriangles[i], sizeGoal);
            }
        } else if(triangle->triangleSize() > sizeGoal){
            cout<<"ASKDPO"<<endl;
            SubTriangle * subTriangle = triangle;
        
            ofVec3f center = subTriangle->getCenter();
            ofVec3f dir = center - subTriangle->corners[0]->pos;
            
            ofVec3f newP = dir * ofRandom(0.5,1.5) + subTriangle->corners[0]->pos;
            newP.z += ofRandom(-dir.length(), dir.length());

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
                newTriangle->parentTriangle = subTriangle;
                
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
                newTriangle->parentTriangle = subTriangle;
                
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
                newTriangle->parentTriangle = subTriangle;
                
                subTriangle->subTriangles.push_back(newTriangle);
            }
            
        }
    }
    
}

void Triangles::collapse(SubTriangle * triangle){
    int subsubtriangles = -1;
    for(int i=0;i<triangle->subTriangles.size();i++){
        if(triangle->subTriangles[i]->subTriangles.size()){
            collapse(triangle->subTriangles[i]);
            subsubtriangles = i;
            //      break;
        }
    }
    
    if(subsubtriangles != -1){
    } else {
        float highestAge = 0;
        for(int i=0;i<triangle->subTriangles.size();i++){
            if(triangle->subTriangles[i]->age > transitionTime){
                triangle->subTriangles[i]->age = transitionTime;
            }
            if(triangle->subTriangles[i]->age > 0){
                triangle->subTriangles[i]->age -= triangle->ageDifference* 2 * 1.0/ofGetFrameRate();
            }
            
            if(highestAge < triangle->subTriangles[i]->age){
                highestAge = triangle->subTriangles[i]->age;
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
    }
}


float ease(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
};

void Triangles::drawTriangle(SubTriangle * triangle, float opacity){
    if(triangle->subTriangles.size() > 0){
        for(int j=0;j<triangle->subTriangles.size();j++){
            drawTriangle(triangle->subTriangles[j], opacity);
        }
    } else {
        
        ofVec3f ambient = ofVec3f(10);
        
        
        float aaa = MIN(1,MAX(0,(transitionTime-triangle->age)/transitionTime));
        
        
        ofVec3f center;
        ofVec3f normal;
        
//        if(aaa == 0){
            center = triangle->getCenter();
            normal = triangle->normal();
  /*      } else {
            center = (1-aaa)*triangle->getCenter() + aaa* triangle->parentTriangle->getCenter();
            normal = (1-aaa) * triangle->normal + (aaa)*triangle->parentTriangle->normal;
        }*/
        
     
        ofVec3f trianglePos = center;
        ofVec3f lightDir = ( trianglePos- lightPos);

        
        normal.normalize();
        
        float angle = lightDir.angle(normal);
        angle = MIN(80,angle);

        
        float dist = lightDir.length();
        float intensity = 100000000* 1.0/(4*PI*dist*dist);
        intensity = 1.0;
        
        angle *= light;
        
        ofVec3f color = intensity*ofVec3f(colorR*255,colorG*255,colorB*255)*fabs(90-angle)/90.0;
        
        
        ofSetColor(MAX(ambient.x, color.x), MAX(ambient.y, color.y), MAX(ambient.z, color.z), 255*opacity);
        
        //Tegn billede 1 pixel pr trekant
        ofVec3f _normal = triangle->normal();
        for(int u=0;u<3;u++){
            glNormal3f(_normal.x, _normal.y, _normal.z);
            
            glTexCoord2d(syphonIn->getWidth()* center.x/OUTWIDTH
                         , syphonIn->getHeight()*(OUTHEIGHT-center.y)/OUTHEIGHT);
            glVertex3d(triangle->corners[u]->pos.x, triangle->corners[u]->pos.y, triangle->corners[u]->pos.z);
        }
        

        
        
        //Tegn billede 1:1
        float bbb = directTextureOpacity;
        if(bbb){
            
            ofSetColor(255,255,255,255*bbb);
            for(int u=0;u<3;u++){
                glNormal3f(_normal.x, _normal.y, _normal.z);

                glTexCoord2d(syphonIn->getWidth()* triangle->corners[u]->pos.x/OUTWIDTH
                             ,syphonIn->getHeight()*(OUTHEIGHT-triangle->corners[u]->pos.y)/OUTHEIGHT);
                
                glVertex3d(triangle->corners[u]->pos.x, triangle->corners[u]->pos.y, triangle->corners[u]->pos.z);
            }
        }
    }
}

void Triangles::draw(){
    
    ofSetColor(255,255,255);
    
    debugShader.begin();
    
    glBegin(GL_TRIANGLES);
    for(int i=0;i<mapping->triangles.size();i++){
        drawTriangle(subTriangles[mapping->triangles[i]],1);
    }
    glEnd();
    
    
    if(syphonOpacity){
        syphonIn->bind();
        glBegin(GL_TRIANGLES);
        for(int i=0;i<mapping->triangles.size();i++){
            drawTriangle(subTriangles[mapping->triangles[i]],syphonOpacity);
        }
        
        glEnd();
        syphonIn->unbind();
    }
    
    debugShader.end();
}

void Triangles::update(){
    for(int i=0;i<mapping->triangles.size();i++){
        SubTriangle * triangle = subTriangles[mapping->triangles[i]];
        triangle->update();
    }
    
    
    _lightPhase += lightSpeed * 1.0 / MAX(10, MIN(ofGetFrameRate(),60));
    lightPos = ofVec3f(2996+1500*sin(_lightPhase),200,-3000);
    
    center.x = 2996;
    center.y = 1200;
    
    
    for(int i=0;i<mapping->triangles.size();i++){
        SubTriangle * triangle = subTriangles[mapping->triangles[i]];
        
        float sizeGoal = 100000;// divideTriangleSize;//*triangle->ageDifference;
        
        if(triangle->getSmallestSize() > sizeGoal
           && ((triangle->getCenter().distance(center) < divideRadius && !divideInvert) || (triangle->getCenter().distance(center) > divideRadius && divideInvert))){
            float a = (divideRadius - triangle->getCenter().distance(center)) / divideRadius;
            if(triangle->getLowestAge() > transitionTime){
                divide(triangle, sizeGoal);
//                cout<<"divide"<<endl;
            }
        } else {
            if(triangle->numTriangles() > 1){
                collapse(triangle);
               // cout<<"Collapse"<<endl;
            }
        }
        
    }
}



void Triangles::parseOscMessage(ofxOscMessage *m){
    ContentScene::parseOscMessage(m);
    
    
 	vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
	string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
    
	if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
        if( rest == "/syphonopacity/x" ) {
            syphonOpacity = m->getArgAsFloat(0);
	    }
/*        if( rest == "/dividecount/x" ) {
            divideCount = m->getArgAsFloat(0) * 6;
	    }*/
        if( rest == "/divideradius/x" ) {
            divideRadius = m->getArgAsFloat(0) * 2400;
	    }
        if( rest == "/divideinvert/x" ) {
            divideInvert = m->getArgAsFloat(0);
	    }
        if( rest == "/transitiontime/x" ) {
            transitionTime = m->getArgAsFloat(0)*5;
	    }
        if( rest == "/light/x" ) {
            light = m->getArgAsFloat(0);
	    }
        
        if( rest == "/lightspeed/x" ) {
            lightSpeed = m->getArgAsFloat(0);
	    }
        if( rest == "/directopacity/x" ) {
            directTextureOpacity = m->getArgAsFloat(0);
	    }
        if( rest == "/triangleColorR/x" ) {
            colorR = m->getArgAsFloat(0);
	    }
        if( rest == "/triangleColorG/x" ) {
            colorG = m->getArgAsFloat(0);
	    }
        if( rest == "/triangleColorB/x" ) {
            colorB = m->getArgAsFloat(0);
	    }
        
    }
    
}