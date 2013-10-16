#include "CloudMesh.h"

void CloudMesh::setup()
{
	cloudCover = 0.5;
	populateMesh(&meshBuffer[0], 0);
	triangulateMesh(&meshBuffer[0]);

	populateMesh(&meshBuffer[1], 1);
	triangulateMesh(&meshBuffer[1]);

	populateMesh(&meshBuffer[2], 2);
	triangulateMesh(&meshBuffer[2]);
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
	moveNoiseField(&meshBuffer[1]);
	moveNoiseField(&meshBuffer[2]);
}

void CloudMesh::customDraw()
{
	meshBuffer[0].draw(OF_MESH_FILL);
	meshBuffer[1].draw(OF_MESH_FILL);
	meshBuffer[2].draw(OF_MESH_FILL);
	ofSetColor(255);
	for(int i = 0; i < meshBuffer[0].getNumVertices(); i++)
	{
		ofSetColor(80);
		//ofDrawBitmapString(ofToString(i), meshBuffer[0].getVertex(i));
	}
}


void CloudMesh::populateMesh(ofMesh* useMesh, int planeNum)
{
	useMesh->clearVertices();
	for(int i = 0; i < 2000; i++)
	{
		bool selected = false;
		while (selected == false)
		{
			float z;
			if (useMesh == &meshBuffer[0])
			{
				z = ofRandomuf() * (100.0f * planeNum);
			}
			else
			{
				z = ofRandomf() * 100.0f + (-100.0f * planeNum);
			}
			ofPoint p(ofRandomf()*5, ofRandomf()*7.5, z);
			float noise = ofNoise(p.x, p.y, p.z);
			if(ofRandomuf() < CloudExp(noise))
			{
				selected = true;
				useMesh->addVertex(ofVec3f(p.x * ofGetWidth()/2.0f, p.y * ofGetWidth()/2.0f, p.z));
			}
		}
	}
	/*useMesh->addVertex(ofVec3f(-300.0f, 0, 0));

	useMesh->addVertex(ofVec3f(300.0f, 0, 0));

	useMesh->addVertex(ofVec3f(0, 300.0f, 0));
	*/
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
			if((sDist = currentVert.distanceSquared(useMesh->getVertex(j))) < first && sDist > 1000 && sDist <= 16000)
			{
				first = sDist;
				firstIndex = j;
			}
			else if((sDist = currentVert.distanceSquared(useMesh->getVertex(j))) < second && sDist > 1000 && sDist <= 16000 )
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
	float resolution = 0.1;
	for(int i = 0; i < useMesh->getNumVertices(); i++)
	{
		ofVec3f currentVert = useMesh->getVertex(i);
		float boxFilt[4];
		for (int i = 0; i < 4; i++)
		{
			float jitX = ofRandomf() * resolution;
			float jitY = ofRandomf() * resolution;
			boxFilt[i] = ofNoise(currentVert.x+jitX, currentVert.y+jitY, 0.0f);
		}
		float angle = (boxFilt[0] + boxFilt[1] + boxFilt[2] + boxFilt[3])/4.0f; 
		// radians                                                          1.5 * 2.0
		angle = angle * 5.0f * PI;
		//float angle = ofRandomuf() * 2.0f * PI;
		useMesh->getVerticesPointer()[i] = currentVert + (ofVec3f(cos(angle), sin(angle), 0.0f).normalized()) * movDist;
	}
}
