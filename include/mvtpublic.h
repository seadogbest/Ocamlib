#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Ocamconfig.h"

#ifndef QVector3
#define QVector3  glm::vec3
#define QVector3D  glm::vec3
#define QMatrix4x4 glm::mat4
#define QScale  glm::scale
#define QRotate  glm::rotate
#define QTranslate  glm::translate
#define QPerspective glm::perspective
#define QRadians glm::radians
#define QLookAt glm::lookAt
#define QPerspective glm::perspective
#define QValuePtr glm::value_ptr
#endif

class ModelInfo{
public:
    ModelInfo() : m_offset(0), m_size(0){
        glm::mat4 sDefault(1.0f);
        m_mat = sDefault;
    }
    int m_offset;
    int m_size;
    glm::mat4 m_mat;
};

class mVTPublic{
public:
    mVTPublic();
    ~mVTPublic();
    class PosInfo{
    public:
        PosInfo()
        :	m_frontOffset(0),
            m_frontSize(0),
            m_rearOffset(0),
            m_rearSize(0),
            m_leftOffset(0),
            m_leftSize(0),
            m_rightOffset(0),
            m_rightSize(0),

            m_frontVOffset(0),
            m_frontVSize(0),
            m_rearVOffset(0),
            m_rearVSize(0),
            m_leftVOffset(0),
            m_leftVSize(0),
            m_rightVOffset(0),
            m_rightVSize(0),

          m_2DFrontLeftOffset(0),
          m_2DFrontLeftSize(0),
          m_2DFrontRightOffset(0),
          m_2DFrontRightSize(0),
          m_2DRearLeftOffset(0),
          m_2DRearLeftSize(0),
          m_2DRearRightOffset(0),
          m_2DRearRightSize(0),
          m_leftFootPlateOffset(0),
          m_leftFootPlateSize(0),
          m_rightFootPlateOffset(0),
          m_rightFootPlateSize(0){
        }

        void setOffset(PosInfo &info, int offset){
            m_frontOffset = info.m_frontOffset + offset;
            m_frontSize = info.m_frontSize;

            m_rearOffset = info.m_rearOffset + offset;
            m_rearSize = info.m_rearSize;

            m_leftOffset = info.m_leftOffset + offset;
            m_leftSize = info.m_leftSize;

            m_rightOffset = info.m_rightOffset + offset;
            m_rightSize = info.m_rightSize;

            m_centerOffset = info.m_centerOffset + offset;
            m_centerSize = info.m_centerSize;

            m_chassisOffset = info.m_chassisOffset + offset;
            m_chassisSize = info.m_chassisSize;
        }

        int m_frontOffset;
        int m_frontSize;

        int m_rearOffset;
        int m_rearSize;

        int m_leftOffset;
        int m_leftSize;

        int m_rightOffset;
        int m_rightSize;

        int m_frontVOffset;
        int m_frontVSize;

        int m_rearVOffset;
        int m_rearVSize;

        int m_leftVOffset;
        int m_leftVSize;

        int m_rightVOffset;
        int m_rightVSize;

        int m_centerSize;
        int m_centerOffset;

        int m_chassisSize;
        int m_chassisOffset;

        int m_fCalibLineOffset;
        int m_fCalibLineSize;

        int m_chassisFboOffset;
        int m_chassisFboSize;

        int m_frontCameraOffset;
        int m_frontCameraSize;

        int m_rearCameraOffset;
        int m_rearCameraSize;

        int m_leftCameraOffset;
        int m_leftCameraSize;

        int m_rightCameraOffset;
        int m_rightCameraSize;

        int m_leftPathLineOffset;
        int m_leftPathLineSize;

        int m_rightPathLineOffset;
        int m_rightPathLineSize;

        int m_2DFrontLeftOffset;
        int m_2DFrontLeftSize;
        int m_2DFrontRightOffset;
        int m_2DFrontRightSize;

        int m_2DRearLeftOffset;
        int m_2DRearLeftSize;
        int m_2DRearRightOffset;
        int m_2DRearRightSize;

        int m_leftFootPlateOffset;
        int m_leftFootPlateSize;

        int m_rightFootPlateOffset;
        int m_rightFootPlateSize;

        glm::mat4 m_rearLeftMatrix;
        glm::mat4 m_rearRightMatrix;
        glm::mat4 m_frontLeftMatrix;
        glm::mat4 m_frontRightMatrix;
    };
    void undistortPoint(int nViewType,float x, float y, float &u, float &v);
    void undistortPoint(float x, float y, float coeff,float scaleU, float scaleV, \
              float &u, float &v, float offsetV, float offsetH);
    void loadDistort(vector<float> &vertices,
                                    vector<float> &textures, vector<unsigned short> &indices);;
    void loadSignalViewVT(vector<float> &vertices,vector<float> &textures, \
                          vector<unsigned short> &indices,int nCamearID,int nViewType);
    void loadUndistortLeft(vector<float> &vertices,
                        vector<float> &textures, vector<unsigned short> &indices,
                        float coeff);
    void loadUndistortRight(vector<float> &vertices,
                        vector<float> &textures, vector<unsigned short> &indices,
                        float coeff);
    void loadUndistortLeft(vector<float> &vertices,
                        vector<float> &textures, vector<unsigned short> &indices);
    void loadUndistortRight(vector<float> &vertices,
                        vector<float> &textures, vector<unsigned short> &indices);
public:
    void index(vector<unsigned short> &indices,int lines, unsigned short step, \
               unsigned short offset);
    void transformCoor(double M[3], const double rotation[9], const double translation[3]);
    void getTextureUV(double M[3], double m[2], int id);
    void fillData(vector<float> &vertices, vector<float> &textures,\
               float x, float y, float z, float alpha, int id);\
    void fillData(vector<float> &vertices, vector<float> &textures,float x, \
               float y, float z, float alpha, int id,float xl, float yl);
    void fillData(vector<float> &vertices, vector<float> &textures,float x,
               float y, float z, float alpha, int id, \
               float xoffset,float yoffset, float offset);
    void fillData(vector<float> &vertices, vector<float> &textures,\
               float x, float y, float z, float u, float v, float alpha);

private:
    void undistortImg(int nCameraID,int nViewType,vector<float> &vertices,vector<float> &textures, \
                      vector<unsigned short> &indices);
    void get2DViewTailParam(int nViewType,CSigViewTailParam *sViewTailParam);
    void get2DViewVT(int nViewType,float x, float y, float &u, float &v);
    
private:
    float m_wp;
    float m_hp;
    float m_wc;
    float m_hc;
    float m_step;

    float m_carOffsetX;
    float m_carOffsetY;

    float m_panOffsetX;
    float m_panOffsetY;

    float m_2OverLookViewXMinValue;
    float m_2OverLookViewYMinValue;
    float m_2OverLookViewXMaxValue;
    float m_2OverLookViewYMaxValue;

    unsigned short m_nCycleLoopIndex;

    float m_fRadarWidth;
    float m_fRadarHeight;

    float m_area;

    float m_xArea;
    float m_yArea;

    int m_circles;
    float m_delta;
    float m_delta_offset;
    float m_zStartTheta;
    float m_zEndTheta;
    float m_dzTheta;

    float m_xOffsetFront;
    float m_yOffsetFront;
    float m_zOffsetFront;
    float m_xOffsetRear;
    float m_yOffsetRear;
    float m_zOffsetRear;
    float m_fDeltaX;
};



