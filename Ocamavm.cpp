#include "Ocamavm.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tinygltf/stb_image_write.h"

#define WIDTH 1920
#define HEIGHT 1536

static const char vtShader_signalView[] =
        "#version 300 es                                \n"
        "layout(location = 0) in vec4 position;         \n"
        "layout(location = 1) in vec3 texcoor;          \n"
        "uniform mat4 transMatrix;                      \n"
        "out vec3 vTexcoor;                             \n"
        "void main()                                    \n"
        "{                                              \n"
        "   vTexcoor = texcoor;                         \n"
        "   gl_Position =  transMatrix * position;      \n"
        "}                                              \n";
static const char fgShader_signalView[] =
        "#version 300 es                            \n"
        "precision mediump float;                   \n"
        "in vec3 vTexcoor;                          \n"
        "uniform sampler2D SamplerY;                \n"
        "uniform sampler2D SamplerUV;               \n"
        "out vec4 mcolor;                           \n"
        "const mat3 convertMat = mat3( 1.0, 1.0, 1.0, 0.0, -0.39465, 2.03211, 1.13983, -0.58060, 0.0 );\n"
        "void main(void)                            \n"
        "{                                          \n"
        "			vec3 yuv;\n"
        "			yuv.x = texture(SamplerY,  vec2(vTexcoor.x,vTexcoor.y)).r;\n"
        "			yuv.y = texture(SamplerUV, vec2(vTexcoor.x,vTexcoor.y)).r - 0.5;\n"
        "			yuv.z = texture(SamplerUV, vec2(vTexcoor.x,vTexcoor.y)).a - 0.5;\n"
        "			vec3 color = convertMat * yuv;\n"
        "			vec4 mainColor = vec4(color, vTexcoor.z);\n"
        "           mcolor = mainColor;\n"
        "}\n";

static OCamRender *m_OCamRender = nullptr;

OCamRender::OCamRender()
{
    printf("OCamRender constructor");
    eglConf = {0};
    eglSurface = EGL_NO_SURFACE;
    eglCtx = EGL_NO_CONTEXT;
    eglDisp = EGL_NO_DISPLAY;
    m_program = 0;
    vertexShader = 0;
    pixelShader = 0;
    m_sTexYHandle = 0;
    m_sTexUvHandle = 0;
    m_transMatrixHandle = -1;
    m_vaoSingleView = 0;
    m_fboSingleView = 0;
    m_imgtextSingleView = 0;
    memset(m_vboSingleView, 0, sizeof(m_vboSingleView));
    memset(m_sVideoTex, 0, sizeof(m_sVideoTex));
    m_ready = false;

    if(!ConfigEGL())
    {
        printf("ConfigEGL failed");
        return;
    }

    CreateVBO();
    if (m_program == 0) {
        printf("CreateVBO failed");
        return;
    }
    m_sVideoBuffer.reset(new char[WIDTH * HEIGHT * 6]);
    m_ready = true;
}

OCamRender::~OCamRender()
{
    printf("OCamRender destructor");
    m_sVideoBuffer.reset();

    if (m_program){
        glDeleteProgram(m_program);
    }
    
    if (m_vaoSingleView) 
    {
        glDeleteVertexArrays(1, &m_vaoSingleView);
        glDeleteBuffers(3, m_vboSingleView);
    }
    if (m_fboSingleView){
        glDeleteFramebuffers(1, &m_fboSingleView);
    }
    if (m_imgtextSingleView){
        glDeleteTextures(1, &m_imgtextSingleView);
    }
    glDeleteTextures(8, m_sVideoTex);

    if (eglDisp != EGL_NO_DISPLAY) {
        eglMakeCurrent(eglDisp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglCtx != EGL_NO_CONTEXT) {
            eglDestroyContext(eglDisp, eglCtx);
        }
        if (eglSurface != EGL_NO_SURFACE) {
            eglDestroySurface(eglDisp, eglSurface);
        }
        eglTerminate(eglDisp);
    }
}

