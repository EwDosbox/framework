################################################################################
# Common Framework for Computer Graphics Courses at FI MUNI.
#
# Copyright (c) Visitlab (https://visitlab.fi.muni.cz)
# All rights reserved.
#
# Course: PV112 (Project Template)
################################################################################

# Generates the lecture.
visitlab_generate_lecture(PV112 project
    EXTRA_FILES
        PlayerCamera.cpp
        PlayerCamera.hpp
        SceneObject.cpp
        SceneObject.hpp
        Utility.cpp
        Utility.hpp
        World.cpp
        World.hpp
)