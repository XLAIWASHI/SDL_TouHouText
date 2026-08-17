#include "Pseudo3DBackground.h"
#include <SDL_image.h>
#include <cmath>

// ====================================================================
// === AI 实现：伪 3D 背景类 ===========================================
// ====================================================================

Pseudo3DBackground::~Pseudo3DBackground()
{
    clean();
}

// --- 加载纹理 -------------------------------------------------------
void Pseudo3DBackground::init(SDL_Renderer* renderer, const std::string& texturePath)
{
    clean();
    texture = IMG_LoadTexture(renderer, texturePath.c_str());
    if (texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Pseudo3D 背景纹理加载失败: %s", IMG_GetError());
    }
}

//设置参数并生成网格
void Pseudo3DBackground::setConfig(float cameraHeight_, float focal_, float horizonYRel, 
                                   float wallHeight_, float halfWidth_, float zNear_, float zFar_, 
                                   int zSegments_, float scrollSpeed_, float vTiles_, 
                                   SDL_Rect floorSrc, SDL_Rect wallSrc, SDL_Rect playAreaRect_) 
{ 
    cameraHeight = cameraHeight_; 
    focal = focal_; 
    halfWidth = halfWidth_; 
    zNear = zNear_; 
    zFar = zFar_; 
    wallHeight = wallHeight_; 
    zSegments = zSegments_ > 0 ? zSegments_ : 1; 
    scrollSpeed = scrollSpeed_; 
    vTiles = vTiles_; 
    playAreaRect = playAreaRect_; 
    scrollZ = 0.0f; 
 
    centerX = playAreaRect.x + playAreaRect.w / 2.0f; 
    horizonY = playAreaRect.y + horizonYRel; 
 
    floor.src = floorSrc;

    // 加这两行
    wallLeft.src = wallSrc;
    wallRight.src = wallSrc;

    buildFloor();

    buildWall(
        wallLeft,
        -halfWidth,
        wallSrc.x,
        wallSrc.x + wallSrc.w
    );

    buildWall(
        wallRight,
        halfWidth,
        wallSrc.x,
        wallSrc.x + wallSrc.w
    );
}

// --- 每帧滚动 -------------------------------------------------------
void Pseudo3DBackground::update(float deltaTime)
{
    scrollZ += scrollSpeed * deltaTime;
}

// --- 渲染 -----------------------------------------------------------
void Pseudo3DBackground::render(SDL_Renderer* renderer)
{
    if (texture == nullptr) return;

    // 裁剪到游玩区，避免墙顶超出
    SDL_RenderSetClipRect(renderer, &playAreaRect);

    // 填充背景色（顶部）
    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &playAreaRect
    );

    renderSurface(renderer, floor);
    renderSurface(renderer, wallLeft);
    renderSurface(renderer, wallRight);

    SDL_RenderSetClipRect(renderer, nullptr);
}

// --- 清理 -----------------------------------------------------------
void Pseudo3DBackground::clean()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

// --- 世界坐标 -> 屏幕坐标 ---------------------------------------------
SDL_FPoint Pseudo3DBackground::project(float x, float y, float z) const
{
    SDL_FPoint p;
    p.x = centerX + focal * x / z;
    p.y = horizonY + focal * (cameraHeight - y) / z;
    return p;
}

