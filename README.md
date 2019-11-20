# Model Loader
A 3D model loader written in C/C++, using OpenGL to render model files.
## Program Setup
This model loader was built in Visual Studio 2019, and as such *must* be built on this version of Visual Studio. Other dependencies for this project are listed below:

Dependency | Version
---------- | ----------
OpenGL     | 4.6.0
GLFW       | 3
GLEW       | 1.2.3.4
GLM        | 0.9.9

Clone and open the Visual Studio Solution for this repo. These dependencies should already be in the NuGet Packages for the project, to install them:
1. Right Click Solution > 'Restore NuGet Packages for the solution'
2. Right Click Project > 'Rescan the project for dependencies'

At this point the project should be ready to build.

## Using the Model loader
Once the model loader is running, you can enter the path to a model file (relative to the project folder). Model file types supported:
* .obj
* .dae

The program will throw an error if the path is invalid, or if there was a problem reading the file.

If the file is valid, the model will be loaded (can take a few minutes for large files) and displayed in the non-console window. You can interact with the model using the following controls:

Action          | Button(s)
----------------|---------
Add Model       | `Keypad +`
Remove Model    | `Keypad -`
Translate Model | `Keypad 8`, `Keypad 6`, `Keypad 2`, `Keypad 4`
Rotate Model    | `Up Arrow`, `Down Arrow`, `Left Arrow`, `Right Arrow`
Select Model    | `F1`, `F2`, `F3`, `F4`, `F5`
Quit Program    | `ESC` (whilst console is not asking for a model)

Actions that modify a model's position/rotation will be executed on the 'active model'. The active model can be chosen by pressing the corresponding F-Key. For instance, pressing F2 will select whichever model was loaded in second, F3- third, F4- fourth, etc.

The models loaded in the program are added onto a stack, so adding and removing models from the scene will do so in a LIFO (Last In, First Out) fashion.

## Project Design
The approach for the program design was to wrap OpenGL methods and functionality within an OOP structure. This encourages code re-use and helps manage the OpenGL state during program runtime.

### 'Model' Structure
The representation of the data OpenGL requires has been set up to enable easy high-level modifications (such as current position, rotation, etc) whilst maintaining low-level differences between mesh data (materials, textures, colours, etc).

#### Model
Encapsulates data that the model needs, such as its position, rotation, scale, etc. Most importantly, this class contains a list of `Object` instances. A `render()` method can be called which will call the render method for each `Object` in the list.

#### Object
This class exists to group meshes together. It has a list of `Mesh` instances. A `render()` method can be called which will call the render method for each `Mesh` in the list.

#### Mesh
The `Mesh` class contains the actual OpenGL resources to render data to the screen. This includes the VAO, VBO, EBO, Texture Setup, as well as interpreting the data to be handed into the current shader. It contains a list of `Vertex` structs which define the mesh data (positional vector, normal, texture coordinate, colour, etc) required to render a given mesh.

The `render()` method will bind the Mesh's VAO, and draw the elements using its EBO.

### Services
Certain OpenGL libraries have been separated and encapsulated away from the main program in their own classes.

#### GLFWServices
`GLFWServices` contains primary GLFW setup and functionality.
* GLFW initialisation
* Window creation
* User Input

#### GLEWServices
`GLEWServices` contains primary GLEW setup and functionality.
* GLEW initialisation

#### ConsoleServices
`ConsoleServices` aims to encapsulate interactions with the console.
* Console formatting
* Printing program header
* Asking for a new model
* Handling errors

### File Loaders
File loaders manage the translation of a given model's file representation into a format that OpenGL can accept and render. The loaders comply to a standard format, only altering how they read & load the files data based on its format (.obj / .dae). To do this, an abstract `ILoader` class exists to govern high-level interactions.

#### ILoader
`ILoader` is an abstract class with little implementation that's primary purpose is to ensure the high-level interactions are consistent across loaders. Some shared functionality such as `verifyFile()` and `getDirectory()` do exist as helper-functions, but `virtual void parse()` provides an interface through which derived classes can perform the data translation.

#### Parsing
Parsing the files is done differently but the result is the same; a `Model` object with the loaded data. The general steps taken are:
1. Parse data sources (positional vertices, texture coordinates, normals) into accessible data structures.
2. Use parsed data sources and file-provided vertex definition indices (a given combination of indexes referencing aforementioned data sources) to generate a list of `Vertex` structs.
3. Generate indices.
4. Load list of `Vertex` structs and indices into a `Mesh` and add to `Object`.
5. Initialise `Mesh` OpenGL resources.
5. If multiple meshes, wipe generated `Vertex` struct list & indices & repeat from step 2.
