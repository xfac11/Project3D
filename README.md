# Project3D
Project Directx 3D 11

Box creation with size and position only (Färdigt?)

1.2 
Mandatory techniques or implementation details: 
- You have to use a “perspective” camera, 
- You have to use textures in your 3D models, 
- You have to implement ambient, diffuse AND specular phong shading per pixel. 
- You have to implement one camera control mode: either FPS or ORBIT, 
  o FPS: Use “w-a-s-d” for moving and the mouse for looking at different directions 
  
  
Geometry (2):
    _Filip_
    o Parsing and rendering an existing model format (OBJ, MD5, etc). This includes parsing and using the material defined in the           
    format (textures). If you use a library like Assimp, it does not count Done (not if Material has to be loaded in)
    x Deferred Rendering
 
   
 
  
Texturing and Lighting (1): 
    _Daniel_
  o Textures (DONE)
  o Normal mapping (DONE)
  o Color/light etc
  o Phong Shading (specular)

Keyboard, inputs, Camera, FPS
  _Karlos_
  o DirectX 3D class system setup
  o FPS done! 
  o Keyboard and Mouse done!
  o simple physics (jump and fall)
  o Height-map terrain rendering, user can walk on the terrain using WASD keys.(done!) (can be slimmed)
  x Particle systems with Billboarded particles
  
  
  5/9 Tekniker gjorda 

Core techniques (1,1): 
  o Deferred Rendering (Filip)

- Geometry (2,1): 
  o Parsing and rendering an existing model format (OBJ, MD5, etc). This includes parsing and using the material defined in the           format (textures). If you use a library like Assimp, it does not count. (Filip,DONE)
  o Height-map terrain rendering (Karlos, DONE), user can walk on the terrain using WASD keys. (Karlos, DONE)

- Texturing and Lighting (1,2): 
  o Normal mapping (Daniel, DONE) 

- Projection Techniques (1,1): 
  o Shadow mapping 
  o Skybox (om denna ger poäng, gör denna)



- Acceleration Techniques (2,1): 
  o Portal Culling 
  o Front to Back rendering 
  o Back face culling using Geometry Shader (Karlos Done kindof (fix model if problem)
 
- Other techniques (2,3):
  o Particle systems with Billboarded particles ▪ If Particles are updated on the GPU, counts as 2 (Karlos)
  o Gaussian filter in Compute Shader  
  o Bilateral filter in CS (any filter that can be applied in two passes) 
  o Mouse Picking () 
  o Screen-Space Antialiasing 
  o Water-effect (based on vertex or texture animations) 
  o Glow-effect 
  o Make a small game in your application

