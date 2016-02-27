/*
 * OpenGLView.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: tungt
 */

#include "OpenGLView.h"
#include "OpenGLThread.h"
#include <math.h>
#include <string.h>

namespace cocos2d
{
    OpenGLView::OpenGLView(const std::string & group,const std::string &id,int width,int height)
    {
        // initialize members shared by derived classes
        m_nbuffers = 2;
        m_enabled = false;
        m_initialized = false;
        m_stale = false;
        m_altered = false;
        m_visible = false;

        m_screen_win = NULL;
        m_screen_dpy = NULL;

        m_angle = 0.0;
        m_x = 0;
        m_y = 0;
        m_width = width;
        m_height = height;
        m_interval = 0;
        m_transparency = 0;

        this->m_group =  group;
        this->m_id =  id;

        //init by default

        m_api = GL_ES_1;
        OpenGLThread *thread = OpenGLThread::getInstance();
        thread->setRenderingAPI(GL_ES_1);
        this->m_screen_ctx =   thread->m_screen_ctx;
        this->m_egl_conf =  thread->m_egl_conf;
        this->m_egl_disp =  thread->m_egl_disp;
        setDisplay(DISPLAY_DEVICE);
        setZ(-5);
        int returnCode = OpenGLView::initGL();
        if (returnCode == EXIT_SUCCESS) {
            //todo DONE
        }
    }

    OpenGLView::~OpenGLView()
    {

    }
    void OpenGLView::setZ(int z)
    {
        m_z = z;
    }
    void OpenGLView::setDisplay(VIEW_DISPLAY display)
    {
        m_display = display;
        m_egl_disp = OpenGLThread::getInstance()->getDisplay(display);
        setScreenEGLDisplay(m_egl_disp);
    }