OCamRender * OCamRender::instance()
{
    if(m_OCamRender == nullptr)
    {
        m_OCamRender = new OCamRender();
    }
    return m_OCamRender;
}

/*初始化EGL资源*/
bool OCamRender::ConfigEGL()
{
    printf("initEglDisplay start");

    EGLint glCtxAttr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,// very important!
        EGL_NONE
    };

    EGLint confAttr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // very important!
        EGL_SURFACE_TYPE,EGL_PBUFFER_BIT, // EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
        EGL_RED_SIZE,	 8,
        EGL_GREEN_SIZE,  8,
        EGL_BLUE_SIZE,	 8,
        EGL_ALPHA_SIZE,  8, // if you need the alpha channel
        EGL_DEPTH_SIZE,  8, // if you need the depth buffer
        EGL_STENCIL_SIZE,8, // if you need the sample buffer
        // EGL_SAMPLE_BUFFERS, 1, // 启用 MSAA
        // EGL_SAMPLES, 4,        // 4x MSAA
        EGL_NONE
    };

    // Pbuffer 的尺寸（只是 EGLSurface 的大小）
    // 真正用来渲染的 FBO 可以是任意尺寸，不一定非要和这里一致
    EGLint pbufferAttr[] = {
        EGL_WIDTH,  1280,   // 你想离屏渲成多大就填多大
        EGL_HEIGHT, 960,
        EGL_NONE
    };
    
    EGLint eglMajVers = 0, eglMinVers = 0;
    eglDisp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisp == EGL_NO_DISPLAY) {
        printf("Unable to open connection to local windowing system");
        return false;
    }

    if (!eglInitialize(eglDisp, &eglMajVers, &eglMinVers)) {
        printf("Unable to initialize EGL");
        return false;
    }


    // 选择一个满足 confAttr 的 EGLConfig
    EGLint numConfigs = 0;
    if (!eglChooseConfig(eglDisp, confAttr, &eglConf, 1, &numConfigs) || numConfigs == 0) {
        printf("eglChooseConfig failed or no configs");
        return false;
    }

    // 用这个 config 创建一个 Pbuffer Surface（离屏）
    eglSurface = eglCreatePbufferSurface(eglDisp, eglConf, pbufferAttr);
    if (eglSurface == EGL_NO_SURFACE) {
        EGLint err = eglGetError();
        printf("eglCreatePbufferSurface failed, err=0x%x", err);
        return false;
    }

    // 创建 GL 上下文
    eglCtx = eglCreateContext(eglDisp, eglConf, EGL_NO_CONTEXT, glCtxAttr);
    if (eglCtx == EGL_NO_CONTEXT) {
        EGLint err = eglGetError();
        printf("eglCreateContext failed, err=0x%x", err);
        return false;
    }

    // 把这个 context + pbuffer 设为当前
    if (!eglMakeCurrent(eglDisp, eglSurface, eglSurface, eglCtx)) {
        printf("eglMakeCurrent failed");
        return false;
    }

    printf("initEglDisplay end");
    return true;

}

GLuint OCamRender::buildShaderProgram(const char* vtxSrc, const char* pxlSrc, const char* name){
    GLuint program = glCreateProgram();
    if (program == 0) {
        printf("Failed to allocate program object\n");
        return 0;
    }

    // Compile the shaders and bind them to this program
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vtxSrc, name);
    if (vertexShader == 0) {
        printf("Failed to load vertex shader\n");
        glDeleteProgram(program);
        return 0;
    }
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pxlSrc, name);
    if (pixelShader == 0) {
        printf("Failed to load pixel shader\n");
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        return 0;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);

    // Link the program
    glLinkProgram(program);
    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        printf("Error linking program.\n");
        GLint size = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
        if (size > 0)
        {
            // Get and report the error message
            std::unique_ptr<char> infoLog(new char[size]);
            glGetProgramInfoLog(program, size, NULL, infoLog.get());
            printf("msg:  %s\n", infoLog.get());
        }

        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(pixelShader);
        return 0;
    }

    return program;
}

