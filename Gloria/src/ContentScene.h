#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "mapping.h"
#include "ofxSyphon.h"

class ContentScene {
    
public:
    
    int index;
    string name;
    string oscAddress = "/default";
    
    float minSpeed = 0;
    float maxSpeed = 1;
    
    // bool syphonDirect;

    Mapping * mapping;

    ofFbo fbo;
    bool enabled;
    bool solo = true;
    float opacity;
    float speed;
    
    ofxSyphonServer syphonOut;
    
    int width;
    int height;
    
    ContentScene() {
    }
    
    virtual ~ContentScene(){}
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
        
    virtual void setGui(ofxUICanvas * gui, float width){
        
        string i = "["+ ofToString(index) + "] ";
        
        gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
        gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
        
        gui->addSpacer(width, 1);
        gui->addSlider(i+"opacity", 0., 1., &opacity);
        
        gui->addSpacer(width, 1);
        gui->addSlider(i+"speed", minSpeed, maxSpeed, &speed);
        
        gui->addToggle(i+"Enabled", &enabled);
        gui->addToggle(i+"Solo", &solo);
        
    }
    
    void checkMsg() {
        
    }

    virtual void parseOscMessage(ofxOscMessage * m){
        
	vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
	string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
	//cout<<adrSplit[1]<<"   "<<rest<<endl;
        
	if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {

	    if( rest == "/opacity/x" ) {
            opacity = m->getArgAsFloat(0);
	    } else if(rest == "/enable/x" ) {
            enabled = m->getArgAsInt32(0);
	    } else if(rest == "/speed/x" ) {
            speed =m->getArgAsFloat(0);
	    } 

        }
        
    }
    
    void setupScene(int width, int height, int i) {
        index = i;
        name = "Scene" + ofToString(i);
        fbo.allocate(width, height);
        setup();
        syphonOut.setName(name);
        
    }
    
    void updateScene() {
        if(enabled) {
            update();
        }
    }
    
    void drawScene() {
        if(enabled) {
            fbo.begin();
            ofClear(0, 0);
            
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            draw();
            
            //glDisable(GL_BLEND);
            
            fbo.end();
        }
        
        if (solo) {
            syphonOut.publishTexture(&fbo.getTextureReference());
        }
    }
    
};
