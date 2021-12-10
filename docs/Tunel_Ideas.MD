======================= 11 aug 21 ========================= <br>
----- Topic: Creating programs for interpolation geometry of tunnel <br>
Some plan for the coming days. <br>

"Crappy plan is always better when no plan at all"(ñ)Kvazikot.
1. Create Mesh of cylynder from primitives. If Mesh from unity primitives is not adjustible in terms of number of segments.
Create mesh of a cylinder from scratch using polar coordinates along spline
or just straight line.


2. How to make interpolation of a geometry of a tunel in real time?
3. Use cylinder primitive created in 1. section of this plan, 
4. Create vertex shader program on cg language from template let say checkboard (https://docs.unity3d.com/520/Documentation/Manual/SL-VertexFragmentShaderExamples.html )
5. Test simple sinus wave on a cylinder mesh with time as a uniform variable  
6. Pass control points of a Bezie spline to vertex shader through uniform variables     Or we can pass control points as a mesh data.
7. To interpolate vertexes in shader we need to know the index of a vertex and current time<br/> Therefore we need to create second layer of uv-coordinates to the mesh.OR simply use z coordinate for that purpose
8. Interpolate control points of a bezie Spline in .NET code 
9. Interpolate control points of a spline according to sin wave.  
10. Make shure that its working at least on a level of Gizmo Drawing
11. Create class in c# to encapsulate parametres for tunnel animation
    List of parametres: time of whole animation cycle, <br>
                        function for geometry interpolation. <br>
                        Fractal parametres <br>
                        List of transformations from one state to another. <br>
                        Make this class as a state machine. <br>
                        Class of animation state machine can be general as possible. <br>
                        <br>
     
     

======================= 10 aug 21 ========================= <br>
----- Topic: Brick walls. Deterministic chaos<br>
I found answer to the arrow of time problem.
Its not specific to unity althrou is a issue of laws determenistic chaos.
We can store every transform matrix state for every brick of the wall.
At any given moment of time. 
But to reverse time backwards we need more memory.
This is why i need more memory on laptop or i can limit the length of the tunnel.
We need only FOV of camera, from far and near planes.

----- Topic: Fractal walls<br>
In this site  http://www.josleys.com/index.php <br>
Specificaly this paper http://www.josleys.com/articles/JosLeyDelahayeV2.pdf <br>
describes fractal tunnels in Esher style.
i found a useful shader programs of how it can be done.
I can decorate the walls with fractals it can look cool in certain segments of the tunnel.
Also in this page i found examples of science visualisation 
specificaly on chaos teory and explanation.
I not sure how you can make money on fractals but never the less.
https://www.chaos-math.org/en.html


======================= 09 aug 21 ========================= <br>
----- Topic: New ideas. Tunnel animation. <br>
All scene should be in style of "Alice in wonderland"
or visualise surrealistic mathematical paradoxes that cannot exist 
in the real world. But can be in some hypotetical world in the multiverse.
1. Bricks and arrow of time
You can animate the bricks from which the tunnel is made.
You can give a random impulse to each brick component
tunnel walls AddForce. Each brick uses bump mapping.
But each brick must build interesting figures in 
Escher style of different detail.
 
--- Questions: But one issues arises here, namely,
--- reverse the arrow of time of Unity physics engine's ?
How to reverse time in physX simulation in Unity engine?
  
2. The entrance to the tunnel can be in the form of a snake or dinosaur head.
The camera can fly into the tunnel (flying car)


====================== 08 aug 21 ========================= <br>
----- Topic: Implementation of Bezie splines in Tunnel project <br>
Make the point look like a textured quad instead of a sphere.
This will reduce the number of cycles wasted on unnecessary geometry.
Add the ability to build Bezier splines, two more control points.
De Casteljau's algorithm.
The interpolation formula is on Wikipedia. Matrices are not needed there.
Hermitian splines can be retained, or all points can be interpolated with a cubic spline.

----- Topic: Implementation of Bezie splines <br>
1. Create a BezieKnot control point class
Control points are red.
Prepare a texture for a quad point or take a gradient texture
You can write a shader program to do this.
Lines from control points can be displayed dotted line
The spline is colored red.
2. Create a class BezieSpline.
This method creates a point by instantiating prefarb.
How many knots are there maximum?
Transfer point cache logic to BezieSpline.
- Checkpoints can only be shown for the selected segment.
3. The spline interface implements the BezieSplineEditor class with an AddKnot button.
4. For Bezie spline use casteljau algorithm
