#ifndef FINAL_LOOP_HPP
#define FINAL_LOOP_HPP

#include<iostream>
#include"../raylib/raylib.h"
#include<cmath>
#include<algorithm>
#include"stdio.h"

#include"Body.hpp"

class FinalLoop{
public:
  FinalLoop();
  ~FinalLoop();
  void Init();
  void deInit();
  void HandleInput();
  void UpdatePhysics(double);
  void Update();
  void Draw(double);
  
  void HandleCollision();

private:
  
  Body hero;
  Body enemy;

  //for input state update
  struct Inputstate{
    
    bool rightPressed = false;
    bool leftPressed = false;
    bool rightPunchPressed = false;
    bool leftPunchPressed = false;
    bool jumpPressed = false;
    
  }inputstate;
  
};



#endif 
