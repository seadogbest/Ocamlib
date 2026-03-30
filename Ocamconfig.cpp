#include "Ocamconfig.h"

OCamConfig* OCamConfig::m_instance = nullptr;

OCamConfig::OCamConfig()
{
    memset(m_ocams, 0, sizeof(m_ocams));

    /*0:前摄像头*/
    const double pol_front[] = {
        -4.364967e+02, 0.000000e+00, 4.318341e-04, 3.460713e-08, 2.769236e-10
    };
    const double invpol_front[] = {
        821.327900, 589.564500, 14.525380, 47.886800, 74.542320, -5.710168, 4.028820, 24.479370, -8.726432, 8.412562,
        14.908330, -36.260920, -22.656940, 48.115420, 47.980660, -10.451710, -36.272510, -21.940470, -5.907285, -0.624337
    };

    m_ocams[0].length = 5;
    m_ocams[0].type_mode = 0;
    m_ocams[0].length_pol = 5;
    m_ocams[0].length_invpol = 20;
    memcpy(m_ocams[0].pol, pol_front, sizeof(pol_front));
    memcpy(m_ocams[0].invpol, invpol_front, sizeof(invpol_front));
    m_ocams[0].yc = 764.407382;
    m_ocams[0].xc = 953.564516;
    m_ocams[0].c = 1.000000;
    m_ocams[0].d = 0.000000;
    m_ocams[0].e = 0.000000;
    m_ocams[0].height = 1536;
    m_ocams[0].width = 1920;

    /*1:左摄像头*/
    const double pol_left[] = {
        -4.387000e+02, 0.000000e+00, 4.305395e-04, 3.432665e-08, 2.714038e-10
    };
    const double invpol_left[] = {
        825.475400, 593.177900, 15.338010, 47.938380, 74.889860, -5.364442, 3.849188, 24.583350, -8.562654, 7.956693,
        15.079060, -35.202740, -22.722540, 46.713810, 47.244430, -9.875424, -35.431670, -21.526400, -5.810395, -0.615272
    };

    m_ocams[1].length = 5;
    m_ocams[1].type_mode = 0;
    m_ocams[1].length_pol = 5;
    m_ocams[1].length_invpol = 20;
    memcpy(m_ocams[1].pol, pol_left, sizeof(pol_left));
    memcpy(m_ocams[1].invpol, invpol_left, sizeof(invpol_left));
    m_ocams[1].yc = 770.525140;
    m_ocams[1].xc = 966.682972;
    m_ocams[1].c = 1.000000;
    m_ocams[1].d = 0.000000;
    m_ocams[1].e = 0.000000;
    m_ocams[1].height = 1536;
    m_ocams[1].width = 1920;

    /*2:后摄像头*/
    const double pol_rear[] = {
        -4.365937e+02, 0.000000e+00, 4.340960e-04, 3.746774e-08, 2.697636e-10
    };
    const double invpol_rear[] = {
        821.504100, 592.153300, 17.688300, 47.406160, 74.291890, -4.174455, 3.414627, 24.231130, -7.807774, 6.638776,
        14.853850, -31.547310, -21.818780, 41.989420, 43.702990, -8.386042, -32.245990, -19.752170, -5.354852, -0.568823
    };

    m_ocams[2].length = 5;
    m_ocams[2].type_mode = 0;
    m_ocams[2].length_pol = 5;
    m_ocams[2].length_invpol = 20;
    memcpy(m_ocams[2].pol, pol_rear, sizeof(pol_rear));
    memcpy(m_ocams[2].invpol, invpol_rear, sizeof(invpol_rear));
    m_ocams[2].yc = 758.172818;
    m_ocams[2].xc = 974.343575;
    m_ocams[2].c = 1.000000;
    m_ocams[2].d = 0.000000;
    m_ocams[2].e = 0.000000;
    m_ocams[2].height = 1536;
    m_ocams[2].width = 1920;

    /*3:右摄像头*/
    const double pol_right[] = {
        -4.351003e+02, 0.000000e+00, 4.240357e-04, 6.729806e-08, 2.561704e-10
    };
    const double invpol_right[] = {
        817.883700, 588.705900, 20.283880, 50.065610, 72.312690, -3.385525, 5.175244, 22.320960, -7.065831, 7.896407,
        12.087970, -30.251320, -17.091860, 39.262420, 36.800190, -8.978092, -27.495210, -16.034730, -4.184829, -0.429234
    };

    m_ocams[3].length = 5;
    m_ocams[3].type_mode = 0;
    m_ocams[3].length_pol = 5;
    m_ocams[3].length_invpol = 20;
    memcpy(m_ocams[3].pol, pol_right, sizeof(pol_right));
    memcpy(m_ocams[3].invpol, invpol_right, sizeof(invpol_right));
    m_ocams[3].yc = 754.683109;
    m_ocams[3].xc = 954.061199;
    m_ocams[3].c = 1.000000;
    m_ocams[3].d = 0.000000;
    m_ocams[3].e = 0.000000;
    m_ocams[3].height = 1536;
    m_ocams[3].width = 1920;

    /*外参矩阵*/
    /*由bin文件直接解析而来 48个double参数 48*8 = 384*/
    const unsigned char kExtrinsicBin[] = {
        0x68, 0xa9, 0x76, 0x13, 0x1c, 0x5e, 0xb4, 0xbf, 0x1f, 0xab, 0xca, 0x73, 
        0x5d, 0xe3, 0xef, 0x3f, 0x40, 0xfd, 0xab, 0xee, 0x21, 0x15, 0x9a, 0x3f, 
        0x3d, 0xda, 0xec, 0xb2, 0x5a, 0xdd, 0xe6, 0x3f, 0x70, 0x3e, 0x9b, 0x7e, 
        0x18, 0x11, 0xa4, 0x3f, 0xa1, 0xf4, 0x78, 0x42, 0x3d, 0x5a, 0xe6, 0x3f, 
        0xc6, 0x7e, 0xe5, 0x2f, 0x0f, 0x3e, 0xe6, 0x3f, 0xc0, 0x14, 0xfa, 0xc6, 
        0xdf, 0xe2, 0xb2, 0x3f, 0xf9, 0x16, 0x0b, 0x45, 0x70, 0xe2, 0xe6, 0xbf, 
        0xab, 0x86, 0xa1, 0x1e, 0xa3, 0xb4, 0xe4, 0xbf, 0x49, 0xce, 0x36, 0xb7, 
        0xd0, 0x54, 0xf7, 0xbf, 0x11, 0x05, 0xf9, 0xe2, 0x6a, 0x47, 0xb6, 0xbf, 
        0x4d, 0xbb, 0x1e, 0x59, 0x46, 0xff, 0xef, 0xbf, 0xec, 0x4c, 0x71, 0x17, 
        0xbb, 0xea, 0x87, 0xbf, 0x90, 0x48, 0xf8, 0xa8, 0x90, 0x1e, 0x7a, 0x3f, 
        0x80, 0x54, 0xc4, 0xc0, 0xe9, 0xc4, 0x62, 0x3f, 0x1e, 0x29, 0x03, 0xbc, 
        0xde, 0x8a, 0xd4, 0x3f, 0xd0, 0xcf, 0xe8, 0x4d, 0x84, 0x4e, 0xee, 0x3f, 
        0xf0, 0x55, 0x3f, 0x72, 0xdb, 0xd7, 0x8a, 0xbf, 0xe6, 0x07, 0xc6, 0x1d, 
        0xf3, 0x4d, 0xee, 0x3f, 0x8a, 0x17, 0x5d, 0x54, 0xf7, 0x89, 0xd4, 0xbf, 
        0x5b, 0x59, 0xc6, 0x27, 0xec, 0x89, 0x9f, 0x3f, 0xd0, 0x8c, 0xa9, 0xfc, 
        0x4a, 0xf9, 0xf4, 0xbf, 0x8b, 0x00, 0xcc, 0xd2, 0xe7, 0x96, 0x01, 0xc0, 
        0xe8, 0x73, 0xa9, 0x04, 0x2b, 0xaa, 0xc6, 0x3f, 0x87, 0x78, 0x35, 0x72, 
        0xd6, 0x12, 0xef, 0xbf, 0x12, 0x88, 0xa5, 0x28, 0x1a, 0x86, 0xc4, 0xbf, 
        0xd7, 0xc4, 0x20, 0xbd, 0xb9, 0x08, 0xe5, 0xbf, 0x04, 0x82, 0xca, 0xd3, 
        0x9d, 0x74, 0xce, 0xbf, 0xe6, 0x25, 0x83, 0x4d, 0xd9, 0xe1, 0xe6, 0x3f, 
        0x14, 0xff, 0x45, 0x23, 0xcb, 0x70, 0xe7, 0xbf, 0xd0, 0x29, 0xe2, 0x45, 
        0xd1, 0xba, 0x95, 0xbf, 0x04, 0xc2, 0xb1, 0xdc, 0xf7, 0xc5, 0xe5, 0xbf, 
        0x61, 0xe3, 0x39, 0xca, 0x36, 0x91, 0xf0, 0x3f, 0xa9, 0xd8, 0x5b, 0xd7, 
        0x2d, 0x59, 0xf3, 0xbf, 0x41, 0x63, 0x3e, 0x34, 0xcd, 0x60, 0xb9, 0xbf, 
        0xde, 0x44, 0x04, 0x5d, 0xde, 0xfa, 0xef, 0x3f, 0x96, 0xff, 0x74, 0xd2, 
        0xf6, 0x59, 0xa1, 0xbf, 0x7c, 0xd3, 0xfa, 0x40, 0xe9, 0xdf, 0x84, 0xbf, 
        0xcc, 0x52, 0x9b, 0xde, 0x16, 0x9a, 0x6d, 0xbf, 0x86, 0x0e, 0x44, 0x89, 
        0x44, 0x97, 0xd8, 0xbf, 0x95, 0x71, 0x8a, 0x07, 0x18, 0x8b, 0xed, 0x3f, 
        0xc0, 0x3e, 0xc4, 0x05, 0x47, 0x06, 0xa2, 0xbf, 0x6f, 0x68, 0x20, 0x00, 
        0x0e, 0x86, 0xed, 0xbf, 0xaa, 0xd4, 0xf1, 0xaf, 0x54, 0x95, 0xd8, 0xbf, 
        0xa3, 0xee, 0xdc, 0x92, 0x0e, 0x56, 0x8c, 0xbf, 0x39, 0xc1, 0xc3, 0xea, 
        0xbd, 0x52, 0xf6, 0xbf, 0x52, 0xf1, 0x29, 0xaf, 0x84, 0xaa, 0xff, 0xbf, 
    };
    const unsigned int kExtrinsicBinSize = 384;

    if(kExtrinsicBinSize == sizeof(double)*48)
    {
        for(int i = 0; i<4 ;i++)
        {
            memcpy(m_rotation + i * 9, kExtrinsicBin + i * 12 * sizeof(double), 9 * sizeof(double));
            memcpy(m_translation + i * 3, kExtrinsicBin + i * 12 * sizeof(double) + 9 * sizeof(double), 3 * sizeof(double));
        }
    }else{
        memset(m_rotation, 0, sizeof(m_rotation));
        memset(m_translation, 0, sizeof(m_translation));
    }
    
    /*前后摄像头畸变系数 coeff*/
    m_frontviewun.offu = 0.5005f;
    m_frontviewun.offv = 0.53f;
    m_frontviewun.scaleu = 1.3f;
    m_frontviewun.scalev = 1.43f;
    m_frontviewun.coeff = 0.0f;

    m_rearviewun.offu = 0.498f;
    m_rearviewun.offv = 0.53f;
    m_rearviewun.scaleu = 1.65f;
    m_rearviewun.scalev = 1.43f;
    m_rearviewun.coeff = 0.0f;


    m_leftviewpa.rotateangle = -27.52;
    m_leftviewpa.lookatpa.lookateyex = -0.75;
    m_leftviewpa.lookatpa.lookateyey = -2.9;
    m_leftviewpa.lookatpa.lookateyez = 0.6;
    m_leftviewpa.lookatpa.lookatcenterx = -0.75;
    m_leftviewpa.lookatpa.lookatcentery = -2.1;
    m_leftviewpa.lookatpa.lookatcenterz = 0.01;
    m_leftviewpa.lookatpa.lookatupx = 0.0;
    m_leftviewpa.lookatpa.lookatupy = 1.0;
    m_leftviewpa.lookatpa.lookatupz = 0.0;

    m_rightviewpa.rotateangle = 27.15;
    m_rightviewpa.lookatpa.lookateyex = 0.73;
    m_rightviewpa.lookatpa.lookateyey = -2.9;
    m_rightviewpa.lookatpa.lookateyez = 0.6;
    m_rightviewpa.lookatpa.lookatcenterx = 0.73;
    m_rightviewpa.lookatpa.lookatcentery = -2.1;
    m_rightviewpa.lookatpa.lookatcenterz = 0.0;
    m_rightviewpa.lookatpa.lookatupx = 0.0;
    m_rightviewpa.lookatpa.lookatupy = 1.0;
    m_rightviewpa.lookatpa.lookatupz = 0.0;
    
}


