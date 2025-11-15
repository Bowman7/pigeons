#include"FinalLoop.hpp"

#if defined (__GNUC__) || defined (__clang__)
template<typename T>
constexpr T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
}
#endif

FinalLoop::FinalLoop(AudioMix* audioMix)
  :hero(WHITE,RIGHT),enemy(WHITE,LEFT)
{
  audioFMix = audioMix;
  enemy.enemyPos();
}

FinalLoop::~FinalLoop(){

}

void FinalLoop::Init(){
  //load background
  backgroundImage = LoadImage("./asset/bakground2-animapng.png");
  backgroundTexture = LoadTextureFromImage(backgroundImage);

  textureWidth = backgroundTexture.width;
  textureHeight = backgroundTexture.height;

  frameRec = {0.0f,0.0f,textureWidth,textureHeight/19};
  
  const char* heroImgSrc= "./asset/hero-shee.png";
  const char* enemyImgSrc = "./asset/dummy-shee.png";


  hero.Init(heroImgSrc,1);
  enemy.Init(enemyImgSrc,2);
}

void FinalLoop::deInit(){
  hero.deInit();
}

void FinalLoop::HandleInput(){
  
  inputstate.rightPressed = IsKeyDown(KEY_RIGHT);
  inputstate.leftPressed = IsKeyDown(KEY_LEFT);

  if(IsKeyPressed(KEY_W) && !hero.IsRightAttacking()){
    inputstate.rightPunchPressed = true;
    audioFMix->PlayHitAir();
  }else if(IsKeyPressed(KEY_E) && !hero.IsLeftAttacking()){
    inputstate.leftPunchPressed = true;
    audioFMix->PlayHitAir();
  }else if(IsKeyPressed(KEY_SPACE) && hero.onGround && !inputstate.jumpPressed){
    inputstate.jumpPressed = true;
  }else if(IsKeyPressed(KEY_P)){
    enemy.enemyPos();
  }
  
}

