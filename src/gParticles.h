//
//  gParticles.h
//  gpuParticleSystem
//
//  Created by Brian on 8/4/18.
//
//

#pragma once

#include "ofMain.h"

class GParticles
{
public:
    
    GParticles();
    ~GParticles();
    
    //  Texture enum
    enum TextureIndex
    {
        POSITION,
        VELOCITY,
        COLOR
    };
    
    //  Constants
    const string UPDATE_SHADER_NAME = "update";
    const string DRAW_SHADER_NAME = "draw";
    
    void setup(unsigned int x_count, unsigned int y_count, ofPrimitiveMode primitive_mode = OF_PRIMITIVE_POINTS, unsigned int num_textures = 3);
    void update();
    void draw();
    
    void loadTexture(unsigned int index, float* texture);
    void initPositionTexture();
    void initVelocityTexture();
    void initColorTexture();
    void clearTexture(unsigned int index);
    
private:
    
    ofFbo fbos[2];
    
    ofVboMesh particle_mesh, bg_mesh;
    
    ofShader update_shader, draw_shader;
    
    unsigned int current_fbo_index;
    unsigned int x_dim, y_dim;
};
