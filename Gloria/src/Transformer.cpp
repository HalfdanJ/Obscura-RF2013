//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "Transformer.h"

void Transformer::setup(){
    
    name = "Transformer";
    oscAddress = "/plw";
    
    drawWidth = OUTWIDTH/12;
	drawHeight = OUTHEIGHT/12;
	// process all but the density on 16th resolution
	flowWidth = drawWidth/6;
	flowHeight = drawHeight/6;
	
    
    // Flow & Mask
	opticalFlow.setup(flowWidth, flowHeight);
	velocityMask.setup(drawWidth, drawHeight);
    
    fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight, false);
    // create a texture from our scene structure
    //fluid.addObstacle(<#ofTexture &_tex#>)
    
    // Particles
	particleFlow.setup(flowWidth, flowHeight, drawWidth, drawHeight);
	
	// Visualisation
	displayScalar.allocate(flowWidth, flowHeight);
	velocityField.allocate(flowWidth / 4, flowHeight / 4);
	temperatureField.allocate(flowWidth / 4, flowHeight / 4);
    
    inputFbo.allocate(drawWidth, drawHeight);
    
    lastTime = ofGetElapsedTimef();
    
    inputFbo.begin(); ofClear(0); inputFbo.end();

}

void Transformer::update(){
    
    deltaTime = ofGetElapsedTimef() - lastTime;
	lastTime = ofGetElapsedTimef();
	
    
}

void Transformer::draw(){;
    
    /*for(int i =0; i<mapping->triangles.size();i++) {
    }*/
    
    inputFbo.begin();
    ofClear(0,0);
    syphonIn->draw(0,0,inputFbo.getWidth(),inputFbo.getHeight());
    inputFbo.end();
    
    opticalFlow.setSource(inputFbo.getTextureReference());
    opticalFlow.update(deltaTime);
		
    velocityMask.setDensity(inputFbo.getTextureReference());
    velocityMask.setVelocity(opticalFlow.getOpticalFlow());
    velocityMask.update();
	
	fluid.addVelocity(opticalFlow.getOpticalFlowDecay());
	fluid.addDensity(velocityMask.getColorMask());
	fluid.addTemperature(velocityMask.getLuminanceMask());
    
    // add more forces
    
    
    fluid.update();
    if (particleFlow.isActive()) {
		particleFlow.setSpeed(fluid.getSpeed());
		particleFlow.setCellSize(fluid.getCellSize());
		particleFlow.addFlowVelocity(opticalFlow.getOpticalFlow());
		particleFlow.addFluidVelocity(fluid.getVelocity());
		particleFlow.setObstacle(fluid.getObstacle());
	}
	particleFlow.update();
    
 
    ofClear(0,0);
    
    ofPushStyle();
    fluid.draw(0, 0, OUTWIDTH, OUTHEIGHT);
    if (particleFlow.isActive())
        particleFlow.draw(0, 0, OUTWIDTH, OUTHEIGHT);
    if (showLogo) {
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
        ofSetColor(255,255,255,255);
        ////flowToolsLogoImage.draw(0, 0, OUTWIDTH, OUTHEIGHT);
    }
    ofPopStyle();
}

void Transformer::parseOscMessage(ofxOscMessage *m){
    
}

void Transformer::setGui(){
    
    //gui->addSlider("speed/x", -1, 1, &xSpeed);
    
}
