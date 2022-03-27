#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include "helper/plane.h"
#include "helper/objmesh.h"

// Sound Library - Used for background music
#include <irrklang/irrklang.h>
using namespace irrklang;


// ImGUI Library - Used for the GUI elements on screen
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "helper/torus.h"
#include "helper/teapot.h"
#include <glm/glm.hpp>

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram shader;
    
    //Coded Meshes
    Torus torus;
    Teapot teapot;
    Plane plane; //plane surface

    //Imported Meshes
    std::unique_ptr<ObjMesh> houseMesh; //House mesh
    std::unique_ptr<ObjMesh> islandMesh; //Island mesh
    std::unique_ptr<ObjMesh> treeMesh; //Tree mesh

    //Angle (used for animating objects e.g. lighting position)
    float angle;

    float tPrev;

    void setMatrices();
    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void refreshShader();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
