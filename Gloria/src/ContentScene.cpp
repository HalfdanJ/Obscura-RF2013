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
    
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
    
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
    gui->addToggle("/syphonout/x", &solo);
    
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
                
                //cout<<rest<<endl;
                
                if(widget->getKind() == OFX_UI_WIDGET_SLIDER_H || widget->getKind() == OFX_UI_WIDGET_SLIDER_V) {
                    
                    ofxUISlider *slider = (ofxUISlider *) widget;
                    
                    slider->setValue(ofMap(m->getArgAsFloat(0), 0, 1, slider->getMin(), slider->getMax()));
                } else if(widget->getKind() == OFX_UI_WIDGET_INTSLIDER_H || widget->getKind() == OFX_UI_WIDGET_INTSLIDER_V) {
                    
                    ofxUISlider *slider = (ofxUISlider *) widget;
                    slider->setValue(ofMap(m->getArgAsInt32(0), 0, 1, slider->getMin(), slider->getMax()));
                    
                } else if(widget->getKind() == OFX_UI_WIDGET_TOGGLE) {
                    
                    ofxUIToggle *toggle = (ofxUIToggle *) widget;
                    toggle->setValue(m->getArgAsInt32(0));
                    
                }
            }
            
        }
    }
}

void ContentScene::updateScene() {
    if(enabled) {
        update();
    }
}

void ContentScene::drawScene() {
    if(enabled) {
        ofPushMatrix();
        ofPushStyle();
        fbo.begin();
        draw();
        fbo.end();
        ofPopStyle();
        ofPopMatrix();
    }
}

void ContentScene::publishSyphonTexture(bool _force) {
    if ( (solo && enabled) || _force ) {
        syphonOut.publishTexture(&fbo.getTextureReference());
    }
}


void ContentScene::exit() {
    delete gui;
}