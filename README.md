# Dynamic Shader Representation Tool - DSRT

## Project Brief
DSR Tool is an application that is used to display imported models and allow the user to switch between multiple different shading techiques. The Shading Technique used will be Phong Shading, Blinn Phong Shading, & Toon Shading, 2 lighting techniques will also be used. These include: Directional, and Spotlight

### Features
- First Person Camera
- Interactive User Interface
  - Choose your shader on launch 
  - Turn off lighting animations
  - Turn off music
  - Exit Application
- Imported 3D Low-Poly Models (With textures)
- Background Music
- Phong Shading
- Blinn Phong Shading
- Toon Shading
- Various Lighting Techniques
  - Directional
  - Spotlight 
- Regular Textures (Custom Textures - Made via Photoshop)
- Normal Textures (Ogre Model - Given via Lab Work) 

### Project Write-up
#### Which version Visual Studio and Operating System you used to test/write the code locally?:
The verion of Visual Studio that I used to develop & test the Dynamic Shader Representation Tool was Microsoft Visual Studio Community 2019 (Version 16.11.5). The Operating System I used was Windows 10.

#### How does it work?:
The application begins with a command prompt to allow the user to decide what Shader Type they wish to experience. You can press "T" for the Toon Shader, It will then load the textures/models and will show a low-poly forest background with directional lighting.
Pressing "B" will do the same, but will instead display an Ogre head with a normal mapping texture and spotlight lighting. Pressing "P" will provide the same as the Toon Shader, except we will have Phong Shading with directional lighting. Each of these come with animated lighting.
The user can also interact with the ImGui in the top left corner, it will allow them to toggle the lighting animations on or off, turn the music on or off, and even exit the application. Other settings include, changing material reflectivity settings, light intensity settings, fog colour and distance, & more.

#### How Does the code fit together and how to navigate it:
The code links together via a variation of different files. I will discuss the main files in the project that are used, scenebasic_uniform.h/.cpp, & bpToonShader.frag/vert. We use the scenebasic_uniform.h to include files from across the project. We declare variables that we can access in scenebasic_uniform.cpp.
These declared variables can be object meshes, floats, bools, methods. This file was primarily used to organise the coding structure of the scenebasic_uniform.cpp. The scenebasic_uniform.cpp file is used to load the shaders, link them and activate them. We also use this class to
set the uniform variables of the Shader Classes. For example, on the bpToonShader.frag, we use directional lighting, this requires a Light Uniform structure containing vec variables e.g. positon. This Position uniform can then be set in the scenebasic_uniform.cpp file. In the
refreshShader() Method, we can set the Light.Position uniform and provide it with vec4 position e.g. glm::vec4(1.0f, 25.0f, 0.0f, 0.0f)). This is repeated for the other uniform variables located in the vert/frag shader files. The header/cpp files are presented into their own folders.
This allows for better naviation and readability. The render method is where we set uniforms such as the diffuse/spectural/ambient lighting intensity values for each object. 

Overall navigation should be clear, the code has been provided with brief but descriptive comments that should explain the major aspects of the application. I would recommend entering the scenebasic_uniform.h/.cpp files and reading the comments to get a 
better understanding of the code. The shaders also contain plenty of comments to understand what's happening.

#### Anything else to help you understand:
Inside the submission folder, you will find a folder called "COMP3015 - Shader Representation Tool - EXE Folder". Inside that folder is a .exe called "Project_Template.exe". Double click this application and you will be greeted
with a Command Prompt waiting for an Input. You can press "T" to view the toon shader (Textures), "B" for the Blinn Phong Shader (Normal Textures), and "P" for the Phong Shading. The program will close if you do not enter the correct information.
After entering your shading choice, you will greeted with an animated scene that will either display a low-poly background or an ogre head that contains normal textures. The GUI in the top left can be interacted with your mouse, mess around with the
lighting settings, material settings, colours, fog distance and more.

#### Link to YouTube Video:
https://www.youtube.com/watch?v=NYuAtj07los

### Screenshots
#### User Interface
<kbd>![DSRT - Introduction](Screenshots/User_Input_Shader_Selection.png?)</kbd>

#### Toon Shading
<kbd>![Toon Shading](Screenshots/Toon_Shader.png?)</kbd>

#### Phong Shading
<kbd>![Phong Directional Shading](Screenshots/Phong_Directional_Shading.png?)</kbd>

#### Blinn Phong Normal Texture Shading
<kbd>![Blinn Phong Normal Texture Shading](Screenshots/BP_Normal_Shading.png?)</kbd>

### References
#### YouTube Channels
 - https://www.youtube.com/c/VictorGordan - Victor Gordon
 - https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw - The Cherno
 - https://www.youtube.com/channel/UCCIfx6nIIWeOCogxGg4j3xQ - Michael Grieco
 - https://www.youtube.com/channel/UC4EJN2OSNdl-mSxGjitRvyA - Code, Tech, and Tutorials

#### Books
 - OpenGL Shading Lagauge Cookbooks - David Wolff
 - OpenGL Super Bible - Graham Sellers
 - Learn OpenGL (Print Edition) - Frahaan Hussain

#### Websites
 - https://learnopengl.com/ - Joey de Vries
 - https://www.khronos.org/opengl/ - Kronos Group
 - Plymouth University DLE Resources - Marius Varga

#### Assets Used
 - https://www.youtube.com/watch?v=LSqttgJRud8 - Background Music
 - Orge Model & Textures - Lab 4 Folder - Had to use it due to errors with my normal map models and textures (Massive vector error)
