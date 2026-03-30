#include "Ocamavm.h"

#include <fstream>
#include <vector>
#include <string>

static std::string g_outpath;

static std::vector<char> ReadFileToBuffer(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<char> buf(static_cast<size_t>(size));
    if(!ifs.read(buf.data(), size))
    {
        printf("Read file failed: %s\n", path.c_str());
        return {};
    }
    return buf;
}

static void OnRgbaFrame(int w, int h, int len, const char* data)
{
    if (data == nullptr || len <= 0 || w <= 0 || h <= 0 || g_outpath.empty()) {
        return;
    }
    std::ofstream ofs(g_outpath, std::ios::binary);
    if (!ofs.is_open()) {
        printf("open output failed: %s\n", g_outpath.c_str());
        return;
    }
    ofs.write(data, static_cast<std::streamsize>(len));
    if (!ofs.good()) {
        printf("write output failed: %s\n", g_outpath.c_str());
    }
}

int main()
{ 
    /*使用去畸变库方法*/
    /*
        图像格式：yuyv（YUV422)输入
        输入图像分辨率：1920*1536
        目前demo可以输出反畸变后的图像 保存在指定地址(RGBA格式)
    */
    
    //预先步骤：把已有的yuyv图片读取到buffer区内
    constexpr int W = 1920;
    constexpr int H = 1536;
    constexpr size_t frameSize = static_cast<size_t>(W) * static_cast<size_t>(H) * 2;
    std::vector<char> mbuffer;
    mbuffer.resize(frameSize);

    mbuffer = ReadFileToBuffer("/data/Ocam/front_out.yuv");
    //接口：把buffer传入去畸变库 即可 全程使用单例 这里传入前摄像头的图像(并选择0:front cam)
    g_outpath = "/data/Ocam/front_distort.rgba";
    OCamRender::instance()->Distortimg(0, mbuffer.data(), W, H, OnRgbaFrame);

    mbuffer = ReadFileToBuffer("/data/Ocam/left_out.yuv");
    //接口：把buffer传入去畸变库 即可 全程使用单例 这里传入左摄像头的图像(并选择1:left cam)
    g_outpath = "/data/Ocam/left_distort.rgba";
    OCamRender::instance()->Distortimg(1, mbuffer.data(), W, H, 462, 290, OnRgbaFrame);

    mbuffer = ReadFileToBuffer("/data/Ocam/rear_out.yuv");
    //接口：把buffer传入去畸变库 即可 全程使用单例 这里传入后摄像头的图像(并选择2:rear cam)
    g_outpath = "/data/Ocam/rear_distort.rgba";
    OCamRender::instance()->Distortimg(2, mbuffer.data(), W, H, 494, 278, OnRgbaFrame);

    mbuffer = ReadFileToBuffer("/data/Ocam/right_out.yuv");
    //接口：把buffer传入去畸变库 即可 全程使用单例 这里传入右摄像头的图像(并选择3:right cam)
    g_outpath = "/data/Ocam/right_distort.rgba";
    OCamRender::instance()->Distortimg(3, mbuffer.data(), W, H, 462, 290, OnRgbaFrame);
    
    return 0;
}
