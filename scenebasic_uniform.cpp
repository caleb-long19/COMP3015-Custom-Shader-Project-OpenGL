#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenebasic_uniform.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


bool switchShader_to_blinnPhongSpotlight = true;
bool switchShader_to_bpToonSpotlight = false;
bool switchShader_to_phongDirectional = false;


glm::vec3 teapotColour(1.0f, 0.0f, 0.0f);
glm::vec3 torusColour(0.7f, 1.0f, 0.3f);
glm::vec3 planeColour(0.5f, 0.3f, 0.9f);

glm::vec3 moveDirectionLight(1.0f, 0.0f, 0.0f);
glm::vec3 moveSpotlight(0.7f, 1.0f, 0.3f);



// Start the sound engine
ISoundEngine* soundEngine = createIrrKlangDevice();
ISoundEngine* backgoundMusic = createIrrKlangDevice();



//constructor for torus

SceneBasic_Uniform::SceneBasic_Uniform() : 
    tPrev(0),
    plane(50.0f, 50.0f, 1, 1),
    teapot(14, glm::mat4(1.0f)),
    torus(1.5f * 1.5f, 0.75f * 0.75f, 10, 10)

{
    houseMesh = ObjMesh::load("media/models/house_low/House_Model.obj", true);
    islandMesh = ObjMesh::load("media/models/island_low/Island_Model.obj", true);
    treeMesh = ObjMesh::load("media/models/tree_low/Tree_Model.obj", true);
}


//constructor for teapot
//SceneBasic_Uniform::SceneBasic_Uniform() : teapot(13, glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f))) {}


int main(int argc, char* argv[])
{
    SceneRunner runner("Shader_Basics");

    std::unique_ptr<Scene> scene;

    scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());

    return runner.run(*scene);
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    refreshShader();

    #pragma region Irrklang & ImGui Initilisation
    // Initialising ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();



    // Loop background music in game and set the volume
    ISound* music = soundEngine->play2D("media/audio/Dreamer-by-Hazy.flac", true, false, true);
    music->setVolume(0.05f);
    #pragma endregion

}


