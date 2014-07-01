//
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "ofxFlowTools.h"
//#include "ofxFluid.h"

using namespace flowTools;

class Transformer : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    float frequency;
    //float lineWidth;
    bool crazySpeed;
    bool reverse;
    
    float	lastTime;
	float   deltaTime;
    
    ftFluidSimulation	fluid;
    
    float flowWidth;
    float flowHeight;
    float drawWidth;
    float drawHeight;
    
    bool showLogo;

    ftOpticalFlow		opticalFlow;
	ftVelocityMask		velocityMask;
	ftParticleFlow		particleFlow;
	
	ftDisplayScalar		displayScalar;
	ftVelocityField		velocityField;
	ftTemperatureField	temperatureField;

    ofFbo inputFbo;
    
};
