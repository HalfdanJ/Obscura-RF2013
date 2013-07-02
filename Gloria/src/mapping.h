//
//  mapping.h
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#ifndef gloriamap_mapping_h
#define gloriamap_mapping_h

#include "ofMain.h"
#include "ofxSvg.h"


struct InputTriangle;

struct WorldPoint {
    ofVec3f pos;
};

struct Corner {

    vector<InputTriangle*> triangles;

    vector<Corner*> joinedCorners;

    ofVec2f pos;
    ofVec3f worldPos;


    void addTriangleReference(InputTriangle* triangle) {

        bool found = false;
        for(int i=0;i<triangles.size();i++) {
            if(triangle == triangles[i]) {
            found = true;
            }
        }

        if(!found) {
            triangles.push_back(triangle);
        }

    }
};


struct InputTriangle {

    int index;
    void debugDraw();
    ofPoint centroid;
    ofPolyline polyline;

    Corner * corners[3];

    ofMesh mesh;

    ofColor color;

};

class Mapping {
public:
    ofxSVG svg;
    vector<InputTriangle*> triangles;
    vector<Corner*> corners;

    vector<WorldPoint*> worldPoints;

};


#endif
