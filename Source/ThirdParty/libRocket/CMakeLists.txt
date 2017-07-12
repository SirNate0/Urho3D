# Define target name
set (TARGET_NAME libRocket)

# Define source files
file (GLOB CPP_FILES 
    ./Source/Controls/*.cpp 
    ./Source/Core/*.cpp 
    ./Source/Core/BitmapFont/*.cpp
    ./Source/Core/FreeType/*.cpp 
    ./Source/Core/*.inl 
    ./Source/Debugger/*.cpp 
#    ./Urho3DInterface/*.cpp
)
file (GLOB_RECURSE H_FILES ./Include/Rocket/*.h)
file (GLOB H_FILES2 ./Source/Controls/*.h ./Source/Core/*.h ./Source/Debugger/*.h ./Urho3DInterface/*.h )

set (SOURCE_FILES ${CPP_FILES} ${H_FILES} ${H_FILES2})

# Define dependency libs
set (LIBS ../FreeType/include)

set (INCLUDE_DIRS Include ../FreeType/include)

# Setup target
setup_library ()

# Install headers for building the Urho3D library
install_header_files (DIRECTORY Include/Rocket DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)
#install_header_files (FILES include/ft2build.h DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)
