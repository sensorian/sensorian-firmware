#ifndef CUBE_HEADER
#define CUBE_HEADER

#include "Transforms.h"

#define BACKGROUND BLACK
#define FOREGROUND WHITE

void InitializeCube(void);
void RotateCubeX(float angle);
void RotateCubeY(float angle);
void RotateCubeZ(float angle);
void SaveOldLinesCube(void);
void ProjectCube(void);
void DrawCube(void);
void ScaleDistort(void);
void OffsetBounce(void);

#endif