//street fight style physics
void FinalLoop::UpdatePhysics(double dt){
  //for enemy  state
  enemy.currentState = IDLE;
  
  //set defs
  const float WALK_SPEED = 350.0f;
  const float JUMP_SPEED = 900.0f;
  const float GRAVITY = 1800.0f;
  const float GROUND_FRICTION = 0.78f;
  const float AIR_FRICTION = 0.99f;

  //horz movement
  if(inputstate.rightPressed && !inputstate.leftPressed){
    if(hero.onGround && !hero.IsRightAttacking() && !hero.IsLeftAttacking()){
      hero.velocity.x = WALK_SPEED;
    }else if(!hero.onGround){
      hero.velocity.x += 1000.0f*dt ;
    }
    //change state
    hero.currentState = WALK_RIGHT;
  }else if(inputstate.leftPressed && !inputstate.rightPressed){
    if(hero.onGround && !hero.IsRightAttacking() && !hero.IsLeftAttacking()){
      hero.velocity.x = -WALK_SPEED;
    }else if(!hero.onGround){
      hero.velocity.x -= 1000.0f*dt;
    }
    //change state
    hero.currentState = WALK_LEFT;
  }else{
    //friction when no input
    float friction = hero.onGround?GROUND_FRICTION:AIR_FRICTION;
    hero.velocity.x *= pow(friction,dt*60.0f);
    //change hero state back to idle
    hero.currentState = IDLE;
  }
  //jump
  if(inputstate.jumpPressed && hero.onGround){
    hero.velocity.y = -JUMP_SPEED;
    inputstate.jumpPressed = false;
  }

  //attacks
  if(inputstate.rightPunchPressed && !hero.IsRightAttacking()
     && !hero.IsLeftAttacking() && !inputstate.leftPunchPressed){
    hero.attackTimer = 0.2f; //200ms
    hero.SetRightAttacking();

    Direction dir = hero.GetDirection();
    hero.velocity.x += (dir == RIGHT?100.0f:-100.0f);
    hero.hitbox1_position.x += 95.0f;
    
    inputstate.rightPunchPressed = false;
    
  }else if(inputstate.leftPunchPressed && !hero.IsRightAttacking()
	   && !hero.IsLeftAttacking() && !inputstate.rightPunchPressed){
    hero.attackTimer = 0.2f;
    hero.SetLeftAttacking();

    Direction dir = hero.GetDirection();
    hero.velocity.x += (dir == RIGHT ? 100.0f: -100.0f);
    hero.hitbox2_position.x += 40.0f;

    inputstate.leftPunchPressed = false;
  }

  //set state of jab and cross
  if(hero.IsLeftAttacking()){
    hero.currentState = JAB;
  }
  if(hero.IsRightAttacking()){
    hero.currentState = CROSS;
  }
  
  //update attacktimer
  if(hero.IsRightAttacking()){
    hero.attackTimer -= 1.0 * dt;
    if(hero.attackTimer <=0){
      hero.hitbox1_position.x -= 95.0f;
      hero.SetRightAttacking();
    }
  }else if(hero.IsLeftAttacking()){
    hero.attackTimer -= 1.0*dt;
    if(hero.attackTimer <=0){
      hero.hitbox2_position.x -= 40.0f;
      hero.SetLeftAttacking();
    }
  }

  //gravity and physics
  hero.acceleration.y = GRAVITY;
  hero.velocity.y += hero.acceleration.y*dt;
  
  hero.position.x += hero.velocity.x*dt;
  hero.hitbox1_position.x += hero.velocity.x*dt;
  hero.hitbox2_position.x += hero.velocity.x*dt;
  
  
  hero.position.y += hero.velocity.y*dt;
  hero.hitbox1_position.y = hero.position.y;
  hero.hitbox2_position.y = hero.position.y;

#if defined (__GNUC__) || defined (__clang__)
  hero.velocity.x = clamp(hero.velocity.x,-400.0f,400.0f);
  hero.velocity.y = clamp(hero.velocity.y,-700.0f,700.0f);
#else

  //speed caps
  hero.velocity.x = std::clamp(hero.velocity.x,-400.0f,400.0f);
  hero.velocity.y = std::clamp(hero.velocity.y,-700.0f,700.0f);
#endif

  

  //ground collision
  if(hero.position.y >= 360.0f){
    hero.position.y = 360.0f;
    hero.hitbox1_position.y = 360.0f+(hero.GetHeight()/3);
    hero.hitbox2_position.y = 360.0f+50.0f;;
    
    hero.velocity.y = 0.0f;
    hero.onGround = true;
  }else{
    hero.onGround = false;
  }
  
}

void FinalLoop::Update(){

  
  
  //hmain collision
  HandleCollision();

  //handle hits logic
  if( enemy.hitsTaken >= 10){
    if(enemy.hitCount<3){
      enemy.hitCount +=1;
      enemy.hitsTaken = 0;
    }else{
      enemy.stunned = true;
      enemy.hitsTaken = 0;
      enemy.hitCount = 0;
    }
  }

  //background update
  frameCounter++;
  if(frameCounter >= (60/10)){
    frameCounter = 0;
    currentFrame++;

    if(currentFrame>18){
      currentFrame = 0;
    }
    frameRec.y = (float)currentFrame*(float)textureHeight/19;
  }
  
  //enemy update
  enemy.frameCounter++;

  if(enemy.frameCounter >= (60/6)){
    enemy.frameCounter = 0;
    enemy.currentFrame++;

    if(enemy.currentFrame > 2){
      enemy.currentFrame = 0;
    }

    switch(enemy.currentState){
    case HIT:
      enemy.frameRec.y = enemy.textureHeight/3;break;
    case STUN:
      enemy.frameRec.y = (enemy.textureHeight/3)*2;break;
    default:
      enemy.frameRec.y = 0.0f;break;
    }
    enemy.frameRec.x = (float)enemy.currentFrame*(float)enemy.textureWidth/3;
  }

  
  

  //idle
  hero.frameCounter++;
  int frameSpeed = 0;
  switch(hero.currentState){
  case IDLE:
    frameSpeed = hero.idleFrameSpeed;break;
  case WALK_RIGHT:
    frameSpeed = hero.walkRightFrameSpeed;break;
  case WALK_LEFT:
    frameSpeed = hero.walkLeftFrameSpeed;break;
  case JAB:
    frameSpeed = hero.jabFrameSpeed;break;
  case CROSS:
    frameSpeed = hero.crossFrameSpeed;break;
  default:
    frameSpeed = hero.idleFrameSpeed;break;
  }

  //max frame count
  int maxFrame = 0;
  switch(hero.currentState){
  case JAB:
    maxFrame = hero.jabMaxFrame;break;
  default:
    maxFrame = hero.defaultMaxFrame;break;
  }
  
  if(hero.frameCounter >= (60/frameSpeed)){
    hero.frameCounter = 0;
    hero.currentFrame++;

    if(hero.currentFrame > maxFrame){
      hero.currentFrame = 0;
    }

    //switch frame rec acc to state
    switch(hero.currentState){
    case IDLE:
      hero.frameRec.y = 0.0f;break;
    case WALK_RIGHT:
      hero.frameRec.y = hero.textureHeight/10;break;
    case WALK_LEFT:
      hero.frameRec.y = (hero.textureHeight/10)*2;break;
    case JAB:
      hero.frameRec.y = (hero.textureHeight/10)*3;break;
    case CROSS:
      hero.frameRec.y = (hero.textureHeight/10)*4;break;
    default:
      hero.frameRec.y = 0.0f;break;
    }
    hero.frameRec.x = (float)hero.currentFrame*(float)hero.textureWidth/8;
  }
  
}

