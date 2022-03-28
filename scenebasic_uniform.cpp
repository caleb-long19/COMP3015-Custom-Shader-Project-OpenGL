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


// Switch Shaders on and off
bool switchShader_to_blinnPhongSpotlight = false;
bool switchShader_to_bpToonSpotlight = false;
bool switchShader_to_phongDirectional = false;


//Bool to turn music on or off
bool toggleCurrentMusic = true;


// Start the sound engine
ISoundEngine* backgoundMusic = createIrrKlangDevice();
ISound* music;


std::string shaderTypeInput;

SceneBasic_Uniform::SceneBasic_Uniform() : 
    angle(0.0f), 
    tPrev(0.0f), 
    rotSpeed(glm::pi<float>() / 8.0f)
{
    //Custom Models
    houseMesh = ObjMesh::load("media/models/House_Model.obj", true);
    islandMesh = ObjMesh::load("media/models/Island_Model.obj", true);
    treeMesh = ObjMesh::load("media/models/Tree_Model.obj", true);

    //Third-Party Models
    ogre = ObjMesh::load("media/bs_ears.obj", false, true);
}




int main(int argc, char* argv[])
{
    std::cout << "PLEASE CHOOSE THE SHADER TYPE YOU WISH TO EXPERIENCE: " << std::endl;
    std::cout << "For The Toon Shader: Press T" << std::endl;
    std::cout << "For The Blinn Phong Spotlight (Normals) Shader: Press B" << std::endl;
    std::cout << "For The Phong Direction Lighting Shader: Press P" << std::endl;
    std::cin >> shaderTypeInput;

    if (shaderTypeInput == "T" || shaderTypeInput == "t")
    {
        switchShader_to_bpToonSpotlight = true;
    }
    else if (shaderTypeInput == "B" || shaderTypeInput == "b")
    {
        switchShader_to_blinnPhongSpotlight = true;
    }
    else if (shaderTypeInput == "P" || shaderTypeInput == "p")
    {
        switchShader_to_phongDirectional = true;
    }
    else 
    {
        std::cout << "WRONG INPUT! " << std::endl;
        exit(EXIT_SUCCESS);
    }

    SceneRunner runner("A Cabin In The Woods...");

    std::unique_ptr<Scene> scene;

    scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());

    return runner.run(*scene);
}


void SceneBasic_Uniform::initScene()
{

    compile();
    glEnable(GL_DEPTH_TEST);
    refreshShader();
    ImGuiSetup();
    toggleMusic();
}




