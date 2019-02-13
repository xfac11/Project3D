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
    format (textures). If you use a library like Assimp, it does not count
    _     _
  o Height-map terrain rendering, user can walk on the terrain using WASD keys. 
  
Texturing and Lighting (1): 
    _Daniel_
  o Normal mapping
  
Projection Techniques (1):
    _      _
  o Shadow mapping 
  
Acceleration Techniques (2):
  o View frustum culling against a quadtree 
  o Portal Culling 
  o Front to Back rendering 
  o Back face culling using Geometry Shader
  
Other techniques (2):
  o Particle systems with Billboarded particles 
    ▪ If Particles are updated on the GPU, counts as 2 o Gaussian filter in Compute Shader  
  o Bilateral filter in CS (any filter that can be applied in two passes) 
  o Mouse Picking 
  o Screen-Space Antialiasing o Water-effect (based on vertex or texture animations) 
  o Glow-effect 
  o Make a small game in your application
