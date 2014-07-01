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
    gui->addSlider("DivideTriangleSize", 1000,60000, &divideTriangleSize);
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
    
    map<Corner*, Corner*> cornerRefs;
    
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        
        SubTriangle * subTriangle =  new SubTriangle();
        subTriangle->level = 0;
        
        //Create a duplicate of the corners
        for(int j=0;j<3;j++){
            if(cornerRefs.find(triangle->corners[j]) != cornerRefs.end()){
                subTriangle->corners[j] = cornerRefs[triangle->corners[j]];
            } else {
                subTriangle->corners[j] = new Corner();
                subTriangle->corners[j]->pos = triangle->corners[j]->pos;
                
                cornerRefs[triangle->corners[j]] = subTriangle->corners[j];

            }
        }
        
        subTriangle->parentTriangle = subTriangle;
        subTriangle->age = 1000;
        
        subTriangles[mapping->triangles[i]] =  subTriangle ;
        
        center = ofVec2f(1500,500);
        divideRadius = 0;
    }
    
    
    //Duplicate the joined corners
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        SubTriangle * subTriangle = subTriangles[triangle];
        
        for(int j=0;j<3;j++){
            for(int k=0; k<triangle->corners[j]->joinedCorners.size(); k++){
                Corner * c1 = triangle->corners[j]->joinedCorners[k];
                Corner * c2 = cornerRefs[c1];
                subTriangle->corners[j]->joinedCorners.push_back(c2);
            }
        }
    }
    
    //Create division Corners
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        SubTriangle * subTriangle = subTriangles[triangle];
        
        for(int j=0;j<3;j++){
            subTriangle->corners[j]->createDivisionCorners();
        }
        
    }
    
    //Divide
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        SubTriangle * subTriangle = subTriangles[triangle];


        subTriangle->divide();
        
    }
    
    
    //
    
    for(int it=0;it<2;it++){
        for(int i=0;i<mapping->triangles.size();i++){
            InputTriangle * triangle = mapping->triangles[i];
            SubTriangle * subTriangle = subTriangles[triangle];
            vector<SubTriangle*> arr = subTriangle->getLowestLevelTriangles();
            
            for(int j=0;j<arr.size();j++){
                SubTriangle * _subTriangle = arr[j];
                
                for(int k=0;k<3;k++){
                    _subTriangle->corners[k]->createDivisionCorners();
                }
                
                _subTriangle->divide();
                
                
            }
        }
    }

    


    debugShader.load("shaders/TrianglesDebug");
}




void Triangles::divide(SubTriangle * triangle, float sizeGoal){
    
}

void Triangles::collapse(SubTriangle * triangle){
   /* int subsubtriangles = -1;
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
    }*/
}


float ease(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
};


void Triangles::drawTriangleWireframe(SubTriangle * triangle){
    if(triangle->subTriangles.size() > 0){
        for(int j=0;j<triangle->subTriangles.size();j++){
            drawTriangleWireframe(triangle->subTriangles[j]);
        }
    } else {
        for(int u=0;u<3;u++){
            glVertex3d(triangle->corners[u]->pos.x, triangle->corners[u]->pos.y, 0/*triangle->corners[u]->pos.z*/);
        }

    }
}
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
            glVertex3d(triangle->corners[u]->pos.x, triangle->corners[u]->pos.y, triangle->corners[u]->pos.z*0.2);
        }
        

      //  cout<<triangle->corners[0]->joinedCorners.size()<<endl;
        
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
    
    ofNoFill();
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glBegin(GL_TRIANGLES);
    ofSetColor(255);
    for(int i=0;i<mapping->triangles.size();i++){
        drawTriangleWireframe(subTriangles[mapping->triangles[i]]);
    }
    glEnd();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    ofFill();
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
        
        float sizeGoal = divideTriangleSize;//*triangle->ageDifference;
        
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