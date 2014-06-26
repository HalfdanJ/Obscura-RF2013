//
//  LampWalker.cpp
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#include "LampWalker.h"

void LampWalker::setup(){
    name = "Lamp Walker";
    oscAddress = "/lampwalker";
}

void LampWalker::update(){
    if(!src){
        src = mapping->corners[ofRandom(mapping->corners.size())];
    }

    if(!dst){
        dst = src->joinedCorners[ofRandom(src->joinedCorners.size())];
    }

    pct += 0.01;
    if(pct > 1){
	pct = 0;
	lastSrc = src;
	src = dst;
      //  cout<<src->joinedCorners.size()<<endl;
	while (src == dst || lastSrc == dst){
	    int i=roundf(ofRandom(src->joinedCorners.size()-1));
	    dst = src->joinedCorners[i];
	   // cout<<dst<<"  "<<src<<endl;
	}
    }

}

void LampWalker::draw(){;
	ofVec2f srcPt =src->pos;
	ofVec2f dstPt = dst->pos;

	ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;

	ofSetColor(255, 0, 0);
    ofFill();
	ofCircle(interpolate.x, interpolate.y, 10);
}
