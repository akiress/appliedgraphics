#include "GLFundamentals_modified.hpp"
#include "GLDemonstration.hpp"
#include "TGA.h"

#include <iostream>
#include <vector>

const int PARTICLE_COUNT = 450000;
const float PI = 3.14159265359f;

const char *title               = "Particle Demo";
const char *vertex_shader       = "vertex.glsl";
const char *fragment_shader     = "fragment.glsl";
const char *geometry_shader     = "geometry.glsl";

class ParticleSystem : public gl::demonstration
{
    GLuint program;

    GLuint PositionAttribute;
    GLuint TextureAttribute;
    GLuint NormalAttribute;

    GLuint vao;
    GLuint vbo;
    GLuint tex;

    GLint ProjectionMatrixLocation;
    GLint ModelViewMatrixLocation;
    GLint NormalMatrixLocation;

    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat r;
    GLfloat theta;
    GLfloat phi;

    float life;

    int tgaWidth;
    int tgaHeight;
    int tgaDepth;

    std::vector<gl::vec3> position;
    std::vector<gl::vec3> direction;

public:
    ParticleSystem();

    void draw();
    void setup();
    void createParticles();
    void updateParticles();
    void FPScounter();
    bool isDead();
};

ParticleSystem::ParticleSystem() :
    gl::demonstration("Fire", 960, 540),
    program(0),
    life(1.0f / 1000.0f),
    position(PARTICLE_COUNT),
    direction(PARTICLE_COUNT)
{
    if ((program = gl::init_program(vertex_shader, fragment_shader, geometry_shader)))
    {
        glUseProgram(program);

        PositionAttribute           = glGetAttribLocation(program, "vPosition");
        TextureAttribute            = glGetAttribLocation(program, "vTexCoord");
        NormalAttribute             = glGetAttribLocation(program, "vNormal");

        ModelViewMatrixLocation     = glGetUniformLocation(program, "ModelView");
        ProjectionMatrixLocation    = glGetUniformLocation(program, "projection");
        NormalMatrixLocation        = glGetUniformLocation(program, "NormalMatrix");

        std::cout << "Program initialized." << std::endl;
    }

    else
    {
        std::cout << "Program failed to initialize." << std::endl;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    cam_position = gl::vec3(0.0f, 2.0f, 18.0f);
}

void ParticleSystem::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_TRUE, projection());
    glUniformMatrix4fv(ModelViewMatrixLocation, 1, GL_TRUE, view());
    glUniformMatrix3fv(NormalMatrixLocation, 1, GL_TRUE, normal(view()));

    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

}

bool ParticleSystem::isDead()
{
    return (life < 3.0f);
}

void ParticleSystem::createParticles()
{
    for (int i = 0; i < PARTICLE_COUNT; i++)
    {
        theta = (rand() % 10000 - 5000) / 5000.0f * 4 * PI;
        phi = (rand() % 10000 - 5000) / 5000.0f * 4 * PI;
        r = (rand() % 10000 - 5000) / 5000.0f;
        x = r * sin(phi) * cos(theta) * 3.0f * ((rand() % 3000 - 1000) / 1000.0f);
        y = r * sin(phi) * cos(theta) * 4.0f * ((rand() % 3000 - 1000) / 1000.0f);
        z = r * sin(phi) * 4.0f * ((rand() % 3000 - 1000) / 1000.0f);
        if (y > 0.0f)
        {
            position[i] = gl::vec3(x, y, z);
        }
    }
}

void ParticleSystem::updateParticles()
{
    life += 0.1;
    for (int i = 0; i < PARTICLE_COUNT; i++)
    {

        theta = (rand() % 10000 - 5000) / 5000.0f * 4 * PI;
        phi = (rand() % 10000 - 5000) / 5000.0f * 4 * PI;
        r = (rand() % 10000 - 5000) / 5000.0f;
        x = r * sin(phi) * cos(theta) * 3.0f * ((rand() % 3000 - 1000) / 1000.0f);
        y = r * sin(phi) * cos(theta) * 4.0f * ((rand() % 3000 - 1000) / 1000.0f);

        if (!isDead())
        {
            z = z = r * sin(phi) * 4.0f * ((rand() % 3000 - 1000) / 1000.0f) * life;
        }
        else
        {
            z = r * sin(phi) * 4.0f * ((rand() % 3000 - 1000) / 1000.0f);
            life = 1.0f / 1000.0f;
        }

        if (y > 0.0f)
        {
            position[i] = gl::vec3(x, y, z);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gl::vec3)*position.size(), 0, GL_DYNAMIC_DRAW);

    gl::vec3 *mapped_vbo = reinterpret_cast<gl::vec3 *>(
                               glMapBufferRange(
                                   GL_ARRAY_BUFFER,
                                   0,
                                   sizeof(gl::vec3) * position.size(),
                                   GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
                               )
                           );

    std::copy(position.begin(), position.end(), mapped_vbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void ParticleSystem::setup()
{
    createParticles();

    tgaWidth = 128;
    tgaHeight = 128;
    tgaDepth = 32;

    const char *tga_source = "flame2.tga";
    glGenTextures (1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, tex);
    void *p = load_tga(tga_source, tgaWidth, tgaHeight, tgaDepth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if ( tgaDepth == 32 )
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, tgaWidth, tgaHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, p );
    if ( tgaDepth == 24 )
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, tgaWidth, tgaHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, p );
    free ( p );
    glUniform1i(TextureAttribute, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gl::vec3)*position.size(), &position[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(PositionAttribute);
    glVertexAttribPointer(PositionAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(gl::vec4), (char *)0 + 0 * sizeof(gl::vec3));

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}

void ParticleSystem::FPScounter() {}

int main(void)
{
    ParticleSystem ps;
    ps.setup();
    ps.run();

    return 0;
}