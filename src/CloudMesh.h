#include "ofMain.h"

class CloudMesh : public ofNode {
	ofVec3f* nextVerts;
	ofMesh meshBuffer[5];
	ofNode xform;
	void populateMesh(ofMesh* useMesh, int planeNum);
	void triangulateMesh(ofMesh* useMesh);
	void moveClosest(ofMesh* fromMesh, ofMesh* toMesh, float t);
	void moveNoiseField(ofMesh* useMesh);
	void customDraw();
	float CloudExp(float v);
	float cloudCover;
public:
	CloudMesh()  {};
	~CloudMesh() {};

	void setup();
	void update();
	
	float setCoverage(float c);
	float getCoverage(float c);
};