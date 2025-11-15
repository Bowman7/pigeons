#ifndef AUDIO_HPP
#define AUDIO_HPP


#include"../raylib/raylib.h"

class AudioMix{
public:
  void Init(){
    
    InitAudioDevice();

    backgroundMusic = LoadMusicStream("./asset/background-music.mp3");
    backgroundMusic.looping = true;
    SetMusicVolume(backgroundMusic,0.8);

    s_hit = LoadSound("./asset/hit.mp3");
    SetSoundVolume(s_hit,6.0);

    hit_air = LoadSound("./asset/punch-air.mp3");
    SetSoundVolume(hit_air,0.3);
    
    PlayMusicStream(backgroundMusic);
  }

  void Update(){
    UpdateMusicStream(backgroundMusic);
  }
  void DeInit(){
    UnloadSound(s_hit);
    UnloadMusicStream(backgroundMusic);
  }

  void PlayHitSound(){
    PlaySound(s_hit);
  }
  void PlayHitAir(){
    PlaySound(hit_air);
  }

private:
  Music backgroundMusic;
  Wave w_hit;
  Sound s_hit;
  Sound hit_air;
};

#endif
