#ifndef PSEUDO3D_BACKGROUND_H
#define PSEUDO3D_BACKGROUND_H

#include <SDL.h>
#include <vector>
#include <string>

// 目标：
//   用「世界坐标 -> 透视投影 -> SDL_RenderGeometry」实现东方风格
//   纵深地板 + 两侧墙壁，并支持向前滚动
//
// 坐标约定：
//   x：左右（右为正）
//   y：高度（上为正）
//   z：前后深度（越大越远）
//   相机位于 (0, cameraHeight, 0)，朝 +z 看，无旋转。
//
// 投影公式（world -> screen，horizonY 已是屏幕坐标）：
//   screen_x = centerX + focal * x / z
//   screen_y = horizonY + focal * (cameraHeight - y) / z
//
// 为什么用网格而不是单个 4 顶点梯形：
//   SDL_RenderGeometry 只做「仿射」贴图，不做「透视校正」。
//   单个梯形会纹理扭曲。把 z 方向切成 zSegments 段，每段一个四边形，
//   段足够密时仿射近似透视，肉眼看不出差别。
class Pseudo3DBackground
{
public:
    Pseudo3DBackground() = default;
    ~Pseudo3DBackground();

    // 加载背景纹理（stg5bg.png）
    void init(SDL_Renderer* renderer, const std::string& texturePath);

    // 设置相机/世界参数并生成网格
    // horizonY 是「相对游玩区顶部」的地平线位置（JSON 里那个值）
    void setConfig(float cameraHeight, float focal, float horizonYRel, float wallHeight,
                   float halfWidth, float zNear, float zFar,
                   int zSegments, float scrollSpeed, float vTiles,
                   SDL_Rect floorSrc, SDL_Rect wallSrc, SDL_Rect playAreaRect);

    // 每帧滚动（前进）
    void update(float deltaTime);

    // 渲染（内部裁剪到游玩区）
    void render(SDL_Renderer* renderer);

    // 清理纹理
    void clean();

private:
    // 一个面（地板 / 左墙 / 右墙）的网格
    struct Surface
    {
        SDL_Rect src;                    // 纹理区域（atlas 里的子矩形）
        std::vector<SDL_FPoint> screen;  // 投影后的屏幕坐标（固定，不随滚动变）
        std::vector<float> worldZ;       // 顶点世界 z（用于算纹理 v，滚动用）
        std::vector<float> texU;         // 顶点纹理 u（固定）
        std::vector<int> indices;        // 三角形索引
        std::vector<SDL_Vertex> verts;   // 每帧渲染顶点（复用缓冲）
    };

    // 世界坐标 -> 屏幕坐标
    SDL_FPoint project(float x, float y, float z) const;

    // 生成地板网格
    void buildFloor();
    // 生成墙网格：x 为墙的世界 x（左墙 -halfWidth，右墙 +halfWidth）
    void buildWall(Surface& s, float x, float u0, float u1);
    // 渲染一个面
    void renderSurface(SDL_Renderer* renderer, Surface& s);

    SDL_Texture* texture = nullptr;

    // 相机参数
    float cameraHeight = 0.0f; // 相机高度
    float focal = 0.0f; // 焦距，透视投影缩放倍数
    float horizonY = 0.0f; // 地平线高度

    // 世界参数
    float halfWidth = 0.0f; // 地面半宽
    float zNear = 0.0f; // 最近距离
    float zFar = 0.0f; // 最远距离
    float wallHeight = 0.0f; // 墙高度

    // 渲染参数
    int zSegments = 0; // 段数
    float scrollSpeed = 0.0f; // 移动速度
    float vTiles = 0.0f; // 地板深度方向纹理平铺次数
    float scrollZ = 0.0f; // 累计滚动（世界单位）

    SDL_Rect playAreaRect{0, 0, 0, 0};
    float centerX = 0.0f; // 屏幕中心

    Surface floor;
    Surface wallLeft;
    Surface wallRight;
};

#endif
