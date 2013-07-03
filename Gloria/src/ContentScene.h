#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"

class ContentScene {
    
public:
    
    int index;
    string name = "unnamed scene";
    string oscAddress = "/default";

    // bool syphonDirect;

    ofFbo fbo;
    bool enabled;
    float opacity;
    float speed;
    
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
        
        gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
        gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
        gui->addSpacer(width, 1);
        
        gui->addSpacer(width, 1);
        gui->addSlider("opacity"+name, 0., 1., &opacity);
        
        gui->addSpacer(width, 1);
        gui->addSlider("speed"+name, 0., 1., &speed);
        
        gui->addToggle("Enabled"+name, &enabled);
        
        //gui->
        
    }
    
    virtual void oscMessage(ofxOscMessage * m){
        
    }
    
    void setupScene(int width, int height) {
        fbo.allocate(width, height);
        setup();
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
    }
    
};
