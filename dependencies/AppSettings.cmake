## description: simple library to store application settings
cmake_minimum_required(VERSION 3.1.0)
include(FetchContent)

# Define the git repository and tag to download from
set(LIB_NAME AppSettings)
set(GIT_REPO https://github.com/KROIA/AppSettings.git)
set(GIT_TAG main)

FetchContent_Declare(
    ${LIB_NAME}
    GIT_REPOSITORY ${GIT_REPO}
    GIT_TAG        ${GIT_TAG}
)

set(AppSettings_NO_SAMPLES True)
set(AppSettings_NO_UNITTTESTS True)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build AppSettings as static library.")
message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
FetchContent_MakeAvailable(${LIB_NAME})


# Add this library to the specific profiles of this project
list(APPEND DEPENDENCIES_FOR_SHARED_LIB AppSettings_static)
list(APPEND DEPENDENCIES_FOR_STATIC_LIB AppSettings_static)
list(APPEND DEPENDENCIES_FOR_STATIC_PROFILE_LIB AppSettings_static_profile) # only use for static profiling profile
