# libOcamavm

`libOcamavm` 是一个基于 OpenGL ES + EGL 的鱼眼相机去畸变渲染库，支持前/左/后/右四路相机输入（YUYV），输出去畸变后的 RGBA 图像。  
项目内置了相机内参、外参和左右视角参数，并提供 demo 用于快速验证。

## 功能特性

- 支持 4 路相机：前(0)、左(1)、后(2)、右(3)
- 输入格式：YUYV（YUV422）
- 输出格式：RGBA（通过回调返回）
- 支持默认输出分辨率（与输入一致）和自定义输出分辨率
- 使用离屏渲染（EGL Pbuffer + FBO），无窗口环境可运行

## 目录结构

- `include/Ocamavm.h`：对外渲染接口
- `include/Ocamconfig.h`：相机参数与配置接口
- `Ocamavm.cpp`：EGL 初始化、shader、纹理与渲染流程
- `Ocamconfig.cpp`：内外参与视角参数定义
- `mvtpublic.cpp`：网格/纹理坐标生成与去畸变数据处理
- `demo/main.cpp`：调用示例
- `CMakeLists.txt`：CMake 构建脚本
- `Android.bp`：Android Soong 构建脚本

## 环境依赖

- C++ 编译器（支持 C++11 及以上）
- EGL
- OpenGL ES 2.0/3.0
- CMake（如使用 CMake 构建）

在 Linux/QNX 环境下，构建脚本会尝试通过 `pkg-config` 查找 `egl` 和 `glesv2`；若未找到则回退到 `find_library`。

## 构建方式

### 1) CMake 构建（Linux/QNX）

```bash
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build . -j
cmake --install .
```

构建产物：
- 动态库：`libOcamavm.so`
- demo 可执行文件：`demo`

安装位置（受 `CMAKE_INSTALL_PREFIX` 影响）：
- 库：`${prefix}/lib`
- 头文件：`${prefix}/include`
- demo：`${prefix}/bin`

### 2) Android Soong 构建

项目提供 `Android.bp`：
- `cc_library_shared { name: "libOcamavm" }`
- `cc_binary { name: "ocamavm_test" }`

可直接纳入 Android 源码树按模块编译。

## API 使用说明

对外核心类：`OCamRender`（单例）

- `static OCamRender* instance();`
- `bool Distortimg(int nCamera, const char* yuv, int w, int h, RgbaFrameCallback cb);`
- `bool Distortimg(int nCamera, const char* yuv, int srcW, int srcH, int outW, int outH, RgbaFrameCallback cb);`

回调定义：

```cpp
typedef void (*RgbaFrameCallback)(int w, int h, int len, const char* data);
```

参数约束（来自当前实现）：
- `nCamera` 取值范围：`0~3`
- 当前实现固定输入尺寸：`1920x1536`
- 输出尺寸需满足：`outW <= 1920` 且 `outH <= 1536`
- 输入 `yuv` 不能为空，回调 `cb` 不能为空

## 最小调用示例

```cpp
#include "Ocamavm.h"
#include <fstream>

static void OnRgba(int w, int h, int len, const char* data) {
    std::ofstream ofs("front_distort.rgba", std::ios::binary);
    ofs.write(data, len);
}

int main() {
    const int W = 1920;
    const int H = 1536;
    const char* yuyv = /* 填充一帧 YUYV 数据 */;
    OCamRender::instance()->Distortimg(0, yuyv, W, H, OnRgba);
    return 0;
}
```

## Demo 说明

`demo/main.cpp` 演示了四路输入的调用方式，默认读取以下路径：

- `/data/Ocam/front_out.yuv`
- `/data/Ocam/left_out.yuv`
- `/data/Ocam/rear_out.yuv`
- `/data/Ocam/right_out.yuv`

输出为对应的 `.rgba` 文件。实际使用时请按你的环境修改输入/输出路径。

## 注意事项

- 库内部会创建 EGL 上下文与离屏 FBO，请确保目标环境存在可用 EGL/GLES 实现。
- 若要支持非 `1920x1536` 输入，需要同步修改实现中的尺寸常量与相关纹理/FBO分配逻辑。
- 回调拿到的 RGBA 数据在 OpenGL 读取坐标系下产生，如显示方向不符合预期，可在上层做翻转处理。

