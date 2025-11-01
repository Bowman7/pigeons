#ifndef BODY_HPP
#define BODY_HPP

#include"../raylib/raylib.h"


typedef enum{
    LEFT,
    RIGHT,
}Direction;

typedef enum{
  IDLE,
  JAB,
  CROSS,
  WALK_RIGHT,
  WALK_LEFT,
  HIT,
  STUN,
}State;

class Body{
public:
  Body(Color color,Direction direction){
    
    this->color = color;
    this->direction = direction;
    //current state
    this->currentState = IDLE;
    
    
    //for hitbox
    this->hitbox1_position.x = 50.0f;
    this->hitbox1_position.y = (bodySize.y/3);

    this->hitbox2_position.x = 110.0f;
    this->hitbox2_position.y = 100.0f;
    
    this->h1_color = GREEN;
    this->h2_color = PINK;
    
  }
  float GetX(){
    return position.x;;
  }
  float GetY(){
    return position.y;
  }
  //for hitbox 1
  float GetHitRX(){
    return hitbox1_position.x;
  }
  
  float GetHitRY(){
    return hitbox1_position.y;
  }
  //for hitbox2
  float GetHitLX(){
    return hitbox2_position.x;
  }
  
  float GetHitLY(){
    return hitbox2_position.y;
  }

  float GetHSizeX(){
    return hitbox_size.x;
  }
  
  float GetHSizeY(){
    return hitbox_size.y;
  }

  Vector2 GetSize(){
    return bodySize;
  }
  Direction GetDirection(){
    return direction;
  }
  
  void SetX(int x){

    position.x += (float)x;

  }
  //init enemypos
  void enemyPos(){
    position.x = 300.0f;
    position.y = 360.0f;
  }
  
  void SetColor(Color color){
    this->color = color;
  }
  void SetDirection(Direction dir){
    this->direction = dir;
  }

  Color GetH1Color(){
    return h1_color;
  }

  Color GetH2Color(){
    return h2_color;
  }
  Color GetColor(){
    return color;
  }
  double GetHeight(){
    return bodySize.y;
  }

  double GetWidth(){
    return bodySize.x;
  }

  bool IsRightAttacking(){
    return rightAttacking;
  }
  void SetRightAttacking(){
    rightAttacking = !rightAttacking;
  }
  bool IsLeftAttacking(){
    return leftAttacking;
  }
  void SetLeftAttacking(){
    leftAttacking = !leftAttacking;
  }
  bool IsOnGround(){
    return onGround;
  }
  void SetOnGround(){
    if(onGround){
      onGround = false;
    }else{
      onGround = true;    }
  }

  Vector2 GetHitboxSize(){
    return hitbox_size;
  }

  void Init(const char* imageSrc,int id){
    bodyImage  = LoadImage(imageSrc);
    if(id == 1){//hero
      ImageResizeNN(&bodyImage,1600,2000);
      bodyTexture =  LoadTextureFromImage(bodyImage);

      textureWidth = bodyTexture.width;
      textureHeight = bodyTexture.height;
    
      frameRec = {0.0f,0.0f,textureWidth/8,textureHeight/10};
    }else if(id == 2){//enemy
      ImageResizeNN(&bodyImage,600,600);
      bodyTexture = LoadTextureFromImage(bodyImage);

      textureWidth = bodyTexture.width;
      textureHeight = bodyTexture.height;

      frameRec = {0.0f,0.0f,textureWidth/3,textureHeight/3};

      std::cout<<"successful init "<<textureWidth<<textureHeight<<std::endl;
    }
    else{
      ImageResizeNN(&bodyImage,200,200);
      bodyTexture = LoadTextureFromImage(bodyImage);
    }
    
  }

  void deInit(){
    UnloadTexture(bodyTexture);
  }
private:
  
  Color color;
  Direction direction;
  bool rightAttacking = false;
  bool leftAttacking = false;
  Vector2 bodySize = {100.0f,200.0f};
  
public:
  bool onGround = false;
  
  Vector2 position = {0.0f,0.0f};
  Vector2 velocity = {0.0f,0.0f};
  Vector2 acceleration = {10.0f,0.0f};
  
  
  float attackTimer = 0.0f;

  //hitbox1 stuff
  Vector2 hitbox_size = {50.0f,50.0f};
  
  Vector2 hitbox1_position = {0.0f,0.0f};
  Vector2 hitbox1_velocity = {0.0f,0.0f};
  Color h1_color;

  //hitbox2 stuff
  Vector2 hitbox2_position = {0.0f,0.0f};
  Vector2 hitbox2_velocity = {0.0f,0.0f};
  Color h2_color;

  //hurtbox stuff
  
  int hitsTaken =0;
  int hitCount = 0;
  bool stunned = false;

  //texture
  Image bodyImage;
  Texture2D bodyTexture;
  Rectangle frameRec;
  float textureWidth;
  float textureHeight;

  //current state
  State currentState;
  int frameCounter = 0;
  int currentFrame = 0;
  
  int idleFrameSpeed = 8;
  int walkRightFrameSpeed = 10;
  int walkLeftFrameSpeed = 12;
  int jabFrameSpeed = 14;
  int crossFrameSpeed = 14;
  //default max frame
  int defaultMaxFrame = 7;
  int jabMaxFrame = 7;
  int crossMaxFrame = 7;
  
};

#endif
