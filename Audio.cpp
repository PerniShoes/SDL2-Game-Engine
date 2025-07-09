#include "Audio.h"

Audio::Audio()
{
    SetPaths();

};

Audio::~Audio()
{
    Free();
    Mix_Quit();
};

void Audio::SetPaths()
{
    // This needs to be better. Too much repetition 
    // No need to static_cast. C-style enum to int is safe)

    // Pong:
    m_SoundPath[int(PaddleSoundEffect)] = "Audio/Sounds/PaddleEffect.mp3";
    m_SoundPath[int(Score_sound)] = "Audio/Sounds/Score_sound.mp3";
    m_SoundPath[int(WallHitSoundEffect)] = "Audio/Sounds/WallEffect.mp3";

    m_MusicPath[int(PressPlayMusic)] = "Audio/Music/PressPlayMusic.mp3";
    m_MusicPath[int(EisenfunkPong)] = "Audio/Music/EisenfunkPong.mp3";
    m_MusicPath[int(HeliosLexica)] = "Audio/Music/Helios.mp3";
    //


    m_MusicPath[int(DefaultMusic)] = "Audio/Music/beat.wav";   
    m_MusicPath[int(HolyF)] = "Audio/Music/HolyFuck.mp3";  
    m_MusicPath[int(Pandemonium)] = "Audio/Music/Pandemonium.mp3";
    m_MusicPath[int(GimmeLove)] = "Audio/Music/GimmeLove.mp3";
    m_MusicPath[int(AreYouGonnaBeMyGirl)] = "Audio/Music/AreYouGonnaBeMyGirl.mp3";
    m_MusicPath[int(WezPigulke)] = "Audio/Music/WezPigulke.mp3";
    m_MusicPath[int(Gold)] = "Audio/Music/Gold.mp3";
    m_MusicPath[int(ThatsWhatILike)] = "Audio/Music/ThatsWhatILike.mp3";
    m_MusicPath[int(ImComing)] = "Audio/Music/ImComing.mp3";
    m_MusicPath[int(Hold)] = "Audio/Music/Hold.mp3";
    m_MusicPath[int(BeautifulMadness)] = "Audio/Music/BeautifulMadness.mp3";
    m_MusicPath[int(ALittleMessedUp)] = "Audio/Music/ALittleMessedUp.mp3";

  

};

void Audio::AdjustSoundVolume(int soundId, int volumePercentage)const
{
    float volumeMult{ (float)volumePercentage/100.0f };

    switch (soundId)
    {
    case (int)PaddleSoundEffect:
        Mix_MasterVolume(int(m_Volume.PaddleSoundEffect * volumeMult));
        break;
    case(int)Score_sound:
        Mix_MasterVolume(int(m_Volume.Score_sound * volumeMult));
        break;
    case (int)WallHitSoundEffect:
        Mix_MasterVolume(int(m_Volume.WallHitSoundEffect * volumeMult));
        break;
    }
}
void Audio::PlaySound(SoundsList soundId, int volumePercentage, int loopAmount, int channel) const
{
    // No point in calling it every frame. Call it once or make it so you can pass volume in
    AdjustSoundVolume((int)soundId, volumePercentage);
    Mix_PlayChannel(channel, m_Sound[static_cast<int>(soundId)], loopAmount);
    
};

void Audio::AdjustVolume(int musicId, int volumePercentage) const
{
    float volumeMult{ (float)volumePercentage / 100.0f };

    switch (musicId)
    {
    case 0:  Mix_VolumeMusic(int(m_Volume.DefaultMusic * volumeMult));      break;
    case 1:  Mix_VolumeMusic(int(m_Volume.HolyF * volumeMult));             break;
    case 2:  Mix_VolumeMusic(int(m_Volume.Pandemonium * volumeMult));       break;
    case 3:  Mix_VolumeMusic(int(m_Volume.GimmeLove * volumeMult));         break;
    case 4:  Mix_VolumeMusic(int(m_Volume.AreYouGonnaBeMyGirl* volumeMult));break;
    case 5:  Mix_VolumeMusic(int(m_Volume.WezPigulke * volumeMult));        break;
    case 6:  Mix_VolumeMusic(int(m_Volume.ALittleMessedUp * volumeMult));   break;
    case 7:  Mix_VolumeMusic(int(m_Volume.BeautifulMadness * volumeMult));  break;
    case 8:  Mix_VolumeMusic(int(m_Volume.ImComing * volumeMult));          break;
    case 9:  Mix_VolumeMusic(int(m_Volume.ThatsWhatILike * volumeMult));    break;
    case 10: Mix_VolumeMusic(int(m_Volume.Gold * volumeMult));              break;
    case 11: Mix_VolumeMusic(int(m_Volume.Hold * volumeMult));              break;
    case 12: Mix_VolumeMusic(int(m_Volume.EisenfunkPong * volumeMult));     break;
    case 13: Mix_VolumeMusic(int(m_Volume.HeliosLexica * volumeMult));      break;
    case 14: Mix_VolumeMusic(int(m_Volume.PressPlayMusic * volumeMult));    break;

    default: Mix_VolumeMusic(int(64 * volumeMult)); break;
    }


};
void Audio::PlayMusic(MusicList musicId, int volumePercentage, int loopAmount)
{

    if (musicId == StopMusic)
    {
       Mix_HaltMusic();
    }
    else if (musicId == PauseOrResume)
    {
        if (Mix_PausedMusic())
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_PauseMusic();
        }
    }
    else if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(m_Music[static_cast<int>(musicId)], loopAmount);
        AdjustVolume(static_cast<int>(musicId), volumePercentage);
    }
     
};

bool Audio::LoadAudio()
{
    bool success = true;

    for (int i{ 0 }; i < static_cast<int>(TotalSounds); i++)
    {
        success = LoadWAV(m_Sound[i], m_SoundPath[i]);
    }

    for (int i{ 0 }; i < static_cast<int>(TotalMusic); i++)
    {
        success = LoadMusic(m_Music[i], m_MusicPath[i]);
    }


    return success;
};
bool Audio::LoadWAV(Mix_Chunk* &soundStorage, const std::string& path) 
{   
    bool success = true;
    soundStorage = Mix_LoadWAV(path.c_str());   
    if (soundStorage == NULL)   
    {
        Debug::Print("Failed to load ", path ," sound effect! SDL_mixer Error: %s ", Mix_GetError(), "\n");
        success = false;
    }
    return success;
};
bool Audio::LoadMusic(Mix_Music* &musicStorage, const std::string& path)
{
    bool success = true;
    musicStorage = Mix_LoadMUS(path.c_str());
    if (musicStorage == NULL)
    {
        Debug::Print("Failed to load ", path, " music! SDL_mixer Error: %s ", Mix_GetError(), "\n");
        success = false;
    }
    return success;

};


void Audio::Free()
{
    for (int i{ 0 }; i < static_cast<int>(TotalSounds); i++)
    {
        Mix_FreeChunk(m_Sound[i]);
        m_Sound[i] = NULL;
    };

    for (int i{ 0 }; i < static_cast<int>(TotalMusic); i++)
    {
        Mix_FreeMusic(m_Music[i]);
        m_Music[i] = NULL;
    };

};