    void OpenGLView::setScreenEGLDisplay(EGLDisplay egl_disp)
    {
        m_egl_disp = egl_disp;
    }
    int OpenGLView::initGL()
    {
        int numberDisplays;
        int numberModes;
        int returnCode;
        EGLBoolean status;
        int type;
        EGLint attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
        EGLint attrib_list[] = { EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_NONE };

        // try this first as it will fail if an HDMI display is not attached
        if (m_api == GL_ES_2) {
            m_egl_ctx = eglCreateContext(m_egl_disp, m_egl_conf, EGL_NO_CONTEXT, attributes);
        } else {
            m_egl_ctx = eglCreateContext(m_egl_disp, m_egl_conf, EGL_NO_CONTEXT, NULL);
        }
        if (m_egl_ctx == EGL_NO_CONTEXT) {
            perror("eglCreateContext");
            return EXIT_FAILURE;
        }

        screen_get_context_property_iv(m_screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT,
                &numberDisplays);

        m_screen_dpy = (screen_display_t *) calloc(numberDisplays, sizeof(screen_display_t));
        screen_get_context_property_pv(m_screen_ctx, SCREEN_PROPERTY_DISPLAYS,
                (void **) m_screen_dpy);

        for (int index = 0; index < numberDisplays; index++) {
            int displayID;

            returnCode = screen_get_display_property_iv(m_screen_dpy[index], SCREEN_PROPERTY_ID,
                    (int *) &displayID);
            if (returnCode) {
                perror("display ID");
                return EXIT_FAILURE;
            } else {
                if (displayID == m_display) {
                    screen_get_display_property_iv(m_screen_dpy[index], SCREEN_PROPERTY_TYPE,
                            &type);
                    if (type == SCREEN_DISPLAY_TYPE_HDMI) {
                        returnCode = screen_create_window(&m_screen_win, m_screen_ctx);
                        if (returnCode) {
                            perror("screen_create_window");
                            return EXIT_FAILURE;
                        }
                    } else {
                        returnCode = screen_create_window_type(&m_screen_win, m_screen_ctx,
                                SCREEN_CHILD_WINDOW);
                        if (returnCode) {
                            perror("screen_create_window (child window)");
                            return EXIT_FAILURE;
                        }
                    }
                    if (type == SCREEN_DISPLAY_TYPE_HDMI) {
                        returnCode = screen_set_window_property_pv(m_screen_win,
                                SCREEN_PROPERTY_DISPLAY, (void **) &(m_screen_dpy[index]));
                        if (returnCode) {
                            perror("window display");
                            return EXIT_FAILURE;
                        }
                    }
                }
            }
        }

        //qDebug() << "OpenGLView::initialize: " << m_screen_ctx << ":" << m_egl_disp << ":"
        //        << m_egl_conf << ":" << m_egl_ctx << ":" << m_screen_win;

        int format = SCREEN_FORMAT_RGBA8888;
        returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_FORMAT, &format);
        if (returnCode) {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)");
            return EXIT_FAILURE;
        }

        if (m_transparency > 0) {
            returnCode = setWindowTransparency(m_transparency);
            if (returnCode) {
                perror("transparency");
                return EXIT_FAILURE;
            }
        }

        returnCode = screen_get_window_property_pv(m_screen_win, SCREEN_PROPERTY_DISPLAY,
                (void **) &m_screen_disp);
        if (returnCode) {
            perror("screen_get_window_property_pv");
            return EXIT_FAILURE;
        }

        int angle = atoi(getenv("ORIENTATION"));

        screen_get_display_property_iv(m_screen_disp, SCREEN_PROPERTY_MODE_COUNT, &numberModes);

        m_screen_modes = (screen_display_mode_t *) calloc(numberModes,
                sizeof(screen_display_mode_t));
        returnCode = screen_get_display_property_pv(m_screen_disp, SCREEN_PROPERTY_MODE,
                (void**) m_screen_modes);
        if (returnCode) {
            perror("screen modes");
            return EXIT_FAILURE;
        }

        int dpi = calculateDPI();
        if (dpi == EXIT_FAILURE) {
            fprintf(stderr, "Unable to calculate dpi\n");
            return EXIT_FAILURE;
        }

        returnCode = setWindowPosition(m_x, m_y);
        if (returnCode) {
            perror("window position");
            return EXIT_FAILURE;
        }

        returnCode = setWindowSize(m_width, m_height);
        if (returnCode) {
            perror("window size");
            return EXIT_FAILURE;
        }

        returnCode = setWindowZ(m_z);
        if (returnCode) {
            perror("z order");
            return EXIT_FAILURE;
        }

        returnCode = setWindowBufferSize(m_width, m_height);
        if (returnCode) {
            perror("buffer size");
            return EXIT_FAILURE;
        }

        returnCode = setWindowAngle(m_angle);
        if (returnCode) {
            perror("angle");
            return EXIT_FAILURE;
        }

        returnCode = screen_create_window_buffers(m_screen_win, m_nbuffers);
        if (returnCode) {
            perror("screen_create_window_buffers");
            return EXIT_FAILURE;
        }

        if (m_api == GL_ES_1) {
            m_usage = SCREEN_USAGE_OPENGL_ES1 | SCREEN_USAGE_ROTATION;
        } else if (m_api == GL_ES_2) {
            attrib_list[9] = EGL_OPENGL_ES2_BIT;
            m_usage = SCREEN_USAGE_OPENGL_ES2 | SCREEN_USAGE_ROTATION;
        } else if (m_api == VG) {
            attrib_list[9] = EGL_OPENVG_BIT;
            m_usage = SCREEN_USAGE_OPENVG | SCREEN_USAGE_ROTATION;
        } else {
            fprintf(stderr, "invalid api setting\n");
            return EXIT_FAILURE;
        }

        returnCode = setWindowUsage(m_usage);
        if (returnCode) {
            perror("screen_set_window_property_iv(window usage)");
            return EXIT_FAILURE;
        }

        //qDebug() << "OpenGLView::initGL:eglCreateContext " << m_egl_ctx;
        m_egl_surf = eglCreateWindowSurface(m_egl_disp, m_egl_conf, m_screen_win, NULL);
        if (m_egl_surf == EGL_NO_SURFACE) {
            OpenGLThread::eglPrintError("eglCreateWindowSurface");
            return EXIT_FAILURE;
        }

        getGLContext();

        EGLint interval = 1;
        status = eglSwapInterval(m_egl_disp, interval);
        if (status != EGL_TRUE) {
            OpenGLThread::eglPrintError("eglSwapInterval");
            return EXIT_FAILURE;
        }

        status = eglQuerySurface(m_egl_disp, m_egl_surf, EGL_WIDTH, &m_surface_width);
        if (status != EGL_TRUE) {
            perror("query surface width");
            return EXIT_FAILURE;
        }

        status = eglQuerySurface(m_egl_disp, m_egl_surf, EGL_HEIGHT, &m_surface_height);
        if (status != EGL_TRUE) {
            perror("query surface height");
            return EXIT_FAILURE;
        }

        returnCode = joinWindowGroup(m_group);
        if (returnCode) {
            perror("window group");
            return EXIT_FAILURE;
        }

        returnCode = setScreenWindowID(m_id);
        if (returnCode) {
            perror("window ID");
            return EXIT_FAILURE;
        }

        //qDebug() << "OpenGLView::initGL: " << angle << ":" << numberModes << ":"
        //        << m_screen_modes[0].width << ":" << m_screen_modes[0].height << ":" << m_egl_disp
       //         << ":" << dpi;

        setInitialized(true);

        return EXIT_SUCCESS;
    }
    int OpenGLView::calculateDPI() {
        int returnCode;
        int screen_phys_size[2];

        //qDebug()  << "OpenGLView::calculateDPI: physical: "<< m_egl_disp;

        returnCode = screen_get_display_property_iv(m_screen_disp, SCREEN_PROPERTY_PHYSICAL_SIZE, screen_phys_size);
        if (returnCode) {
            perror("screen_get_display_property_iv");
            return EXIT_FAILURE;
        }

        //Simulator will return 0,0 for physical size of the screen, so use 170 as default dpi
        if ((screen_phys_size[0] == 0) && (screen_phys_size[1] == 0)) {
            return 170;
        } else {
            int screen_resolution[2];

            //qDebug()  << "OpenGLView::calculateDPI: screen: "<< m_egl_disp;

            returnCode = screen_get_display_property_iv(m_screen_disp, SCREEN_PROPERTY_SIZE, screen_resolution);
            if (returnCode) {
                perror("screen_get_display_property_iv");
                return EXIT_FAILURE;
            }

            int diagonal_pixels = sqrt(screen_resolution[0] * screen_resolution[0] + screen_resolution[1] * screen_resolution[1]);
            int diagontal_inches = 0.0393700787 * sqrt(screen_phys_size[0] * screen_phys_size[0] + screen_phys_size[1] * screen_phys_size[1]);
            return (int)(diagonal_pixels / diagontal_inches);
        }
    }
    void OpenGLView::getGLContext()
    {
        EGLBoolean status;

        if (m_egl_ctx != eglGetCurrentContext()) {
            status = eglMakeCurrent(m_egl_disp, m_egl_surf, m_egl_surf, m_egl_ctx);
            if (status != EGL_TRUE) {
                OpenGLThread::eglPrintError("getGLContext (eglMakeCurrent)");
            }
        }
    }
    int OpenGLView::joinWindowGroup(const std::string& group)
    {
        int returnCode = 0;

        if (m_screen_win != NULL) {
            returnCode = screen_join_window_group(m_screen_win, group.c_str());
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }

    int OpenGLView::setWindowTransparency(int transparency)
    {
        int returnCode = 0;

        if (m_screen_win != NULL) {
            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_TRANSPARENCY, &transparency);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    int OpenGLView::setWindowPosition(int x, int y)
    {
        int returnCode;
        int position[2];

        if (m_screen_win != NULL) {
            position[0] = x;
            position[1] = y;

            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_POSITION, position);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    void OpenGLView::setInitialized(bool initialized)
    {
        m_initialized = initialized;
    }
    int OpenGLView::setScreenWindowID(const std::string& id)
    {
        int returnCode = 0;

        if (m_screen_win != NULL) {
            returnCode = screen_set_window_property_cv(m_screen_win, SCREEN_PROPERTY_ID_STRING, strlen( id.c_str()), id.c_str());
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    int OpenGLView::setWindowSize(int width, int height)
    {
        int returnCode;
        int size[2];

        if (m_screen_win != NULL) {
            size[0] = width;
            size[1] = height;

            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_SIZE, size);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    int OpenGLView::setWindowAngle(int angle)
    {
        int returnCode;

        if (m_screen_win != NULL) {
            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_ROTATION, &angle);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    int OpenGLView::setWindowBufferSize(int width, int height)
    {
        int returnCode;
        int size[2];

        if (m_screen_win != NULL) {
            size[0] = width;
            size[1] = height;

            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_BUFFER_SIZE, size);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    int OpenGLView::setWindowUsage(int usage)
    {
        int returnCode = 0;

        if (m_screen_win != NULL) {
            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_USAGE, &usage);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
    int OpenGLView::setWindowZ(int z)
    {
        int returnCode = 0;

        if (m_screen_win != NULL) {
            returnCode = screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_ZORDER, &z);
        } else {
            returnCode = EXIT_SUCCESS;
        }

        return returnCode;
    }
}
