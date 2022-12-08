# Projects 5 & 6: Lights, Camera & Action!

All project handouts can be found [here](https://cs1230.graphics/projects).

PROJECT 5

DESIGN CHOICES: I would outline 3 significant design choices I made
1) shape.h : I created an abstract class Shape that acts as a parent for shapes classes
used in the project. This choice helps to reduce some code as we treat different shapes
as a single data type.
2) loadHelper.cpp : this file contains functions that are mainly related to controlling
the metadata. VBO and VAO initialization and update functions are located in this class
as well.
3) paintHelper.cpp : this file contains helper functions used in paintGL call. Mostly
they deal with passing uniform data to GLSL program .

PROJECT6
DEFAULT FBO: the default fbo variable is "default_fbo" located in Realtime class.
I initialize the value of the deafult fbo in the method initializeGL() line 70
DESIGN CHOICES: I added a few helper files that contain helper functions
actionHandler.cpp contains helper functions related to the translation and
rotation of the camera.
fboHandler.cpp contains helper functions related to the framebuffer management.
I also modified helper functions in paintHelper.cpp as well as shader programs to 
support all types of lights. Finally created fbo.vert and fbo.frag files in order 
to post-process output image.

EXTRA CREDIT1: implemented grayscale per-pixel filter
EXTRA CREDIT2: implemented gamma per-pixel filter | EFFECT: applies gamma correction(with index 2) e.g.
draws closer representation of colors that our eyes percieve
EXTRA CREDIT3: implemented Laplacian kernel filter | EFFECT: draws color-preserving edges
EXTRA CREDIT4: implemented sharpen kernel filter
+all the perpixel filters are compatible with each other and with a single kernel filter at a time
