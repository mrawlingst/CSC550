# Assignment 04
Due 11/2

Write a complete FBX model loader:
* Allow for a single texture to be loaded as the diffuse color (if specified in the FBX). You can assume we are only supporting JPG files
* Allow for either *Lambert* or *Phong* materials to be specified using both ambient, diffuse, and specular colors defined in the FBX
    * *Note*: This will require you to use different shaders depending on the FBX file
* Load a single FBX using a command-line argument
* Allow the user to change rendering modes using the following keys:
    * **1** - Toggle specular
    * **2** - Toggle texture (this option should be disabled if no texture is defined in FBX)
    * **3** - Set default rendering parameters as defined in the FBX

**HINTS**:
* Create and pass an instance of a shader program in the model class

## Credit
All works are completed by Michael Rawlings and none else unless explicitly stated otherwise.

## License
Copyright &copy; 2018 Michael Rawlings. Licensed under the MIT License, see [License](LICENSE) for more information.
