/*
用于读取车辆相关数据给vtpublic
包含相机内参外参
车长车宽
*/
#pragma once

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <pthread.h>
#include <chrono>
#include <ios>
#include <iostream>
#include <sstream>


#include <GLES3/gl3.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define QVector vector
using namespace std;

struct CSigViewTailParam{
    float m_coeff;  // 畸变强度系数（控制畸变程度）
    float m_scaleU; // 水平方向输出缩放因子
    float m_scaleV; // 垂直方向输出缩放因子
    float m_offsetV;// 畸变中心点垂直偏移
    float m_offsetH;// 畸变中心点水平偏移
};

enum DisplayType{
    Front_AVM = 1,
    Rear_AVM,
    Left_AVM,
    Right_AVM,
    Front_LR_Wide,
    Rear_LR_Wide,
    Front_Wide,
    Rear_Wide,
    Front_AVM_3D,//前
    Rear_AVM_3D,//后
    Left_AVM_3D,
    Right_AVM_3D,
    Left_Front_3D,
    Left_Rear_3D,
    Right_Front_3D,
    Right_Rear_3D,
    Left_Right_Front_3D ,
    Left_Right_Rear_3D,
};

struct QPoint{
    QPoint(unsigned short nX,unsigned short nY){
        nXValue = nX;
        nyValue = nY;
    }
    unsigned short x(){
        return nXValue;
    }
    unsigned short y(){
        return nyValue;
    }
    unsigned short nXValue;
    unsigned short nyValue;
};

/*相机内参参数*/
struct CameraInnerParam{
    int	length;						// inner length
    int	type_mode;					// type mode
    double pol[32];    // the polynomial coefficients: pol[0] + x"pol[1] + x^2*pol[2] + ... + x^(N-1)*pol[N-1]
    int length_pol;    // length of polynomial
    double invpol[32]; // the coefficients of the inverse polynomial
    int length_invpol; // length of inverse polynomial
    double xc;         // row coordinate of the center
    double yc;         // column coordinate of the center
    double c;          // affine parameter
    double d;          // affine parameter
    double e;          // affine parameter
    int width;         // image width
    int height;        // image height
};

struct CAllCameraInnerParam{
    CameraInnerParam m_ocamModel[4];
};

struct Lookatpa
{
    float lookateyex;
    float lookateyey;
    float lookateyez;
    float lookatcenterx;
    float lookatcentery;
    float lookatcenterz;
    float lookatupx;
    float lookatupy;
    float lookatupz;
};

struct undistort
{
    float offu;
    float offv;
    float scaleu;
    float scalev;
    float coeff;
};

struct leftviewpa
{
    Lookatpa lookatpa;
    float rotateangle;
};

struct rightviewpa
{
    Lookatpa lookatpa;
    float rotateangle;
};

class OCamConfig
{
public:
    OCamConfig();
    ~OCamConfig();
    static OCamConfig* instance();
    CameraInnerParam getCameraInnerParam(int nCameraID);
    void world2cam(double point2D[2], double point3D[3], const struct CameraInnerParam& pOcamModel);
    double* getRotation(int id);
    double* getTranslation(int id);
    const undistort& getFrontviewun();
    const undistort& getRearviewun();
    const leftviewpa& getLeftviewpa();
    const rightviewpa& getRightviewpa();
private:
    static OCamConfig* m_instance;
    CameraInnerParam m_ocams[4];
    double m_rotation[36];//外参旋转矩阵
    double m_translation[12]; //外参平移矩阵
    undistort m_frontviewun;  //前摄畸变系数
    undistort m_rearviewun;   //后摄畸变系数
    leftviewpa m_leftviewpa;  //左摄畸变系数
    rightviewpa m_rightviewpa;//右摄畸变系数
};