GLuint OCamRender::loadShader(GLenum type, const char *shaderSrc, const char *name){
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }

    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printf("Error compiling %s shader for %s\n", (type==GL_VERTEX_SHADER) ? "vtx":"pxl", name);

        GLint size = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        if (size > 0){
            // Get and report the error message
            std::unique_ptr<char> infoLog(new char[size]);
            glGetShaderInfoLog(shader, size, NULL, infoLog.get());
            printf("msg: %s\n", infoLog.get());
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void OCamRender::CreateVBO()
{
    m_program = buildShaderProgram(vtShader_signalView,fgShader_signalView,"Ocamrender");
    if (m_program == 0) {
        return;
    }

    m_sTexYHandle = glGetUniformLocation(m_program, "SamplerY");
    m_sTexUvHandle = glGetUniformLocation(m_program, "SamplerUV");
    m_transMatrixHandle = glGetUniformLocation(m_program, "transMatrix");
    glUniformMatrix4fv(m_transMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_distort.m_mat));

    /*加载反畸变数据*/
    vector<float> m_2DCommonViewVertex;
    vector<float> m_2DCommonViewTexture;
    vector<unsigned short> m_2DComonViewIndex;
    vector<float> m_2DSignalViewVertex;
    vector<float> m_2DSignalViewTexture;
    vector<unsigned short> m_2DSignalViewIndex;

    m_distort.m_offset = m_2DSignalViewIndex.size();
    m_signalViewVt.loadDistort(m_2DSignalViewVertex, m_2DSignalViewTexture, m_2DSignalViewIndex);
    m_distort.m_size = m_2DSignalViewIndex.size() - m_distort.m_offset;
    m_2DSignalViewtFront.m_offset = m_2DSignalViewIndex.size();
    m_signalViewVt.loadSignalViewVT(m_2DSignalViewVertex, m_2DSignalViewTexture, m_2DSignalViewIndex, 1,(int)Front_AVM);
    m_2DSignalViewtFront.m_size = m_2DSignalViewIndex.size() - m_2DSignalViewtFront.m_offset;
    m_2DSignalViewtRear.m_offset = m_2DSignalViewIndex.size();
    m_signalViewVt.loadSignalViewVT(m_2DSignalViewVertex, m_2DSignalViewTexture, m_2DSignalViewIndex, 3,(int)Rear_AVM);
    m_2DSignalViewtRear.m_size = m_2DSignalViewIndex.size() - m_2DSignalViewtRear.m_offset;
    m_2DSignalViewtLeft.m_offset = m_2DSignalViewIndex.size();
    m_signalViewVt.loadUndistortLeft(m_2DSignalViewVertex, m_2DSignalViewTexture, m_2DSignalViewIndex);
    m_2DSignalViewtLeft.m_size = m_2DSignalViewIndex.size() - m_2DSignalViewtLeft.m_offset;
    m_2DSignalViewtRight.m_offset = m_2DSignalViewIndex.size();
    m_signalViewVt.loadUndistortRight(m_2DSignalViewVertex, m_2DSignalViewTexture, m_2DSignalViewIndex);
    m_2DSignalViewtRight.m_size = m_2DSignalViewIndex.size() - m_2DSignalViewtRight.m_offset;
    for(int nIndex = 0;nIndex < m_2DSignalViewVertex.size();nIndex ++)
        m_2DCommonViewVertex.push_back(m_2DSignalViewVertex[nIndex]);
    for(int nIndex = 0;nIndex < m_2DSignalViewTexture.size();nIndex ++)
        m_2DCommonViewTexture.push_back(m_2DSignalViewTexture[nIndex]);
    for(int nIndex = 0;nIndex < m_2DSignalViewIndex.size();nIndex ++)
        m_2DComonViewIndex.push_back(m_2DSignalViewIndex[nIndex]);

    /*左右视图的旋转矩阵*/
    glm::mat4 sDefaultMat(1.0f);
    glm::mat4 m_sRotateSrcMat = sDefaultMat;
    glm::mat4 sRotateMat;//旋转矩阵
    glm::mat4 sLookMat;//视图矩阵 确定虚拟相机位置
    glm::mat4 sPerMat = glm::perspective(glm::radians(60.0f), 4.5f / 3.f, 0.1f, 100.f); //透视投影矩阵

    leftviewpa m_leftviewpa = OCamConfig::instance()->getLeftviewpa();
    rightviewpa m_rightviewpa = OCamConfig::instance()->getRightviewpa();
    sLookMat = glm::lookAt(glm::vec3(m_leftviewpa.lookatpa.lookateyex,m_leftviewpa.lookatpa.lookateyey,m_leftviewpa.lookatpa.lookateyez),\
                  glm::vec3(m_leftviewpa.lookatpa.lookatcenterx,m_leftviewpa.lookatpa.lookatcentery,m_leftviewpa.lookatpa.lookatcenterz),\
                  glm::vec3(m_leftviewpa.lookatpa.lookatupx,m_leftviewpa.lookatpa.lookatupy,m_leftviewpa.lookatpa.lookatupz));
    sRotateMat = glm::rotate(sDefaultMat,glm::radians(m_leftviewpa.rotateangle),glm::vec3(0.0, 0.0, 1.0));  
    m_leftSignalViewMat = m_sRotateSrcMat * sPerMat * sLookMat * sRotateMat;

    sLookMat = glm::lookAt(glm::vec3(m_rightviewpa.lookatpa.lookateyex,m_rightviewpa.lookatpa.lookateyey,m_rightviewpa.lookatpa.lookateyez),\
                  glm::vec3(m_rightviewpa.lookatpa.lookatcenterx,m_rightviewpa.lookatpa.lookatcentery,m_rightviewpa.lookatpa.lookatcenterz),\
                  glm::vec3(m_rightviewpa.lookatpa.lookatupx,m_rightviewpa.lookatpa.lookatupy,m_rightviewpa.lookatpa.lookatupz));
    sRotateMat = glm::rotate(sDefaultMat,glm::radians(m_rightviewpa.rotateangle),glm::vec3(0.0, 0.0, 1.0));
    m_rightSignalViewMat = m_sRotateSrcMat * sPerMat * sLookMat * sRotateMat;

    /*创建VBO*/
    glGenVertexArrays(1, &m_vaoSingleView);
    glGenBuffers(3, m_vboSingleView);

    glBindVertexArray(m_vaoSingleView);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboSingleView[0]);
    glBufferData(GL_ARRAY_BUFFER, m_2DCommonViewVertex.size() * sizeof(GLfloat), m_2DCommonViewVertex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboSingleView[1]);
    glBufferData(GL_ARRAY_BUFFER, m_2DCommonViewTexture.size() * sizeof(GLfloat), m_2DCommonViewTexture.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboSingleView[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_2DComonViewIndex.size() * sizeof(GLushort), m_2DComonViewIndex.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

    /*先创建FBO绑定的纹理*/
    glGenTextures(1, &m_imgtextSingleView);
    glBindTexture(GL_TEXTURE_2D, m_imgtextSingleView);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /*指定纹理大小*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*创建FBO*/
    glGenFramebuffers(1, &m_fboSingleView);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboSingleView);
    /*将纹理绑定到FBO*/
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_imgtextSingleView, 0);
    // 检查FBO完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("FBO error");
    }
    /*解绑*/
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*创建摄像头图像绑定的纹理*/
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(8, m_sVideoTex);
    for (int i = 0; i < 4; i++){
        glBindTexture(GL_TEXTURE_2D, m_sVideoTex[i * 2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, WIDTH, HEIGHT, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);//对应Y的VBO
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, m_sVideoTex[i * 2 + 1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, WIDTH / 2, HEIGHT / 2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, NULL);//对应UV的VBO
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool OCamRender::Draw(int nCamera,const char* buffer, int srcW, int srcH, int outW, int outH)
{  
    if (!m_ready) {
        return false;
    }
    if (buffer == nullptr || srcW <= 0 || srcH <= 0 || outW <= 0 || outH <= 0) {
        return false;
    }
    if (nCamera < 0 || nCamera >= 4) {
        return false;
    }
    if (srcW != WIDTH || srcH != HEIGHT)
    {
        printf("width or height error");
        return false;
    }
    if (outW > WIDTH || outH > HEIGHT) {
        printf("output width or height error");
        return false;
    }

    size_t ySize = static_cast<size_t>(srcW) * static_cast<size_t>(srcH);
    size_t frameSize = ySize + ySize / 2;
    const GLubyte* frameBase = reinterpret_cast<const GLubyte*>(buffer) + static_cast<size_t>(nCamera) * frameSize;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, m_sVideoTex[nCamera * 2]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, srcW, srcH, GL_LUMINANCE, GL_UNSIGNED_BYTE, frameBase);

    glBindTexture(GL_TEXTURE_2D, m_sVideoTex[nCamera * 2 + 1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, srcW / 2, srcH / 2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, frameBase + ySize);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboSingleView);
    glViewport(0, 0, outW, outH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_program);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(m_vaoSingleView);
    bindVideoTexture(nCamera);
    glUniformMatrix4fv(m_transMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_distort.m_mat));
    switch(nCamera)
    {
    case 0://0:front cam
        glDrawElements(GL_TRIANGLES,m_2DSignalViewtFront.m_size,GL_UNSIGNED_SHORT,(const void*)(m_2DSignalViewtFront.m_offset * sizeof(unsigned short)));
        break;
    case 1://1:left cam
        glUniformMatrix4fv(m_transMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_leftSignalViewMat));
        glDrawElements(GL_TRIANGLES,m_2DSignalViewtLeft.m_size,GL_UNSIGNED_SHORT,(const void*)(m_2DSignalViewtLeft.m_offset * sizeof(unsigned short)));
        break;
    case 2://2:rear cam
        glDrawElements(GL_TRIANGLES,m_2DSignalViewtRear.m_size,GL_UNSIGNED_SHORT,(const void*)(m_2DSignalViewtRear.m_offset * sizeof(unsigned short)));
        break;
    case 3://3:right cam
        glUniformMatrix4fv(m_transMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_rightSignalViewMat));
        glDrawElements(GL_TRIANGLES,m_2DSignalViewtRight.m_size,GL_UNSIGNED_SHORT,(const void*)(m_2DSignalViewtRight.m_offset * sizeof(unsigned short)));
        break;
    default:
        printf("nCamera error");
        break;
    }
    
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glFinish();
    return true;
}

