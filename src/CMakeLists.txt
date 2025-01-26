# BSpline lib
add_subdirectory(Utils/BSpline)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
	SET(exe_name cogli2_debug)
ELSE()
	SET(exe_name cogli2)
ENDIF()

SET(cogli2_SOURCES
	Drawables/Drawable.cpp
	Drawables/Scene.cpp
	Drawables/Cylinder.cpp
	Drawables/Icosahedron.cpp
	Drawables/Ellipsoid.cpp
	Drawables/Arrow.cpp
	Drawables/Sphere.cpp
	Drawables/DNA.cpp
	Drawables/DHS.cpp
	Drawables/PatchyParticle.cpp
	Drawables/Shape.cpp
	Drawables/Triangle.cpp
	Drawables/Group.cpp
	Drawers/Drawer.cpp
	Drawers/BlenderDrawer.cpp
	Drawers/OGLDrawer.cpp
	Drawers/PovDrawer.cpp
	Parsers/Parser.cpp
	Parsers/MGLParser.cpp
	Parsers/DNAParser.cpp
	Parsers/ManfredoParser.cpp
	Parsers/IcoParser.cpp
	Parsers/NathanParser.cpp
	Parsers/PatchyParser.cpp
	Parsers/PatchySwapParser.cpp
	Parsers/StarrParser.cpp
	Parsers/ShellParser.cpp
	Parsers/RBCParser.cpp
	Parsers/LevyParser.cpp
	Parsers/TEPParser.cpp
	Parsers/IoxDNAParser.cpp
	Utils/StringHelper.cpp
	Utils/Generic.cpp
	Utils/Logging.cpp
	Utils/MathHelper.cpp
	OGL/OGLManager.cpp
	OGL/Shader.cpp
	OGL/Shapes/Mesh.cpp
	OGL/Shapes/OGLSphere.cpp
	OGL/Shapes/OGLSphericalCap.cpp
	OGL/Shapes/OGLCylinder.cpp
	OGL/Shapes/OGLIcosahedron.cpp
	OGL/Shapes/OGLCone.cpp
	Actions/ActionBuilder.cpp
	Actions/BaseAction.cpp
	Actions/Rotation.cpp
	Actions/Spin.cpp
	Actions/Translation.cpp
	Actions/Visibility.cpp
	povray/PovManager.cpp
	povray/DNARibbon.cpp
	Blender/BlenderManager.cpp
	Camera.cpp
	Lighting.cpp
	Cogli2.cpp
	main.cpp
)

ADD_EXECUTABLE(${exe_name} ${cogli2_SOURCES} ${GLAD_GL})
TARGET_LINK_LIBRARIES(${exe_name} glfw BSpline ${CMAKE_DL_LIBS} ${LIBS})

include(CMakeRC.cmake)
SET(Assets_SOURCES
	OGL/Assets/Shaders/axes.fs
	OGL/Assets/Shaders/axes.vs
	OGL/Assets/Shaders/shape.fs
	OGL/Assets/Shaders/shape.vs
)

cmrc_add_resource_library(assets ${Assets_SOURCES})
TARGET_LINK_LIBRARIES(${exe_name} assets)

# if libpng is installed then we enable exporting in png format
FIND_PACKAGE(PNG)
IF(PNG_FOUND AND NOT NOPNG)
	ADD_DEFINITIONS(-DENABLE_PNG_EXPORT)
	MESSAGE(STATUS "Compiling with libpng support enabled")
	
	TARGET_LINK_LIBRARIES(${exe_name} ${PNG_LIBRARY})
ELSE()
	MESSAGE(STATUS "Compiling with no libpng support")
ENDIF()
