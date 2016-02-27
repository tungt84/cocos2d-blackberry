/*
 * OpenGLView.h
 *
 *  Created on: Jan 12, 2016
 *      Author: tungt
 */

#ifndef OPENGLVIEW_H_
#define OPENGLVIEW_H_

#include <stdlib.h>
#include <string>
#include <screen/screen.h>
#include <sys/platform.h>

#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

namespace cocos2d
{
    typedef enum RENDERING_API
    {
        GL_UNKNOWN = 0,
        GL_ES_1 = EGL_OPENGL_ES_BIT,
        GL_ES_2 = EGL_OPENGL_ES2_BIT,
        VG = EGL_OPENVG_BIT
    } RENDERING_API;

    typedef enum VIEW_DISPLAY
    {
        DISPLAY_UNKNOWN, DISPLAY_DEVICE, DISPLAY_HDMI
    } VIEW_DISPLAY;
    class OpenGLView
    {
    public:
        OpenGLView(const std::string & group,const std::string &id,int width,int height);
        virtual ~OpenGLView();
        void setDisplay(VIEW_DISPLAY display);
        void setZ(int z);
        void setScreenEGLDisplay(EGLDisplay egl_disp);
        int initGL();
        int calculateDPI();
        void getGLContext();
        int joinWindowGroup(const std::string& group);
        int setWindowTransparency(int transparency);
        int setWindowPosition(int x, int y);
        void setInitialized(bool initialized);
        int setScreenWindowID(const std::string &id);
        int setWindowSize(int width, int height);
        int setWindowAngle(int angle);
        int setWindowBufferSize(int width, int height);
        int setWindowUsage(int usage);
        int setWindowZ(int z);



        // state
        bool m_enabled;
        bool m_initialized;
        bool m_altered;
        bool m_visible;
        bool m_stale;

        VIEW_DISPLAY m_display;
        RENDERING_API m_api;

        // EGL parameters
        EGLContext m_egl_ctx;
        EGLConfig m_egl_conf;
        EGLDisplay m_egl_disp;
        EGLSurface m_egl_surf;
        int m_usage;
        int m_nbuffers;

        EGLint m_surface_width;
        EGLint m_surface_height;

        // screens / windows
        screen_context_t m_screen_ctx;
        screen_window_t m_screen_win;
        screen_display_t m_screen_disp;
        screen_display_t *m_screen_dpy;
        screen_display_mode_t *m_screen_modes;


        // view properties
        int m_angle;
        int m_x;
        int m_y;
        int m_z;
        int m_width;
        int m_height;
        int m_interval;
        int m_transparency;


        // window group / ID
        std::string m_group;
        std::string m_id;

    };
}

#endif /* OPENGLVIEW_H_ */
