#pragma once

#include "stdafx.h"
#include "BaseObject.h"
#include <list>
using namespace std;

BOOL GameInit();

void KeyProcess();
void Update();
void Action();
void Draw();

BOOL FrameSkip();

void YAxisSort();
bool Comp(CBaseObject* first, CBaseObject * sec);
