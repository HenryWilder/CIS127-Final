#pragma once
#include "Utility.h"
#include "Visuals.h"
#include "Menu.h"

void BeginDrawing();
void EndDrawing();
bool ShouldClose();
template<typename _Token> _Token Prompt();