//生成地板网格
void Pseudo3DBackground::buildFloor()
{
    int rows = zSegments + 1;
    int vertCount = rows * 2;

    floor.screen.resize(vertCount);
    floor.worldZ.resize(vertCount);
    floor.texU.resize(vertCount);
    floor.indices.clear();

    // 纹理 u：左 -> 右（对应地板 src 的 x 范围）
    float u0 = static_cast<float>(floor.src.x);
    float u1 = static_cast<float>(floor.src.x + floor.src.w);

    // 深度按 1/z 线性分布，让屏幕上的分段更均匀（近处密、远处疏）
    float invZNear = 1.0f / zNear;
    float invZFar = 1.0f / zFar;

    for (int i = 0; i < rows; i++)
    {
        float t = static_cast<float>(i) / zSegments;
        float z = 1.0f / (invZNear * (1.0f - t) + invZFar * t);

        // 左顶点
        floor.worldZ[i * 2] = z;
        floor.texU[i * 2] = u0;
        floor.screen[i * 2] = project(-halfWidth, 0.0f, z);
        // 右顶点
        floor.worldZ[i * 2 + 1] = z;
        floor.texU[i * 2 + 1] = u1;
        floor.screen[i * 2 + 1] = project(halfWidth, 0.0f, z);
    }

    // 每段 2 个三角形
    for (int i = 0; i < zSegments; i++)
    {
        int a = i * 2, b = i * 2 + 1, c = i * 2 + 2, d = i * 2 + 3;
        floor.indices.push_back(a);
        floor.indices.push_back(b);
        floor.indices.push_back(d);
        floor.indices.push_back(a);
        floor.indices.push_back(d);
        floor.indices.push_back(c);
    }
}

//生成墙网格
// x：墙的世界 x（固定），u0/u1：墙纹理 u 范围（下 -> 上）
void Pseudo3DBackground::buildWall(Surface& s, float x, float u0, float u1)
{
    int rows = zSegments + 1;
    int vertCount = rows * 2;

    s.screen.resize(vertCount);
    s.worldZ.resize(vertCount);
    s.texU.resize(vertCount);
    s.indices.clear();

    float invZNear = 1.0f / zNear;
    float invZFar = 1.0f / zFar;

    for (int i = 0; i < rows; i++)
    {
        float t = static_cast<float>(i) / zSegments;
        float z = 1.0f / (invZNear * (1.0f - t) + invZFar * t);

        // 下顶点（贴地 y=0）
        s.worldZ[i * 2] = z;
        s.texU[i * 2] = u0;
        s.screen[i * 2] = project(x, 0.0f, z);
        // 上顶点（墙顶 y=wallHeight）
        s.worldZ[i * 2 + 1] = z;
        s.texU[i * 2 + 1] = u1;
        s.screen[i * 2 + 1] = project(x, wallHeight, z);
    }

    for (int i = 0; i < zSegments; i++)
    {
        int a = i * 2, b = i * 2 + 1, c = i * 2 + 2, d = i * 2 + 3;
        s.indices.push_back(a);
        s.indices.push_back(b);
        s.indices.push_back(d);
        s.indices.push_back(a);
        s.indices.push_back(d);
        s.indices.push_back(c);
    }
}

// --- 渲染一个面 ------------------------------------------------------
void Pseudo3DBackground::renderSurface(SDL_Renderer* renderer, Surface& s)
{
    
    int n = static_cast<int>(s.screen.size());
    if (n == 0 || s.src.h == 0) return;

    s.verts.resize(n);

    // 一个纹理平铺对应的世界深度
    float tileDepth = (zFar - zNear) / vTiles;

    int texW, texH;

    SDL_QueryTexture(
        texture,
        nullptr,
        nullptr,
        &texW,
        &texH
    );

    for (int i = 0; i < n; i++)
    {
        float v = static_cast<float>(s.src.y) 
                + fmodf(
                    (s.worldZ[i] + scrollZ - zNear)
                    / tileDepth * s.src.h,
                    static_cast<float>(s.src.h)
                );

        s.verts[i].position = s.screen[i];
        s.verts[i].color = {255, 255, 255, 255};

        // 这里改
        s.verts[i].tex_coord =
        {
            s.texU[i] / texW,
            v / texH
        };
    }

    

    int ret = SDL_RenderGeometry(renderer, texture,
                       s.verts.data(), n,
                       s.indices.data(), static_cast<int>(s.indices.size()));
    // if(ret != 0)
    // {
    //     SDL_Log("Geometry error: %s", SDL_GetError());
    // }
}
