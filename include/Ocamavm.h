#pragma once
#include <stdio.h>
#include "mvtpublic.h"

#include <GLES3/gl3.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <memory>
#include <vector>
#include <string>

class OCamRender
{
public:
    typedef void (*RgbaFrameCallback)(int w, int h, int len, const char* data);
    OCamRender();
    ~OCamRender();
    static OCamRender *instance();
    /*前（0）左（1）后（2）右（3）*/
    bool Distortimg(int nCamera, const char* yuv, int w, int h, RgbaFrameCallback callback);
    /*支持手动指定传出分辨率 set outW/outH ****** srcW/srcH：是原图尺寸*/
    bool Distortimg(int nCamera, const char* yuv, int srcW, int srcH, int outW, int outH, RgbaFrameCallback callback);

private:
    EGLConfig eglConf;
    EGLSurface eglSurface;
    EGLContext eglCtx;
    EGLDisplay eglDisp;

    GLuint m_program;
    GLuint vertexShader;
    GLuint pixelShader;

    GLuint m_sTexYHandle;
    GLuint m_sTexUvHandle;
    GLint m_transMatrixHandle;
    
    ModelInfo m_distort;
    ModelInfo m_2DSignalViewtFront;
    ModelInfo m_2DSignalViewtRear;
    ModelInfo m_2DSignalViewtLeft;
    ModelInfo m_2DSignalViewtRight;

    mVTPublic m_signalViewVt;
    glm::mat4 m_leftSignalViewMat;
    glm::mat4 m_rightSignalViewMat;
    GLuint m_vboSingleView[3];
    GLuint m_vaoSingleView;
    GLuint m_fboSingleView;
    GLuint m_imgtextSingleView;//FBO绑定的纹理
    GLuint m_sVideoTex[8];//将传来的摄像头图像绑定为纹理
    std::unique_ptr<char[]> m_sVideoBuffer;//用于存储转换后的NV12图像
    bool m_ready;

    bool ConfigEGL();//初始化OpenGl资源
    GLuint buildShaderProgram(const char* vtxSrc, const char* pxlSrc, const char* name);//生成着色器程序
    GLuint loadShader(GLenum type, const char *shaderSrc, const char *name);//生成着色器
    void CreateVBO();//创建VBO FBO等
    bool Draw(int nCamera,const char* buffer, int srcW, int srcH, int outW, int outH);//传入data和图像的长宽
    void yuvtoNv12(int nCamera,const char* yuv, char* nv12, int w, int h);//将YUYV转换为NV12
    void bindVideoTexture(int cameraSN);//绑定摄像头图像纹理  
};


