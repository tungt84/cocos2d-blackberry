/*
 * OpenGLThread.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: tungt
 */

#include "OpenGLThread.h"
#include <screen/screen.h>
namespace cocos2d
{
    OpenGLThread OpenGLThread::singleton;

    int OpenGLThread::initBPS() {
        //Initialize BPS library
        bps_initialize();
        m_api = GL_ES_1;
        //Create a screen context that will be used to create an EGL surface to to receive libscreen events
        if (EXIT_SUCCESS != screen_create_context(&m_screen_ctx, 0)) {
            fprintf(stderr, "screen_request_events failed\n");
            return EXIT_FAILURE;
        }

        // wait for rendering API to be set
        while (m_api == GL_UNKNOWN) {
            usleep(10);
        };

        // initialize EGL
        if (EXIT_SUCCESS != initEGL()) {
            fprintf(stderr, "initialize EGL failed\n");
            return EXIT_FAILURE;
        }

        //Signal BPS library that screen events will be requested
        if (BPS_SUCCESS != screen_request_events(m_screen_ctx)) {
            fprintf(stderr, "screen_request_events failed\n");
            return EXIT_FAILURE;
        }

        setInitialized(true);

        return EXIT_SUCCESS;
    }

    void OpenGLThread::setInitialized(bool initialized)
    {

        m_initialized = initialized;


    }
    int OpenGLThread::initEGL() {
        int returnCode, type;
        int num_configs;

        m_numberDisplays = 0;
        EGLint attrib_list[]= { EGL_RED_SIZE,        8,
                                EGL_GREEN_SIZE,      8,
                                EGL_BLUE_SIZE,       8,
                                EGL_DEPTH_SIZE,      16,
                                EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
                                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
                                EGL_NONE};

        screen_get_context_property_iv(m_screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &m_numberDisplays);

        m_screen_dpy = (screen_display_t *)calloc(m_numberDisplays, sizeof(screen_display_t));
        screen_get_context_property_pv(m_screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)m_screen_dpy);

        m_egl_disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_egl_disp == EGL_NO_DISPLAY) {
            eglPrintError("eglGetDisplay");
            return EXIT_FAILURE;
        }

        returnCode = eglInitialize(m_egl_disp, NULL, NULL);
        if (returnCode != EGL_TRUE) {
            eglPrintError("eglInitialize");
            return EXIT_FAILURE;
        }

        if ((m_api == GL_ES_1) || (m_api == GL_ES_2)) {
            returnCode = eglBindAPI(EGL_OPENGL_ES_API);
        } else if (m_api == VG) {
            returnCode = eglBindAPI(EGL_OPENVG_API);
        }

        if (returnCode != EGL_TRUE) {
            eglPrintError("eglBindApi");
            return EXIT_FAILURE;
        }

        if(!eglChooseConfig(m_egl_disp, attrib_list, &m_egl_conf, 1, &num_configs)) {
            perror("eglChooseConfig");
            return EXIT_FAILURE;
        }

        setEGLInitialized(true);

        return EXIT_SUCCESS;
    }
    void OpenGLThread::setEGLInitialized(bool initialized)
    {

        m_egl_initialized = initialized;


    }


    void  OpenGLThread::cleanupEGL() {
        //Typical EGL cleanup

        if (m_egl_disp != EGL_NO_DISPLAY) {
            eglTerminate(m_egl_disp);
            m_egl_disp = EGL_NO_DISPLAY;
        }

        if (m_egl_disp_hdmi != EGL_NO_DISPLAY) {
            eglTerminate(m_egl_disp_hdmi);
            m_egl_disp_hdmi = EGL_NO_DISPLAY;
        }

        eglReleaseThread();

        m_initialized = false;
    }

    OpenGLThread::OpenGLThread()
    {
        // TODO Auto-generated constructor stub

    }

    OpenGLThread::~OpenGLThread()
    {
        // TODO Auto-generated destructor stub
    }

    OpenGLThread* OpenGLThread::getInstance()
    {
        //if (!singleton.isRunning() && !singleton.stopped()) {
        //    singleton.start();
        //}
        if(!singleton.m_initialized||!singleton.m_egl_initialized){
            singleton.initBPS();
        }
        return &singleton;
    }
    EGLDisplay OpenGLThread::getDisplay(VIEW_DISPLAY display)
    {
        EGLDisplay egl_display = EGL_NO_DISPLAY;

        while (!eglInitialized()) {
            usleep(1);
        };

        switch (display) {
            case DISPLAY_DEVICE:
                egl_display = m_egl_disp;
                break;

            case DISPLAY_HDMI:
                egl_display = m_egl_disp_hdmi;
                break;
        }

        return egl_display;
    }
    bool OpenGLThread::eglInitialized()
    {
        bool initialized = false;
        initialized = m_egl_initialized;
        return initialized;
    }
    void OpenGLThread::setRenderingAPI(RENDERING_API api)
    {
        m_api = api;

    }
    void OpenGLThread::eglPrintError(const char *msg) {
        static const char *errmsg[] = {
            "function succeeded",
            "EGL is not initialized, or could not be initialized, for the specified display",
            "cannot access a requested resource",
            "failed to allocate resources for the requested operation",
            "an unrecognized attribute or attribute value was passed in an attribute list",
            "an EGLConfig argument does not name a valid EGLConfig",
            "an EGLContext argument does not name a valid EGLContext",
            "the current surface of the calling thread is no longer valid",
            "an EGLDisplay argument does not name a valid EGLDisplay",
            "arguments are inconsistent",
            "an EGLNativePixmapType argument does not refer to a valid native pixmap",
            "an EGLNativeWindowType argument does not refer to a valid native window",
            "one or more argument values are invalid",
            "an EGLSurface argument does not name a valid surface configured for rendering",
            "a power management event has occurred",
        };

        fprintf(stderr, "%s: %s\n", msg, errmsg[eglGetError() - EGL_SUCCESS]);
    }

}