void OCamRender::bindVideoTexture(int cameraSN)
{
    glUniform1i(m_sTexYHandle, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sVideoTex[cameraSN * 2]);

    glUniform1i(m_sTexUvHandle, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_sVideoTex[cameraSN * 2 + 1]);
}

void OCamRender::yuvtoNv12(int nCamera,const char* yuv, char* nv12, int w, int h)
{
    (void)nCamera;
    if (yuv == nullptr || nv12 == nullptr || w <= 0 || h <= 0) {
        return;
    }

    const uint8_t* src = reinterpret_cast<const uint8_t*>(yuv);
    uint8_t* dstY = reinterpret_cast<uint8_t*>(nv12);//Y平面起始指针
    uint8_t* dstUV = dstY + static_cast<size_t>(w) * static_cast<size_t>(h);//UV平面起始指针

    const size_t srcStride = static_cast<size_t>(w) * 2;//源数据每一行占多少字节 输入YUYV 每两个2像素占4个字节 Y0 U0 Y1 V1

    /*处理Y平面*/
    for (int row = 0; row < h; ++row) {
        const uint8_t* srcRow = src + static_cast<size_t>(row) * srcStride;//计算每一行的开头索引
        uint8_t* dstRow = dstY + static_cast<size_t>(row) * static_cast<size_t>(w);//计算处于Y平面的位置
        for (int col = 0; col < w; ++col) {
            dstRow[col] = srcRow[static_cast<size_t>(col) * 2];//yuv格式单像素占用2字节
        }
    }

    /*处理UV平面*/
    for (int row = 0; row < h; row += 2) {//降采样 每次采两行
        const uint8_t* srcRow0 = src + static_cast<size_t>(row) * srcStride;
        const uint8_t* srcRow1 = (row + 1 < h) ? (src + static_cast<size_t>(row + 1) * srcStride) : srcRow0;
        uint8_t* dstRow = dstUV + static_cast<size_t>(row / 2) * static_cast<size_t>(w);

        for (int col = 0; col + 1 < w; col += 2) {
            const size_t i = static_cast<size_t>(col) * 2;
            const uint8_t u0 = srcRow0[i + 1];
            const uint8_t v0 = srcRow0[i + 3];
            const uint8_t u1 = srcRow1[i + 1];
            const uint8_t v1 = srcRow1[i + 3];

            dstRow[col] = static_cast<uint8_t>((static_cast<unsigned int>(u0) + static_cast<unsigned int>(u1) + 1u) / 2u);
            dstRow[col + 1] = static_cast<uint8_t>((static_cast<unsigned int>(v0) + static_cast<unsigned int>(v1) + 1u) / 2u);
        }
    }
}

