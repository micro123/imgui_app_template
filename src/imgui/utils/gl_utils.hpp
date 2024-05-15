#ifndef GL_UTILS_HPP
#define GL_UTILS_HPP

#include <core/log.hpp>

#include "imgui/utils/glad.h"

extern "C" FW_API const char* gl_error_str(GLenum code);

#define GL_CLEAR_ERRORS() do { err = glGetError(); } while (err != GL_NO_ERROR)
#define GL_CHECK_ERROR_MSG(FMT, ...) do { err = glGetError(); LC_ERROR(err != GL_NO_ERROR, "GL ERROR %d (%s)! " FMT, err, gl_error_str(err), ##__VA_ARGS__); } while (0)
#define GL_CALL(x) \
    do { \
        GLenum err; \
        GL_CLEAR_ERRORS(); \
        x; \
        GL_CHECK_ERROR_MSG("error occured in code '%s'", #x); \
    } while (0)
    


#endif /* GL_UTILS_HPP */
