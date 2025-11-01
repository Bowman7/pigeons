#ifndef FINAL_LOOP_HPP
#define FINAL_LOOP_HPP

#include<iostream>
#include"../raylib/raylib.h"
#include<cmath>
#include<algorithm>
#include"stdio.h"

#include"Body.hpp"
#include"audio.hpp"

class FinalLoop{
public:
  FinalLoop(AudioMix* audioMix);
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
  //background animation
  Image backgroundImage;
  Texture2D backgroundTexture;
  Rectangle frameRec;
  float textureWidth;
  float textureHeight;
  
  int frameCounter = 0;
  int currentFrame = 0;

  //for input state update
  struct Inputstate{
    
    bool rightPressed = false;
    bool leftPressed = false;
    bool rightPunchPressed = false;
    bool leftPunchPressed = false;
    bool jumpPressed = false;
    
  }inputstate;
  //audio
  AudioMix* audioFMix;
  
  
};



#endif 