bool OCamRender::Distortimg(int nCamera, const char* yuyv, int w, int h, RgbaFrameCallback callback)
{
    return Distortimg(nCamera, yuyv, w, h, w, h, callback);
}

bool OCamRender::Distortimg(int nCamera, const char* yuyv, int srcW, int srcH, int outW, int outH, RgbaFrameCallback callback)
{
    if (!m_ready) {
        printf("OCamRender not ready");
        return false;
    }

    if (yuyv == nullptr || callback == nullptr || srcW <= 0 || srcH <= 0 || outW <= 0 || outH <= 0) {
        printf("error buffer");
        return false;
    }
    if (nCamera < 0 || nCamera >= 4) {
        printf("error CameraID");
        return false;
    }
    if (srcW != WIDTH || srcH != HEIGHT)
    {
        printf("width or height error");
        return false;
    }
    if (outW > WIDTH || outH > HEIGHT) {
        printf("output width or height error");
        return false;
    }

    size_t ySize = static_cast<size_t>(srcW) * static_cast<size_t>(srcH);
    size_t frameSize = ySize + ySize / 2;
    char* nv12Frame = m_sVideoBuffer.get() + static_cast<size_t>(nCamera) * frameSize;
    yuvtoNv12(nCamera, yuyv, nv12Frame, srcW, srcH);
    if (!Draw(nCamera, m_sVideoBuffer.get(), srcW, srcH, outW, outH)) {
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboSingleView);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    std::vector<uint8_t> cpuBuffer(static_cast<size_t>(outW) * static_cast<size_t>(outH) * 4u);
    glReadPixels(0, 0, outW, outH, GL_RGBA, GL_UNSIGNED_BYTE, cpuBuffer.data());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /*做图片翻转*/
    std::vector<uint8_t> flipped(cpuBuffer.size());
    const size_t stride = static_cast<size_t>(outW) * 4u;
    for(int y = 0; y< outH; ++y)//逐行拷贝
    {   
        const size_t src = static_cast<size_t>(outH - 1 - y) * stride;
        const size_t dst = static_cast<size_t>(y) * stride;
        memcpy(flipped.data() + dst, cpuBuffer.data() + src, stride);
    }

    callback(outW, outH, static_cast<int>(flipped.size()), reinterpret_cast<const char*>(flipped.data()));
    return true;
}