void FinalLoop::Draw(double alpha){
  //draw background
  //DrawTextureV(backgroundTexture,{0.0f,0.0f},WHITE);
  DrawTextureRec(backgroundTexture,frameRec,{0.0f,0.0f},WHITE);
  //render player pos
  Vector2 renderHeroPos = {
    (float)(hero.position.x * alpha + hero.position.x * (1-alpha)),
    (float)(hero.position.y * alpha + hero.position.y * (1-alpha))
  };

  //render hitbox1
  /* Vector2 renderHitboxPos = { */
  /*   (float)(hero.hitbox1_position.x * alpha + hero.hitbox1_position.x * (1-alpha)), */
  /*   (float)(hero.hitbox1_position.y * alpha + hero.hitbox1_position.y * (1-alpha)) */
  /* }; */

  //render hitbox2
  /* Vector2 renderHitbox2Pos = { */
  /*   (float)(hero.hitbox2_position.x * alpha + hero.hitbox2_position.x * (1-alpha)), */
  /*   (float)(hero.hitbox2_position.y * alpha + hero.hitbox2_position.y * (1-alpha)) */
  /* }; */
   
  //ground line
  DrawLine(0,560,1080,560,BLACK);

  //DrawRectangleV(renderHitbox2Pos,hero.GetHitboxSize(),hero.GetH1Color());
  //DrawRectangleV(renderHeroPos,hero.GetSize(),hero.GetColor());
  //DrawTextureV(hero.bodyTexture,renderHeroPos,WHITE);
  DrawTextureRec(hero.bodyTexture,hero.frameRec,renderHeroPos,WHITE);
  //DrawRectangleV(renderHitboxPos,hero.GetHitboxSize(),hero.GetH2Color());

  //render enemy pos
  //DrawTextureV(enemy.bodyTexture,enemy.position,enemy.GetColor());
  DrawTextureRec(enemy.bodyTexture,enemy.frameRec,enemy.position,enemy.GetColor());
}

void FinalLoop::HandleCollision(){
  if((hero.GetHitRX()<enemy.GetX()+enemy.GetWidth() &&
      hero.GetHitRX()+hero.GetHSizeX()>enemy.GetX()+40.0f)
     ||
     (hero.GetHitLX()<enemy.GetX()+enemy.GetWidth() &&
      hero.GetHitLX()+hero.GetHSizeX()>enemy.GetX()+40.0f)
  
     ){
    
    //enemy.SetColor(RED);
    audioFMix->PlayHitSound();
    enemy.currentState = HIT;
    enemy.hitsTaken += 1;

    
    switch(hero.GetDirection()){
    case LEFT:
      break;
    case RIGHT:
      if(enemy.stunned){
	enemy.SetX(50);
	enemy.stunned = false;
	enemy.currentState = STUN;
      }
      break;
    default:
      break;
    }
  }else{
    enemy.SetColor(WHITE);
  }
}
