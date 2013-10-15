#include "CloudMesh.h"

void CloudMesh::setup()
{
	cloudCover = 0.5;
	populateMesh(&meshBuffer[0], 0.0f);
	triangulateMesh(&meshBuffer[0]);
	
}

float CloudMesh::CloudExp(float v)
{
	float c = v - cloudCover;
	if (c < 0) {
		c = 0;	
	}
	float CloudDensity = 1.0 - pow(0.7, c);
	return  CloudDensity;
}

void CloudMesh::update()
{
	moveNoiseField(&meshBuffer[0]);
}

void CloudMesh::customDraw()
{
	meshBuffer[0].draw(ofPolyRenderMode::OF_MESH_FILL);
	ofSetColor(255);
	for(int i = 0; i < meshBuffer[0].getNumVertices(); i++)
	{
		ofDrawBitmapString(ofToString(i), meshBuffer[0].getVertex(i));
	}
}


void CloudMesh::populateMesh(ofMesh* useMesh, float z)
{
	useMesh->clearVertices();
	for(int i = 0; i < 200; i++)
	{
		bool selected = false;
		while (selected == false)
		{
			ofPoint p(ofRandomf(), ofRandomf(), -1.0f);
			float noise = ofNoise(p.x, p.y, z);
			if(ofRandomuf() < CloudExp(noise))
			{
				selected = true;
				useMesh->addVertex(ofVec3f(p.x * ofGetWidth()/2.0f, p.y * ofGetWidth()/2.0f));
			}
		}
	}
}

void CloudMesh::triangulateMesh(ofMesh* useMesh)
{
	useMesh->clearIndices();
	for(int i = 0; i < useMesh->getNumVertices(); i++)
	{
		ofVec3f currentVert = useMesh->getVertex(i);
		float first = 999999, second = 999999;
		int firstIndex = -1 , secondIndex = -1;
		for(int j = 0; j < useMesh->getNumVertices(); j++)
		{
			float sDist = 0;
			if(currentVert == useMesh->getVertex(j))
			{
				continue;
			}
			if((sDist = currentVert.distanceSquared(useMesh->getVertex(j))) < first && sDist > 1000 && sDist <= 8000)
			{
				first = sDist;
				firstIndex = j;
			}
			else if((sDist = currentVert.distanceSquared(useMesh->getVertex(j))) < second && sDist > 1000 && sDist <= 8000)
			{
				second = sDist;
				secondIndex = j;
			}
		}
		if(firstIndex >= 0 && secondIndex >= 0)
		{
			useMesh->addTriangle(i,firstIndex, secondIndex);
		}
		
	}
}

void CloudMesh::moveClosest(ofMesh* fromMesh, ofMesh* toMesh, float t)
{
	for(int i = 0; i < fromMesh->getNumVertices(); i++)
	{
		ofVec3f currentVert = fromMesh->getVertex(i);
		float first = 999999;
		int Index = -1;
		for(int j = 0; j < toMesh->getNumVertices(); j++)
		{
			float sDist = 0;
			if((sDist = currentVert.distanceSquared(toMesh->getVertex(j))) < first && sDist > 1000)
			{
				first = sDist;
				Index = j;
			}
		}
		*(fromMesh->getVerticesPointer() + i) += ((toMesh->getVertex(Index) - *(fromMesh->getVerticesPointer() + i)) * t);
	}
}

void CloudMesh::moveNoiseField(ofMesh* useMesh){
	float movDist = 1.0f;
	for(int i = 0; i < useMesh->getNumVertices(); i++)
	{
		ofVec3f currentVert = useMesh->getVertex(i);

		// radians                                                          1.5 * 2.0
		float angle = (ofNoise(currentVert.x, currentVert.y, currentVert.z)) * 3.0f * PI;
		useMesh->getVerticesPointer()[i] = currentVert + (ofVec3f(cos(angle), sin(angle), 0.0f).normalized());
	}
}