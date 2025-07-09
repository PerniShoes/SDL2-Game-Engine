#pragma once // NOLINT
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Debug.h"

enum class SoundsList
{
	PaddleSoundEffect = 0,
	Score_sound = 1,
	WallHitSoundEffect = 2,

	TotalSounds = 3

};

enum class MusicList
{
	StopMusic = -2,
	PauseOrResume = -1,

	DefaultMusic = 0,
	HolyF = 1,
	Pandemonium = 2,
	GimmeLove = 3,
	AreYouGonnaBeMyGirl = 4,
	WezPigulke = 5,
	ALittleMessedUp = 6,
	BeautifulMadness = 7,
	ImComing = 8,
	ThatsWhatILike = 9,
	Gold = 10,
	Hold = 11,

	// Pong:
	EisenfunkPong = 12,
	HeliosLexica = 13,
	PressPlayMusic = 14,


	TotalMusic = 15
};

using enum MusicList;
using enum SoundsList;

class Audio
{
public:
	Audio();
	~Audio();

	Audio(const Audio&) = delete;
	Audio& operator = (const Audio&) = delete;

	// ManualAdjustVolume(music/soundId Id, int volume);
	// Add some "music player" functionality, Ui or just "next, previous, skip, etc"

	bool LoadAudio();
	void PlaySound(SoundsList soundId, int volumePercentage = 100, int loopAmount = 0, int channel = -1)const;
	void PlayMusic(MusicList musicId, int volumePercentage = 100, int loopAmount = 10);

	struct VolumeList
	{
		const int DefaultMusic = 16;
		const int HolyF = 48;
		const int Pandemonium = 16;
		const int GimmeLove = 32;
		const int AreYouGonnaBeMyGirl = 88;
		const int WezPigulke = 32;
		const int ALittleMessedUp = 32;
		const int BeautifulMadness = 64;
		const int ImComing = 32;
		const int ThatsWhatILike = 48; 
		const int Gold = 32;
		const int Hold = 32;
		const int EisenfunkPong = 128;
		const int HeliosLexica = 128;
		const int PressPlayMusic = 128;

		const int PaddleSoundEffect = 128;
		const int Score_sound = 64;
		const int WallHitSoundEffect = 128;

	}m_Volume;

private:

	void AdjustSoundVolume(int soundId, int volumePercentage)const;
	void AdjustVolume(int musicId, int volumePercentage) const;
	void SetPaths();
	bool LoadWAV(Mix_Chunk*& soundStorage, const std::string& path);
	bool LoadMusic(Mix_Music*& musicStorage, const std::string& path);
	void Free();

	Mix_Chunk* m_Sound[(int)SoundsList::TotalSounds]{};	
	const char* m_SoundPath[(int)TotalSounds]{};

	Mix_Music* m_Music[(int)MusicList::TotalMusic]{};		
	const char* m_MusicPath[(int)TotalMusic]{};	

};