void SceneBasic_Uniform::refreshShader()
{
    if (switchShader_to_phongDirectional == true)
    {
        view = glm::lookAt(vec3(4.0f, 4.0f, 6.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        projection = mat4(1.0f);

        angle = 0.0;

        shader.setUniform("Light.Position", view * glm::vec4(1.0f, 5.0f, 0.0f, 0.0f));
        shader.setUniform("Light.L", vec3(0.8f, 0.8f, 0.8f));
        shader.setUniform("Light.La", vec3(0.1f));
    }

    if (switchShader_to_blinnPhongSpotlight == true)
    {
        view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        projection = mat4(1.0f);

        angle = 0.0;

        shader.setUniform("Spot.L", vec3(0.6f));
        shader.setUniform("Spot.La", vec3(1.0f));
        shader.setUniform("Spot.Exponent", 50.0f);
        shader.setUniform("Spot.Cutoff", glm::radians(40.0f));
    }


    if (switchShader_to_bpToonSpotlight == true)
    {
        view = glm::lookAt(vec3(4.0f, 4.0f, 6.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        projection = mat4(1.0f);

        angle = 0.0;

        shader.setUniform("Light.L", vec3(0.9f));
        shader.setUniform("Light.La", vec3(0.2f));
    }
}



void SceneBasic_Uniform::compile()
{
    // SWITCH TO DIRECTIONAL LIGHT SHADER - IF TRUE
    if (switchShader_to_phongDirectional == true)
    {
        try {
            shader.compileShader("shader/basic_uniform.vert");
            shader.compileShader("shader/basic_uniform.frag");
            shader.link();
            shader.use();
        }
        catch (GLSLProgramException& e) {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    // SWITCH TO SPOTLIGHT SHADER - IF TRUE
    if (switchShader_to_blinnPhongSpotlight == true)
    {
        try {
            shader.compileShader("shader/blinnPhongShader.vert");
            shader.compileShader("shader/blinnPhongShader.frag");
            shader.link();
            shader.use();
        }
        catch (GLSLProgramException& e) {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    // SWITCH TO SPOTLIGHT TOON SHADER - IF TRUE
    if (switchShader_to_bpToonSpotlight == true)
    {
        try {
            shader.compileShader("shader/bpToonShader.vert");
            shader.compileShader("shader/bpToonShader.frag");
            shader.link();
            shader.use();
        }
        catch (GLSLProgramException& e) {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void SceneBasic_Uniform::update(float t)
{
    if (switchShader_to_blinnPhongSpotlight == true || switchShader_to_bpToonSpotlight == true || switchShader_to_phongDirectional == true)
    {
        //update your angle here
        float deltaT = t - tPrev;
        if (tPrev == 0.0f) deltaT = 0.0f;
        tPrev = t;

        angle += 1.0f * deltaT;
        if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
    }

}

void SceneBasic_Uniform::render()
{
  
    #pragma region BLINN PHONG SHADER SETTINGS & MODELS
    //THIS IS THE DIRECTIONAL CODE
    if (switchShader_to_phongDirectional == true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec4 lightPos = vec4(15.0f * cos(angle), 15.0f, 15.0f * sin(angle), 1.0f);
        shader.setUniform("Light.Position", view * lightPos);


        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 180.0f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.2f, 0.2f, 0.2f));
        setMatrices();
        houseMesh->render();




        shader.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
        shader.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        shader.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
        shader.setUniform("Material.Shininess", 100.0f);

        model = mat4(1.0f);

        //Change the location (X.Y.Z) of the mesh object
        model = glm::translate(model, vec3(-20.0f, -20.0f, -20.0f));

        //Rotate the mesh object
        model = glm::rotate(model, glm::radians(135.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));

        //Change the size of the mesh object
        model = glm::scale(model, vec3(1.0f, 1.0f, 1.0f));
        setMatrices();
        islandMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 180.0f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(-10.0f, -13.5f, -12.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.5f, 0.5f, 0.5f));
        setMatrices();
        treeMesh->render();

    }
    #pragma endregion



    // THIS IS THE SPOTLIGHT SETTINGS
    #pragma region BLINN PHONG SHADER SETTINGS & MODELS
    if (switchShader_to_blinnPhongSpotlight == true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
        shader.setUniform("Spot.Position", vec3(view * lightPos));
        mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));

        shader.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));

        shader.setUniform("Material.Kd", teapotColour);
        shader.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        shader.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
        shader.setUniform("Material.Shininess", 100.0f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
        model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        teapot.render();

        shader.setUniform("Material.Kd", torusColour);
        shader.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        shader.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
        shader.setUniform("Material.Shininess", 100.0f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(-1.0f, 0.75f, 3.0f));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        torus.render();


        shader.setUniform("Material.Kd", planeColour);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        shader.setUniform("Material.Shininess", 180.0f);

        model = mat4(1.0f);
        setMatrices();
        plane.render();

    }
    #pragma endregion



    //THIS IS THE TOON SPOTLIGHT CODE
    #pragma region TOON SHADER SETTINGS & MODELS
    if (switchShader_to_bpToonSpotlight == true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
        shader.setUniform("Light.Position", view* lightPos);

        shader.setUniform("Material.Kd", teapotColour);
        shader.setUniform("Material.Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
        model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        teapot.render();

        shader.setUniform("Material.Kd", torusColour);
        shader.setUniform("Material.Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(-1.0f, 0.75f, 3.0f));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        torus.render();

        shader.setUniform("Material.Kd", planeColour);
        shader.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);

        model = mat4(1.0f);
        setMatrices();
        plane.render();
    }
    #pragma endregion

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display window with background colour and player speed changing values
    {
        ImGui::Begin("Dynamic Shader Representation Tool - Editor Window"); // Create Window - Title: Super Dodger

        ImGui::ColorEdit3("Change Teapot Colour: ", (float*)&teapotColour);
        ImGui::ColorEdit3("Change Torus Colour: ", (float*)&torusColour);
        ImGui::ColorEdit3("Change Plane Colour: ", (float*)&planeColour);


        if (ImGui::Button("Change Shader To Blinn Phong - Spotlight"))
        {
            switchShader_to_blinnPhongSpotlight = true;
            switchShader_to_phongDirectional = false;
            switchShader_to_bpToonSpotlight = false;
 
        }

        if (ImGui::Button("Change Shader To Phong - Directional"))
        {
            switchShader_to_phongDirectional = true;
            switchShader_to_blinnPhongSpotlight = false;
            switchShader_to_bpToonSpotlight = false;

        }

        if (ImGui::Button("Change Shader To BP Toon - Directional"))
        {
            switchShader_to_phongDirectional = false;
            switchShader_to_blinnPhongSpotlight = false;
            switchShader_to_bpToonSpotlight = true;
        }

        //ImGui::ChangeCameraSpeed("Player Speed", &camera.cameraSpeed, 0.0f, 100.0f); // Player can alter the speed of the camera
        //ImGui::BackgroundColour("Background Colour", (float*)&clear_BackgroundColour); // Change the RGB values of the background

        ImGui::Text("Average Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Dispplay the framerate
        ImGui::End();
    }

    ImGui::Render();


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //we create a model view matrix

    shader.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix

    shader.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix

    shader.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
