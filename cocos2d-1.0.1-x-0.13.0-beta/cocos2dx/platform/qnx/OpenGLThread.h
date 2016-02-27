/*
 * OpenGLThread.h
 *
 *  Created on: Jan 12, 2016
 *      Author: tungt
 */

#ifndef OPENGLTHREAD_H_
#define OPENGLTHREAD_H_
#include "OpenGLView.h"
#include "OpenGLThread.h"
#include <assert.h>
#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
namespace cocos2d
{
    class OpenGLThread
    {
    public:
        OpenGLThread();
        static OpenGLThread *getInstance();
        virtual ~OpenGLThread();
        EGLDisplay getDisplay(VIEW_DISPLAY display);
        bool eglInitialized();
        void setRenderingAPI(RENDERING_API api);
        static void eglPrintError(const char *msg);
        int initBPS();
        int initEGL() ;
        void  cleanupEGL();
        void setEGLInitialized(bool initialized);
        void setInitialized(bool initialized);

        static OpenGLThread singleton;
        EGLDisplay m_egl_disp;
        EGLDisplay m_egl_disp_hdmi;
        EGLConfig m_egl_conf;
        EGLContext m_egl_ctx;
        int m_usage;

        screen_context_t m_screen_ctx;
        screen_display_t *m_screen_dpy;

        bool m_initialized;
        bool m_egl_initialized;
        bool m_stopped;


        int m_numberDisplays;
        RENDERING_API m_api;

    };
}

#endif /* OPENGLTHREAD_H_ */
