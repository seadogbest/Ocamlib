#include "mvtpublic.h"

mVTPublic::mVTPublic(){
    //m_ocams = OCamConfig::instance();
}

mVTPublic::~mVTPublic(){

}

void mVTPublic::loadDistort(vector<float> &vertices,
                    vector<float> &textures, vector<unsigned short> &indices){
    unsigned short offset = vertices.size() / 3;
    fillData(vertices, textures, -1.f, 1.f, 0.f, 0.f, 0.f, 1.f);
    fillData(vertices, textures, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f);
    fillData(vertices, textures, -1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
    fillData(vertices, textures, 1.f, -1.f, 0.f, 1.f, 1.f, 1.f);

    index(indices, 2, (unsigned short)2, offset);
}

void mVTPublic::loadSignalViewVT(vector<float> &vertices,
                    vector<float> &textures, vector<unsigned short> &indices,
                    int nCameraID,int nViewType){
    undistortImg(nCameraID,nViewType,vertices, textures, indices);
}


void mVTPublic::loadUndistortLeft(vector<float> &vertices,
                    vector<float> &textures, vector<unsigned short> &indices,
                    float coeff){
    unsigned short offset = vertices.size() / 3;
    unsigned short step, lines = 0;

    float xStart,xEnd, yStart, yEnd, x, y, alpha;
    int id = 2;
    bool break_x, break_y;
    float fStep = 0.1f;

    xStart = -12.f;
    xEnd = 0.2f;
    yStart = 12.f ;
    yEnd = -1;

    alpha = 1.f;
    lines = 0;
    break_y = false;
    break_x = false;
    for (y = yStart; y >= yEnd; y -= fStep){
        lines ++;
        if (y < yEnd){
            y = yEnd;
            break_y = true;
        }

        break_x = false;
        for (x = xEnd; x >= xStart; x -= fStep){
            if (x < xStart){
                x = xStart;
                break_x = true;
            }
            fillData(vertices, textures, x ,y , 0.f, alpha, id, 1.75f,  -3.15f, 0.f);
            if (break_x)
                break;
        }

        if (break_y)
            break;
    }

    step = (vertices.size() / 3 - offset) / lines;
    index(indices, lines, step, offset);
}


void mVTPublic::loadUndistortLeft(vector<float> &vertices,
                    vector<float> &textures, vector<unsigned short> &indices){
    unsigned short offset = vertices.size() / 3;
    unsigned short step, lines = 0;

    float xStart,xEnd, yStart, yEnd, x, y, alpha;
    int id = 2;
    bool break_x, break_y;
    float fStep = 0.1f;

    xStart = -12.f;
    xEnd = 0.2f;
    yStart = 12.f ;
    yEnd = -1;

    alpha = 1.f;
    lines = 0;
    break_y = false;
    break_x = false;
    for (y = yStart; y >= yEnd; y -= fStep){
        lines ++;
        if (y < yEnd){
            y = yEnd;
            break_y = true;
        }

        break_x = false;
        for (x = xEnd; x >= xStart; x -= fStep){
            if (x < xStart){
                x = xStart;
                break_x = true;
            }
            fillData(vertices, textures, x ,y , 0.f, alpha, id, 1.75f,  -3.15f, 0.f);
            if (break_x)
                break;
        }

        if (break_y)
            break;
    }

    step = (vertices.size() / 3 - offset) / lines;
    index(indices, lines, step, offset);
}


void mVTPublic::loadUndistortRight(vector<float> &vertices,
                    vector<float> &textures, vector<unsigned short> &indices,
                    float coeff){
    unsigned short offset = vertices.size() / 3;
    unsigned short step, lines = 0;

    float xStart,xEnd, yStart, yEnd, x, y, alpha;
    int id = 0;
    bool break_x, break_y;
    float fStep = 0.1f;

    xStart = -0.6f;
    xEnd = 12.f;
    yStart = 12.f ;
    yEnd = -1.0f;

    alpha = 1.f;
    lines = 0;
    break_y = false;
    for (y = yStart; y >= yEnd; y -= fStep){
        lines ++;
        if (y < yEnd){
            y = yEnd;
            break_y = true;
        }
        break_x = false;
        for (x = xEnd; x >= xStart; x -= fStep){
            if (x < xStart){
                x = xStart;
                break_x = true;
            }
            fillData(vertices, textures, x ,y , 0.f, alpha, id, -1.765f,  -3.15f, 0.f);
            if (break_x)
                break;
            if (break_x)
                break;
        }
        if (break_y)
            break;
    }
    step = (vertices.size() / 3 - offset) / lines;
    index(indices, lines, step, offset);
}


void mVTPublic::loadUndistortRight(vector<float> &vertices,
                    vector<float> &textures, vector<unsigned short> &indices){
    unsigned short offset = vertices.size() / 3;
    unsigned short step, lines = 0;

    float xStart,xEnd, yStart, yEnd, x, y, alpha;
    int id = 0;
    bool break_x, break_y;
    float fStep = 0.1f;

    xStart = -0.6f;
    xEnd = 12.f;
    yStart = 12.f ;
    yEnd = -1.0f;

    alpha = 1.f;
    lines = 0;
    break_y = false;
    for (y = yStart; y >= yEnd; y -= fStep){
        lines ++;
        if (y < yEnd){
            y = yEnd;
            break_y = true;
        }
        break_x = false;
        for (x = xEnd; x >= xStart; x -= fStep){
            if (x < xStart){
                x = xStart;
                break_x = true;
            }
            fillData(vertices, textures, x ,y , 0.f, alpha, id, -1.765f,  -3.15f, 0.f);
            if (break_x)
                break;
            if (break_x)
                break;
        }
        if (break_y)
            break;
    }
    step = (vertices.size() / 3 - offset) / lines;
    index(indices, lines, step, offset);
}


void mVTPublic::fillData(vector<float> &vertices, vector<float> &textures,
                    float x, float y, float z, float alpha, int id){
    double M[3], m[2];
    M[0] = x;
    M[1] = y;
    M[2] = z;
    getTextureUV(M, m, id);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    textures.push_back(m[0]);
    textures.push_back(m[1]);
    textures.push_back(alpha);
}

void mVTPublic::fillData(vector<float> &vertices, vector<float> &textures,
                    float x, float y, float z, float alpha, int id,
                    float xl, float yl){
    double M[3], m[2];
    M[0] = x;
    M[1] = y;
    M[2] = z;
    getTextureUV(M, m, id);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    textures.push_back(m[0]);
    textures.push_back(m[1]);
    textures.push_back(alpha);
}

void mVTPublic::fillData(vector<float> &vertices, vector<float> &textures,
                                 float x, float y, float z, float alpha, int id, float xoffset,float yoffset, float offset){
    double M[3], m[2];
    M[0] = x;
    M[1] = y;
    M[2] = z;

    getTextureUV(M, m, id);
    vertices.push_back(x + xoffset);
    vertices.push_back(y + yoffset);
    vertices.push_back(z);

    textures.push_back(m[0]);
    textures.push_back(m[1]);
    textures.push_back(alpha);
}

void mVTPublic::fillData(vector<float> &vertices, vector<float> &textures,
                    float x, float y, float z, float u, float v, float alpha){
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    textures.push_back(u);
    textures.push_back(v);
    textures.push_back(alpha);
}

void mVTPublic::index(vector<unsigned short> &indices,
                        int lines, unsigned short step, unsigned short offset){
    int i, j;
    unsigned short startPos, nextStartPos;
    for (j = 0; j < lines - 1; ++j){
        startPos = j * step + offset;
        nextStartPos = (j + 1) * step + offset;
        for (i = 0; i < step - 1; ++i){
            indices.push_back(startPos + i);
            indices.push_back(startPos + i + 1);
            indices.push_back(nextStartPos + i);

            indices.push_back(nextStartPos + i);
            indices.push_back(startPos + i + 1);
            indices.push_back(nextStartPos + i + 1);
        }
    }
}

void mVTPublic::undistortImg(int nCameraID,int nViewType,vector<float> &vertices,vector<float> &textures, vector<unsigned short> &indices){
    float x, y, u, v, delta;

    unsigned short offset = vertices.size() / 3;
    unsigned short step, lines = 0;
    delta = 0.05f;

    /*
        x,y为原始图像的uv坐标 范围从0-1 采样为0.05
    */
    for (y = 0.f; y < 1.f + delta; y += delta){
        lines ++;

        for (x = 0.f; x < 1.f + delta; x += delta){
            undistortPoint(nViewType,x, y, u, v);//得到反畸变后的uv

            /*
                归一化为Opengl的标准纹理坐标 前面的是顶点坐标（归一化后）
                x y为传入undistortPoint的原始图像的uv坐标 塞入纹理坐标数组
                同时设置好了透明度
                经过下面的操作 理论上bind后draw出来的图像 是基于原始图像的去径向畸变
            */
            if (nCameraID == 1) {
                fillData(vertices, textures,(u * 2.f - 1.0f), (1.f - v * 2.0), 0.5f,x, y , 1.f);
            }
            else if (nCameraID == 3) {
                fillData(vertices, textures,u * 2.f - 1.0f, 1.f - v * 2.0, 0.5f,1.f - x, y , 1.f);
            }
        }
    }
    step = (vertices.size() / 3 - offset) / lines;
    index(indices, lines, step, offset);
}


void mVTPublic::undistortPoint(int nViewType,float x, float y, float &u, float &v){
    get2DViewVT(nViewType,x,y,u,v);
}

void mVTPublic::undistortPoint(float x, float y, float coeff,
                float scaleU, float scaleV, float &u, float &v, float offsetV, float offsetH){
    float radius = sqrt(offsetV * offsetV + offsetH * offsetH);
    float angle = atanf((y - offsetV) / (x - offsetH));
    float r1 = sqrt((x - offsetH) * (x - offsetH) + (y - offsetV) * (y - offsetV));
    float r = (1.f - coeff * r1 / radius) * r1;
    if (x < offsetH){
        u = 0.5f - cosf(angle) * r * scaleU;
        v = 0.5f - sinf(angle) * r * scaleV;
    }
    else{
        u = 0.5f + cosf(angle) * r * scaleU;
        v = 0.5f + sinf(angle) * r * scaleV;
    }
}

void mVTPublic::getTextureUV(double M[3], double m[2], int id){
    struct CameraInnerParam ocam =  OCamConfig::instance()->getCameraInnerParam(id);
    int width = ocam.width;
    int height = ocam.height;
    transformCoor(M,  OCamConfig::instance()->getRotation(id),  OCamConfig::instance()->getTranslation(id));
    OCamConfig::instance()->world2cam(m, M, ocam);
    m[0] = m[0] / (double)width;
    m[1] = m[1] / (double)height;
}

void mVTPublic::transformCoor(double M[3], const double rotation[9], const double translation[3]){
    double tmp[3] = {M[0], M[1], M[2]};

    M[0] = rotation[0] * tmp[0] + rotation[1] * tmp[1] + rotation[2] * tmp[2] + translation[0];
    M[1] = rotation[3] * tmp[0] + rotation[4] * tmp[1] + rotation[5] * tmp[2] + translation[1];
    M[2] = rotation[6] * tmp[0] + rotation[7] * tmp[1] + rotation[8] * tmp[2] + translation[2];

    M[0] = -M[0];
    M[1] = -M[1];
    M[2] = -M[2];
}

void mVTPublic::get2DViewVT(int nViewType,float x, float y, float &u, float &v){
    CSigViewTailParam sViewTailParam;
    get2DViewTailParam(nViewType,&sViewTailParam);
    float radius = sqrt(sViewTailParam.m_offsetV * sViewTailParam.m_offsetV + sViewTailParam.m_offsetH * sViewTailParam.m_offsetH); //计算参考半径
    float angle = atanf((y - sViewTailParam.m_offsetV) / (x - sViewTailParam.m_offsetH));
    float r1 = sqrt((x - sViewTailParam.m_offsetH) * (x - sViewTailParam.m_offsetH) + (y - sViewTailParam.m_offsetV) * (y - sViewTailParam.m_offsetV));// 计算当前点到中心的距离
    /*
    核心畸变公式 r = r₁ - (k/R) × r₁²
    m_coeff > 0 产生枕形畸变
    m_coeff < 0 产生桶形畸变
    */
    float r = (1.f - sViewTailParam.m_coeff * r1 / radius) * r1;
    if (x < sViewTailParam.m_offsetH){
        u = 0.5f - cosf(angle) * r * sViewTailParam.m_scaleU;
        v = 0.5f - sinf(angle) * r * sViewTailParam.m_scaleV;
    }
    else{
        u = 0.5f + cosf(angle) * r * sViewTailParam.m_scaleU;
        v = 0.5f + sinf(angle) * r * sViewTailParam.m_scaleV;
    }
}

void mVTPublic::get2DViewTailParam(int nViewType,CSigViewTailParam *sViewTailParam){
    undistort ust;
    switch(nViewType){
    case (int)Front_AVM:
        ust = OCamConfig::instance()->getFrontviewun();
        sViewTailParam->m_coeff = ust.coeff;
        sViewTailParam->m_scaleU = ust.scaleu;
        sViewTailParam->m_scaleV = ust.scalev;
        sViewTailParam->m_offsetV = ust.offv;
        sViewTailParam->m_offsetH = ust.offu;
        break;
    default:
        ust = OCamConfig::instance()->getRearviewun();
        sViewTailParam->m_coeff = ust.coeff;
        sViewTailParam->m_scaleU = ust.scaleu;
        sViewTailParam->m_scaleV = ust.scalev;
        sViewTailParam->m_offsetV = ust.offv;
        sViewTailParam->m_offsetH = ust.offu;
        break;
    }
}