void SceneBasic_Uniform::compile()
{
    // SWITCH TO DIRECTIONAL LIGHT SHADER - IF TRUE
    if (switchShader_to_phongDirectional == true)
    {
        try {
            shader.compileShader("shader/phongShader.vert");
            shader.compileShader("shader/phongShader.frag");
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
    //update your angle here
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;

    if (this->m_animate)
    {
        angle += 0.8f * deltaT;
        if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
    }

}




void SceneBasic_Uniform::render()
{
  
    // THIS IS THE SPOTLIGHT SETTINGS
    #pragma region BLINN PHONG SHADER SETTINGS & MODELS
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Animate the direct lighting position each loop
    vec4 directLight = vec4(15.0f * cos(angle), 15.0f, 15.0f * sin(angle), 1.0f);
    shader.setUniform("Light.Position", view * directLight);


    //Animate the spotlight lighting position each loop
    vec4 lightPos = vec4(15.0f * cos(angle), 15.0f, 15.0f * sin(angle), 1.0f);
    shader.setUniform("Spot.Position", vec3(view * lightPos));


    //Set the Direction for the spot light
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    shader.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));
    #pragma endregion

    if (switchShader_to_blinnPhongSpotlight == true) {
        //Alter the Diffuse/Spectural/Ambient Light Reflective Power
        //Alter the Shininess (Glossy Look) of Spectural Light
        shader.setUniform("Material.Ks", 0.3f, 0.3f, 0.3f);
        shader.setUniform("Material.Shininess", 20.0f);

        // Alter the Poisition/Rotation/Size of the Mesh/Model
        model = mat4(1.0f);
        model = glm::translate(model, vec3(3.0f, 4.0f, 5.0f));
        model = glm::rotate(model, glm::radians(70.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(2.4f, 2.4f, 2.4f));

        setMatrices();
        ogre->render();
    }
    else
    {
     #pragma region House Model Settings
        // Alter the Diffuse/Spectural/Ambient Light Reflective Power
        // Alter the Shininess (Glossy Look) of Spectural Light
        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.3f, 0.3f, 0.3f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);

        // Alter the Poisition/Rotation/Size of the Mesh/Model
        model = mat4(1.0f);
        model = glm::translate(model, vec3(2.0f, 5.1f, -1.5f));
        model = glm::rotate(model, glm::radians(85.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.4f, 0.4f, 0.4f));

        //Load Matrices Settings & Render House Mesh
        setMatrices();
        houseMesh->render();

    #pragma endregion


     #pragma region Island Model Settings
        // Alter the Diffuse/Spectural/Ambient Light Reflective Power
        // Alter the Shininess (Glossy Look) of Spectural Light
        shader.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
        shader.setUniform("Material.Ks", 0.3f, 0.3f, 0.3f);
        shader.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
        shader.setUniform("Material.Shininess", 20.0f);


        // Alter the Poisition/Rotation/Size of the Mesh/Model
        model = mat4(1.0f);
        model = glm::translate(model, vec3(-3.0f, 0.0f, -3.0f));
        model = glm::rotate(model, glm::radians(120.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(1.0f, 1.0f, 1.0f));


        //Load Matrices Settings & Render Island Mesh
        setMatrices();
        islandMesh->render();
    #pragma endregion


     #pragma region Tree Models Settings
        // Alter the Diffuse/Spectural/Ambient Light Reflective Power
        // Alter the Shininess (Glossy Look) of Spectural Light
        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.3f, 0.3f, 0.3f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);

        // Alter the Poisition/Rotation/Size of the Mesh/Model
        model = mat4(1.0f);
        model = glm::translate(model, vec3(-12.0f, 8.5f, -2.5f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.5f, 0.5f, 0.5f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);

        model = mat4(1.0f);
        model = glm::translate(model, vec3(-2.0f, 4.8f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.5f, 0.5f, 0.5f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);


        model = mat4(1.0f);
        model = glm::translate(model, vec3(1.0f, 5.0f, -2.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.5f, 0.5f, 0.5f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);


        model = mat4(1.0f);
        model = glm::translate(model, vec3(3.0f, 4.5f, 8.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.5f, 0.5f, 0.5f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);


        model = mat4(1.0f);
        model = glm::translate(model, vec3(4.0f, 4.5f, 4.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.2f, 0.2f, 0.2f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);


        model = mat4(1.0f);
        model = glm::translate(model, vec3(6.0f, 4.5f, 6.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.3f, 0.3f, 0.3f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();



        shader.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        shader.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        shader.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        shader.setUniform("Material.Shininess", 20.0f);


        model = mat4(1.0f);
        model = glm::translate(model, vec3(2.0f, 4.2f, 6.0f));
        model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(0.3f, 0.3f, 0.3f));

        //Load Matrices Settings & Render Tree Mesh
        setMatrices();
        treeMesh->render();
    #pragma endregion
    }

    #pragma region ImGUI Elements
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display window with background colour and player speed changing values
    {
        ImGui::Begin("Dynamic Shader Representation Tool - Editor Window"); // Create Window - Title: Super Dodger

        ImGui::Checkbox("Toggle Animation", &m_animate);
        if (ImGui::Button("Toggle Music"))
        {
            if (toggleCurrentMusic == true)
            {
                //Turn music off
                toggleCurrentMusic = false;
                toggleMusic();
            }
            else 
            {
                //Turn music on
                toggleCurrentMusic = true;
                toggleMusic();
            }
        }
        if (ImGui::Button("Exit Application"))
        {
            exit(EXIT_SUCCESS);
        }
        

        ImGui::Text("Average Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Dispplay the framerate
        ImGui::End();
    }

    ImGui::Render();


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #pragma endregion
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
    projection = glm::perspective(glm::radians(90.0f), (float)w / h, 0.3f, 100.0f);
}




void SceneBasic_Uniform::refreshShader()
{

    mat4 directionalView = glm::lookAt(vec3(4.0f, 4.0f, 6.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    projection = mat4(1.0f);
    angle = 0.0;

#pragma region Directional Lighting - Phong Shading
    // Alters the position of the light, the specular settings, and the ambient settings 
    if (switchShader_to_phongDirectional == true || switchShader_to_bpToonSpotlight == true)
    {
        shader.setUniform("Light.Position", directionalView * glm::vec4(1.0f, 25.0f, 0.0f, 0.0f));
        shader.setUniform("Light.L", vec3(0.9f, 0.9f, 0.9f));
        shader.setUniform("Light.La", vec3(1.0f));

    }
#pragma endregion


#pragma region Spotlight Lighting - Blinn Phong Shading
    // Alters the specular settings, ambient light,
    if (switchShader_to_blinnPhongSpotlight == true)
    {
        shader.setUniform("Spot.L", vec3(0.8f));
        shader.setUniform("Spot.La", vec3(0.5f));
        shader.setUniform("Spot.Exponent", 20.0f);
        shader.setUniform("Spot.Cutoff", glm::radians(30.0f));

        // Load diffuse texture
        GLuint diffuseTexture = Texture::loadTexture("media/texture/ogre_diffuse.png");

        // Load normal map
        GLuint normalMapTexture = Texture::loadTexture("media/texture/ogre_normalmap.png");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMapTexture);
    }
    else
    {
        shader.setUniform("Fog.MaxDist", 11.5f);
        shader.setUniform("Fog.MinDist", 0.0f);
        shader.setUniform("Fog.Color", 0.2f, 0.5f, 0.9f);

        // Load texture and bind it to the active meshes
        GLuint texID = Texture::loadTexture("media/nice69-32x.png");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);

    }
#pragma endregion

}




void SceneBasic_Uniform::toggleMusic()
{
    if (toggleCurrentMusic == true) 
    {
        // Loop background music in game and set the volume
        music = backgoundMusic->play2D("media/audio/Dreamer-by-Hazy.flac", true, false, true);
        music->setVolume(0.07f);
    }
    else 
    {
        music->stop();
    }

}




void SceneBasic_Uniform::ImGuiSetup()
{
    // Initialising ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

}
