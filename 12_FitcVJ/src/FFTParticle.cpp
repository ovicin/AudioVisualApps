//
//  FFTParticle.cpp
//  FFTMultiScene
//
//  Created by Atsushi Tadokoro on 1/5/13.
//
//

#include "FFTParticle.h"
#include "testApp.h"

FFTParticle::FFTParticle(){
    fft_size = ((testApp*)ofGetAppPtr())->fft_size;
    
    // 頂点情報の初期化
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// 移動速度
		billboardVels[i].set(0,0,0);
		
		// 初期位置
		billboardVerts[i].set(0,0,0);
		
		// 色
		billboardColor[i].set(1, 1, 1);
	}
	
	// VBOにパーティクルの色と座標を格納
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// テクスチャー画像を設定
	ofDisableArbTex();
	texture.loadImage("sub.png");
    
    magnitude = new float[NUM_BILLBOARDS];
    stiffness = 0.1;
    damping = 0.91;
    mass = 60.0;
}

void FFTParticle::update(){
    
    // パーティクルの座標を更新
	float t   = (ofGetElapsedTimef()) * 0.9f;
	float div = 3000.0;
	
    /*
     for (int i=0; i<NUM_BILLBOARDS; i++) {
     
     // ノイズを生成し移動速度に
     ofVec3f vec(ofSignedNoise(t, billboardVerts[i].y/div, billboardVerts[i].z/div),
     ofSignedNoise(billboardVerts[i].x/div, t, billboardVerts[i].z/div),
     ofSignedNoise(billboardVerts[i].x/div, billboardVerts[i].y/div, t));
     
     vec *= 0.99;
     
     billboardColor[i].set(1,1,1, ((testApp*)ofGetAppPtr())->magnitude[i]);
     
     // 速度をもとにパーティクル位置を更新
     billboardVels[i] += vec;
     billboardVerts[i] += billboardVels[i];
     billboardVels[i]  *= 0.94f;
     }
     */
    
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        
    }
    
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        magnitude[i] = powf(((testApp*)ofGetAppPtr())->magnitude[i], 0.75);
        float addFroce;
        addFroce = magnitude[i] *200.0;
        float direction = ofRandom(360);
        ofVec3f add = ofVec3f(cos(direction) * addFroce, sin(direction) * addFroce,  ofRandom(-1,1)*addFroce);
        ofVec3f force = stiffness * -billboardVerts[i] + add;
        ofVec3f acceleration = force / mass;
        billboardVels[i] = damping * (billboardVels[i] + acceleration);
        billboardColor[i].set(1.0,1.0,1.0,magnitude[i]/1.54);
        billboardVerts[i] += billboardVels[i];
    }
    
	
	// VBOの頂点と色の情報を更新
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
}

void FFTParticle::draw(){
    // パーティクル描画設定
    glDepthMask(GL_FALSE);
    ofEnablePointSprites();
    texture.getTextureReference().bind();
    
    // パーティクルのZ軸の位置によって大きさを変化させる
    static GLfloat distance[] = { 0.0, 0.0, 1.0 };
    glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance);
    glPointSize(50000);
    
    ofTranslate(0, 0, -1000);
    // パーティクルを描画
    //ofRotateY(ofGetElapsedTimef() * 10);
    billboardVbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
    texture.getTextureReference().unbind();
    ofDisablePointSprites();
    glDepthMask(GL_TRUE);

}

void FFTParticle::reset(){
    // 頂点情報の初期化
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// 移動速度
		billboardVels[i].set(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10));
		
		// 初期位置
		billboardVerts[i].set(ofRandom(-2000,2000),ofRandom(-2000,2000),ofRandom(-2000,2000));
		
		// 色
		billboardColor[i].set(1, 1, 1, 0.5);
	}
}
