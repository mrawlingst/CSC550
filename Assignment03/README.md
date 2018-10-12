# Assignment 03
Due 10/12

Develop a Window-based OpenGL program with the following items drawn on screen:
* 6 unique hard-coded arrays of meshes
    * 3 with at least 10 unique vertices each
    * 3 with 4 vertices to define plane each
    * At least 2 meshes are animated (spin, move back and ford, etc.)

Provide keyboard controls:
* Left/Right rotations
* Animations

Provide a fragment shader that shades each mesh using the following items:
* Ambient
* Diffuse
* Specular

Light the meshes with 3 point light sources with each unique color.

**HINTS**
* Use indices to define faces
* Draw each mesh by binding/unbinding in the render function
* Specify an unique transformation (mode_view) matrix for each mesh
* Normals must be calculated and stored in the buffer for use in the fragment shader


## Credit
All works are completed by Michael Rawlings and none else unless explicitly stated otherwise.

## License
Copyright &copy; 2018 Michael Rawlings. Licensed under the MIT License, see [License](LICENSE) for more information.
