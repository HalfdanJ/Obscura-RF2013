//
//  ContentScene.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 27/06/14.
//
//

#include "ContentScene.h"

void ContentScene::init() {
}
void ContentScene::setup(){
}
void ContentScene::update(){
}
void ContentScene::draw() {
}
void ContentScene::setGui() {
}

void ContentScene::parseOscMessage(ofxOscMessage *m) {
}

void ContentScene::setupScene(int _width, int _height, int _i) {
    index = _i;
    name = "Scene" + ofToString(_i);
    
    ofFbo::Settings settings;
    
    height = _height;
    width = _width;
    
    settings.height = _height;
    settings.width = _width;
    settings.numSamples = numSamples;
    settings.useDepth = false;
    //settings.colorFormats = GL_RGBA;
    
    fbo.allocate(settings);
    
    setup();
    syphonOut.setName(name);
}


void ContentScene::setSceneGui(){
    
    gui = new ofxUICanvas();
    
    string i = "["+ ofToString(index) + "] ";
    gui->setName(name);
    
    gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
    
    gui->addSpacer(GUIWIDTH, 1);
    gui->addSlider("/opacity/x", 0., 1., &opacity);
    
    gui->addSpacer(GUIWIDTH, 1);
    //gui->addSlider(i+"speed", minSpeed, maxSpeed, &speed);
    gui->addToggle("/enabled/x", &enabled);
    gui->addToggle("/SyphonOut/x", &solo);
    
    setGui();
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent,this,&ContentScene::guiEvent);
}

void ContentScene::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    string canvasParent = e.widget->getCanvasParent()->getName();
    //cout << canvasParent << endl;
}

void ContentScene::parseSceneOscMessage(ofxOscMessage * m){

    // you can change values of widgets in gui with osc just send to its name
    
	vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
	string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
	//cout<<adrSplit[1]<<"   "<<rest<<endl;
    
    if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
        
        for(int i=0; i< gui->getWidgets().size(); i++ ) {
        
        ofxUIWidget * widget = gui->getWidgets()[i];
        
        if(widget->getName() == rest) {
            if(widget->getKind() == OFX_UI_WIDGET_SLIDER_H) {
                
                    ofxUISlider *slider = (ofxUISlider *) widget;
                    slider->setValue(m->getArgAsFloat(0));
                
                }
            }
        }
    
        /*if( rest == "/opacity/x" ) {
            opacity = m->getArgAsFloat(0);
        } else if(rest == "/enable/x" ) {
            enabled = m->getArgAsInt32(0);
        }/* else if(rest == "/speed/x" ) {
            speed =m->getArgAsFloat(0);
        }*/
    }
}

void ContentScene::updateScene() {
    if(enabled) {
        update();
    }
    
    //time += speed;
}

void ContentScene::drawScene() {
    if(enabled) {
        ofPushMatrix();
        ofPushStyle();
        
        fbo.begin();
        ofClear(0, 0, 0);
        ofClearAlpha();
        
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        draw();
        //glDisable(GL_BLEND);
        
        fbo.end();
        ofPopStyle();
        ofPopMatrix();
    }
    
    if (solo && enabled) {
        syphonOut.publishTexture(&fbo.getTextureReference());
    }
}

void ContentScene::exit() {
    delete gui;
    delete syphonIn;
    delete mapping;
}