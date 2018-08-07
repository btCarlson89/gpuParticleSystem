//
//  gParticles.cpp
//  gpuParticleSystem
//
//  Created by Brian on 8/4/18.
//
//

#include "gParticles.h"

//--------------------------------------------------------------
GParticles::GParticles() :
    current_fbo_index(0)
{
    
}

//--------------------------------------------------------------
GParticles::~GParticles()
{
    
}

//--------------------------------------------------------------
void GParticles::setup(unsigned int x_count, unsigned int y_count, ofPrimitiveMode primitive_mode, unsigned int num_textures)
{
    x_dim = x_count;
    y_dim = y_count;
    
    //  Fbo setup
    ofFbo::Settings s;
    s.internalformat = GL_RGBA32F_ARB;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.minFilter = GL_NEAREST;
    s.maxFilter = GL_NEAREST;
    s.wrapModeHorizontal = GL_CLAMP;
    s.wrapModeVertical = GL_CLAMP;
    s.width = x_dim;
    s.height = y_dim;
    s.numColorbuffers = num_textures;
    
    for(unsigned i = 0; i < 2; ++i)
    {
        fbos[i].allocate(s);
    }
    
    //  Particle mesh setup
    particle_mesh.clear();
    
    for(int y = 0; y < y_dim; ++y)
    {
        for(int x = 0; x < x_dim; ++x)
        {
            particle_mesh.addVertex(ofVec3f(200.f * x / (float)x_dim - 100.f, 200.f * y / (float)y_dim - 100.f, -500.f));
            particle_mesh.addTexCoord(ofVec2f(x, y));
        }
    }
    particle_mesh.setMode(primitive_mode);
    
    //  BG mesh setup
    bg_mesh.addVertex(ofVec3f(-1.f, -1.f, 0.f));
    bg_mesh.addVertex(ofVec3f( 1.f, -1.f, 0.f));
    bg_mesh.addVertex(ofVec3f( 1.f,  1.f, 0.f));
    bg_mesh.addVertex(ofVec3f(-1.f,  1.f, 0.f));
    
    bg_mesh.addTexCoord(ofVec2f(0.f, 0.f));
    bg_mesh.addTexCoord(ofVec2f(x_dim, 0.f));
    bg_mesh.addTexCoord(ofVec2f(x_dim, y_dim));
    bg_mesh.addTexCoord(ofVec2f(0.f, y_dim));
    
    bg_mesh.addIndex(0);
    bg_mesh.addIndex(1);
    bg_mesh.addIndex(2);
    bg_mesh.addIndex(0);
    bg_mesh.addIndex(2);
    bg_mesh.addIndex(3);
    
    bg_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //  Shaders
    update_shader.load("shaders/" + UPDATE_SHADER_NAME);
    draw_shader.load("shaders/" + DRAW_SHADER_NAME);
    
    //  Texture setup
    initPositionTexture();
    initVelocityTexture();
    initColorTexture();
}

void GParticles::update()
{
    fbos[1 - current_fbo_index].begin(false);
    
    glPushAttrib(GL_ENABLE_BIT);
    glViewport(0, 0, x_dim, y_dim);
    glDisable(GL_BLEND);
    ofSetColor(255, 255, 255);
    fbos[1 - current_fbo_index].activateAllDrawBuffers();
    
    update_shader.begin();
    ofVec3f mouse(ofGetMouseX() - 0.5f * ofGetWidth(), 0.5f * ofGetHeight() - ofGetMouseY(), 0.0f);
    update_shader.setUniform3fv("mouse", mouse.getPtr());
    update_shader.setUniform1f("elapsed", ofGetLastFrameTime());
    update_shader.setUniform1f("radiusSquared", 160000.0f);
    update_shader.setUniform2f("resolution", ofVec2f(ofGetWidth(), ofGetHeight()));
    
    for(unsigned i = 0; i < fbos[current_fbo_index].getNumTextures(); ++i)
    {
        ostringstream stream;
        stream << "particles" << ofToString(i);
        update_shader.setUniformTexture(stream.str().c_str(), fbos[current_fbo_index].getTexture(i), i);
    }
    
    bg_mesh.draw();
    update_shader.end();
    glPopAttrib();
    
    fbos[1 - current_fbo_index].end();
    
    current_fbo_index = 1 - current_fbo_index;
}

void GParticles::draw()
{
    glPointSize(2.0);
    draw_shader.begin();
    
    for(unsigned i = 0; i < fbos[current_fbo_index].getNumTextures(); ++i)
    {
        ostringstream stream;
        stream << "particles" << ofToString(i);
        draw_shader.setUniformTexture(stream.str().c_str(), fbos[current_fbo_index].getTexture(i), i);
    }
    
    particle_mesh.draw();
    draw_shader.end();
}

void GParticles::loadTexture(unsigned int index, float* texture)
{
    if(index < fbos[current_fbo_index].getNumTextures())
    {
        fbos[current_fbo_index].getTexture(index).bind();
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, x_dim, y_dim, GL_RGBA, GL_FLOAT, texture);
        fbos[current_fbo_index].getTexture(index).unbind();
    }
    else
    {
        ofLogError() << "Trying to load texture into non-existent buffer.";
    }
}

void GParticles::initPositionTexture()
{
    float* pos_tex = new float[x_dim * y_dim * 4]; //4 for RGBA
    
    float screen_w = ofGetWidth();
    float screen_h = ofGetHeight();
    
    for(unsigned int y = 0; y < y_dim; ++y)
    {
        for(unsigned int x = 0; x < x_dim; ++x)
        {
            unsigned int index = y * x_dim + x;
            pos_tex[index * 4 + 0] = screen_w * x / (float)x_dim - 0.5f * screen_w;
            pos_tex[index * 4 + 1] = screen_h * y / (float)y_dim - 0.5f * screen_h;
            pos_tex[index * 4 + 2] = 0.0f;
            pos_tex[index * 4 + 3] = 0.0f;
        }
    }
    
    loadTexture(POSITION, pos_tex);
    delete[] pos_tex;
    
    return;
}

void GParticles::initVelocityTexture()
{
    clearTexture(VELOCITY);
}

void GParticles::initColorTexture()
{
    float* col_tex = new float[x_dim * y_dim * 4]; //4 for RGBA
    
    float screen_w = ofGetWidth();
    float screen_h = ofGetHeight();
    
    for(unsigned y = 0; y < y_dim; ++y)
    {
        for(unsigned x = 0; x < x_dim; ++x)
        {
            unsigned index = y * x_dim + x;
            col_tex[index * 4 + 0] = 0.33 + 0.67 * float(y) / float(y_dim);
            col_tex[index * 4 + 1] = 0.33 * float(y) / float(y_dim);
            col_tex[index * 4 + 2] = 1.0f - 0.67 * float(x) / float(x_dim);
            col_tex[index * 4 + 3] = 1.0f;
        }
    }
    
    loadTexture(COLOR, col_tex);
    delete[] col_tex;
    
    return;
}

void GParticles::clearTexture(unsigned int index)
{
    float* tex = new float[x_dim * y_dim * 4];
    memset(tex, 0, sizeof(float) * x_dim * y_dim * 4);
    
    loadTexture(index, tex);
    delete[] tex;
    
    return;
}









