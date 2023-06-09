/*
* gameApp.hpp
*/

#pragma once

#include "playerUnit.hpp"
#include "botUnit.hpp"
#include "defenderUnit.hpp"
#include "attackerUnit.hpp"


void reshapeCallback(int, int);
void displayCallback();
void idleCallback();
void keyboardCallback(unsigned char, int, int);
void keyboardUpCallback(unsigned char, int, int);

void makeProjection();
void makeView();

void handleCollisions();
void handleCollision(GameObject& a, GameObject& b);

void renderScene();
void drawGrid();