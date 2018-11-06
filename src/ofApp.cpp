#include "ofMain.h"
#include "ofxGui.h"

#define NUM 2000

class ofApp : public ofBaseApp {
    
    const int WIDTH = ofGetWidth();
    const int HEIGHT = ofGetHeight();
    
    ofEasyCam cam;
    ofShader godray, lightingShader;
    ofSpherePrimitive lightSphere;
    
    ofMesh quad;
    float time;
    
    ofxPanel gui;
    ofxFloatSlider density;
    ofxFloatSlider weight;
    ofxFloatSlider decay;
    ofxFloatSlider exposure;
    ofxIntSlider sampleNum;

    ofSpherePrimitive box[NUM];
    ofVec3f pos[NUM];

    ofFbo fbo;
    bool bhide;
    
    //--------------------------------------------------------------
    void setup()
    {
        
        quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        quad.addVertex(ofVec3f(1.0, 1.0, 0.0));
        quad.addTexCoord(ofVec2f(1.0f, 1.0f));
        quad.addVertex(ofVec3f(1.0, -1.0, 0.0));
        quad.addTexCoord(ofVec2f(1.0f, 0.0f));
        quad.addVertex(ofVec3f(-1.0, -1.0, 0.0));
        quad.addTexCoord(ofVec2f(0.0f, 0.0f));
        quad.addVertex(ofVec3f(-1.0, 1.0, 0.0));
        quad.addTexCoord(ofVec2f(0.0f, 1.0f));
        
        gui.setup();
        gui.add(density.setup("density", 1.0, 0.0, 2.0));
        gui.add(weight.setup("weight", 0.01, 0.0, 1.0));
        gui.add(decay.setup("decay", 1.0, 0.95, 1.05));
        gui.add(exposure.setup("exposure", 1.0, 0.0, 1.0));
        gui.add(sampleNum.setup("sampleNum", 100.0, 1.0, 200.0));
        
        ofDisableArbTex();
        fbo.allocate(WIDTH, HEIGHT);
        ofEnableArbTex();
        
        
        lightingShader.load("shaders/phong");
        godray.load("shaders/lightScattering");
        
        
        lightSphere.set(20, 10);
        lightSphere.setPosition(0, 0, 0);
        
        float R = 430;
        for(int i = 0; i < NUM; i++)
            pos[i] = ofVec3f(ofRandom(-R, R), ofRandom(-R/2, R/2), ofRandom(-R, R));
        
    }
    
    //--------------------------------------------------------------
    void update()
    {
        time = ofGetElapsedTimef();
    }
    
    //--------------------------------------------------------------
    void draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        
        
        
        // 1. lighting Pass-----------------------------------------
        glViewport(0, 0, WIDTH, HEIGHT);
        fbo.begin();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        cam.begin();
        ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
        cam.end();
        
        lightingShader.begin();
        lightingShader.setUniformMatrix4f("view", viewMatrix);
        lightingShader.setUniformMatrix4f("projection", cam.getProjectionMatrix());

        lightingShader.setUniform3f("lightPos", lightSphere.getPosition());
        lightingShader.setUniform3f("viewPos", cam.getPosition());
        lightingShader.setUniform3f("u_color", ofVec3f(0.0));
        scene(lightingShader);
        
        ofMatrix4x4 model;
        model.translate(lightSphere.getPosition());
        lightingShader.setUniformMatrix4f("model", model);
        lightingShader.setUniform3f("u_color", ofVec3f(1.0, 1.0, 1.0));

        lightSphere.draw();
        lightingShader.end();
        fbo.end();
        
        
        fbo.draw(0, 0);
        
        // post Effect
        // Alpha BLendering with Volumetric Light Scattering Effect
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        godray.begin();
        godray.setUniformTexture("u_src", fbo.getTexture(), 0);
        
        ofVec2f screenSpaceLightPos = cam.worldToScreen(lightSphere.getPosition());
        screenSpaceLightPos = screenSpaceLightPos / ofVec2f(WIDTH, HEIGHT);
        godray.setUniform2f("u_screenSpaceLightPos", screenSpaceLightPos);
        godray.setUniform1f("u_density", density);
        godray.setUniform1f("u_weight", weight);
        godray.setUniform1f("u_decay", decay);
        godray.setUniform1f("u_exposure", exposure);
        godray.setUniform1i("u_sampleNum", sampleNum);
        quad.draw();
        godray.end();
        
        
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        if(bhide) gui.draw();
    }
    
    void scene(const ofShader& shader)
    {
        for(int i = 0; i < NUM; i++)
        {
            
                ofMatrix4x4 model;
                model.scale(0.5, 0.5, 0.5);
                model.translate(pos[i]);
                model.rotate(time*10.0, 1, -90, 0);
            
                shader.setUniformMatrix4f("model", model); //To do : use namespace
                box[i].draw();
        }
        
    }
    
    //--------------------------------------------------------------
    void keyPressed(int key)
    {
        if(key == ' ') bhide = !bhide;
        
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key)
    {
        
    }
    
};

    
    //==============================================================
    int main()
    {
        ofGLFWWindowSettings settings;
        settings.setGLVersion(4, 1);
        settings.setSize(1280, 720);
        settings.resizable = false;
        ofCreateWindow(settings);
        
        // this kicks off the running of my app
        // can be OF_WINDOW or OF_FULLSCREEN
        // pass in width and height too:
        ofRunApp(new ofApp());
        
    }
