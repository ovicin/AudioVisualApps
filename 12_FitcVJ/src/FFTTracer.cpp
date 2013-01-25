//
//  FFTTracer.cpp
//  FFTMultiScene
//
//  Created by Atsushi Tadokoro on 1/5/13.
//
//

#include "FFTTracer.h"
#include "testApp.h"

class Tracker
{
public:
	deque<ofVec3f> points;
	float trackerLength;
    
	void setup(){

	}
    
	void update(ofVec3f _pos) {
        ofVec3f p = _pos;
        
		//if (p.distance(points.front()) > 1){
			points.push_front(p);
        //}
        
		if (points.size() > trackerLength){
			points.pop_back();
		}
	}
    
	void draw()	{
		if (points.empty()) return;
        
		for (int i = 0; i < points.size() - 1; i++){
			float a = ofMap(i, 0, points.size() - 1, 1, 0);
            
			ofVec3f &p0 = points[i];
			ofVec3f &p1 = points[i + 1];
            
			float dist = p0.distance(p1);
            
			//if (dist < 40) {
				//ofSetLineWidth(ofMap(dist, 0, 30, 0, 10));
				//ofSetColor(dist*20, 127-dist*10, 255-dist*20);
				ofLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
			//}
		}
	}
    
	void clear() {
		points.clear();
	}
    
	void setTrackerLength(float _trackerLength) {
		trackerLength = _trackerLength;
	}
};

vector<Tracker*> trackers;

FFTTracer::FFTTracer(){
    fft_size = ((testApp*)ofGetAppPtr())->fft_size;
    magnitude = new float[fft_size];
    
    for (int i = 0; i < fft_size; i++) {
        ofVec3f p = ofVec3f(0, 0, 0);
        pos.push_back(p);
        ofVec3f v = ofVec3f(0, 0, 0);
        vel.push_back(v);
        
        Tracker *t = new Tracker;
        t->setup();
        t->setTrackerLength(16);
        trackers.push_back(t);
    }
    
    //ばねパラメータ
    /*
    stiffness = 1.0;
    damping = 0.9;
    mass = 100.0;
    */
     stiffness = 4.0;
     damping = 0.95;
     mass = 500.0;
}

void FFTTracer::update(){
    for (int i=0; i<fft_size; i++) {
        magnitude[i] = powf(((testApp*)ofGetAppPtr())->magnitude[i], 0.1);
        float addFroce;
        addFroce = magnitude[i] *1000.0;
        float direction = ofRandom(360);
        ofVec3f add = ofVec3f(cos(direction) * addFroce, sin(direction) * addFroce,  ofRandom(-1,1)*addFroce);
        ofVec3f force = stiffness * -pos[i] + add;
        ofVec3f acceleration = force / mass;
        vel[i] = damping * (vel[i] + acceleration);
        pos[i] += vel[i];
        
        trackers[i]->update(pos[i]);
    }
}

void FFTTracer::draw(){
    glEnable(GL_DEPTH_TEST);
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofDisableLighting();
    ofPushMatrix();
    ofScale(2.0, 2.0, 2.0);
    ofRotateX(ofGetElapsedTimef() * 4);
    ofRotateY(ofGetElapsedTimef() * 3);
    ofRotateZ(ofGetElapsedTimef() * 2);
    for (int i=0; i<fft_size; i++) {
        if(magnitude[i] > 1){
            ofSetColor(ofColor::fromHsb(255 * i / fft_size, 127, 100));
            ofSetLineWidth(((testApp*)ofGetAppPtr())->power[i]*8.0);
            trackers[i]->draw();
        }
    }
    ofPopMatrix();
    ofEnableLighting();
    glDisable(GL_DEPTH_TEST);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
}