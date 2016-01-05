#include <gl/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <list>
#include "CrashReporter.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#define WINDOW_TITLE "Flight Simulator"

#define TEXTURES_SUBDIR "Textures/"
#define SHADERS_SUBDIR "Shaders/"
#define MODELS_SUBDIR "Models/"



#define earthRadius 6378.410
#define PI2  6.28318531