OCamConfig::~OCamConfig()
{

}

OCamConfig* OCamConfig::instance()
{
    if(m_instance == nullptr)
    {
        m_instance = new OCamConfig();
    }
    return m_instance;
}

CameraInnerParam OCamConfig::getCameraInnerParam(int nCameraID)
{
    return m_ocams[nCameraID];
}

void OCamConfig::world2cam(double point2D[2], double point3D[3], const struct CameraInnerParam& pOcamModel)
{
    const double *invpol = pOcamModel.invpol;//添加畸变的 多项式系数
    double xc = (pOcamModel.xc);
    double yc = (pOcamModel.yc);
    double c = (pOcamModel.c);
    double d = (pOcamModel.d);
    double e = (pOcamModel.e);
    int length_invpol = (pOcamModel.length_invpol);
    double norm = sqrt(point3D[0]*point3D[0] + point3D[1]*point3D[1]);//归一化坐标
    double theta = atan(point3D[2]/norm);
    double t, t_i;
    double rho, x, y;
    double invnorm;
    int i;

    if (norm != 0){
        invnorm = 1/norm;
        t  = theta;
        rho = invpol[0];
        t_i = 1;

        /*计算多项式*/
        for (i = 1; i < length_invpol; i++){
            t_i *= t;
            rho += t_i*invpol[i];
        }

        x = point3D[1]*invnorm*rho;
        y = point3D[0]*invnorm*rho;

        point2D[1] = x*c + y*d + xc;//对应图像的列坐标（u坐标）
        point2D[0] = x*e + y   + yc;//对应图像的行坐标（v坐标）
    }
    else{
        point2D[1] = xc;
        point2D[0] = yc;
    }
}

double* OCamConfig::getRotation(int id)
{
    return m_rotation + id * 9;
}

double* OCamConfig::getTranslation(int id)
{
    return m_translation + id * 3;
}

const undistort& OCamConfig::getFrontviewun()
{
    return m_frontviewun;
}

const undistort& OCamConfig::getRearviewun()
{
    return m_rearviewun;
}

const leftviewpa& OCamConfig::getLeftviewpa()
{
    return m_leftviewpa;
}

const rightviewpa& OCamConfig::getRightviewpa()
{
    return m_rightviewpa;
}