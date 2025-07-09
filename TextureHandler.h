#pragma once // NOLINT
#include <string>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
	
#include "TimeHandler.h"
#include "Window.h"
#include "Renderer.h"
#include "Debug.h"

	// Just hold all the textures here and then if I want to change one I will call Function(textureId, arg);
	// Most of the code can be reused, I just need to implement the usage of an array with all of the textures

	// I can also have functions like: renderALL(); setAlphaAll(); setRotateALL(); If I wanted to apply something to all of them at once, 
	// like just reseting to default or spinning the whole scene or making the whole scene bright/dark or w/e
	// 
	// 
	// Or if feeling fancy (DON'T xD!) I could have things like setAlphaMany(textureId listOfIds); to apply something to many at once
	
	

enum class TextureList
{
	// With hitbox:

	FireProjectiles = 0,
	TimeText = 1,
	PongBall = 2,
	PongPlayer = 3,
	EnemyPong = 4,
	
	// Without hitbox:

	QuitText = 5,
	BlueEffects = 6,
	BackGroundSpace = 7,
	ScoreText = 8,
	BackGroundBlackHole = 9,
	BackgroundDazzlingForest = 10,
	RestartText = 11,
	EndScreenText = 12,

	TotalTextures = 13
	
};

enum class TexturesWithHitBox
{
	HB_FireProjectiles = 0,
	HB_TimeText = 1,
	HB_PongBall = 2,
	HB_PongPlayer = 3,
	HB_EnemyPong = 4,

	HB_TotalTextures = 5

};
using enum TexturesWithHitBox;

using enum TextureList;

class TextureHandler
{
public:

	TextureHandler();
	~TextureHandler();

	TextureHandler(const TextureHandler&) = delete;
	TextureHandler& operator = (const TextureHandler&) = delete;

	bool loadTextures();

	void render(TextureList textureId, bool stretch = false) const;
	void setPos(TextureList textureId, SDL_Point currentPos = SDL_Point{ 0,0 }, SDL_Point change = SDL_Point{ 0,0 });
	void transform(TextureList textureId, 
		float scale = 1.0f, SDL_RendererFlip flipType = SDL_FLIP_NONE, 
		double angle = 0.0, SDL_Point* center = NULL);


	void setScaleAll(float ratio = 1.0f);
	void setScale(TextureList textureId, float ratio = 1.0f); // Add functionality to only scale x or y
	void setColor(TextureList textureId, SDL_Color color);
	void setAlpha(TextureList textureId, Uint8 alpha) const;
	void setBlendMode(TextureList textureId, SDL_BlendMode blendMode) const;

	void setFlip(TextureList textureId, SDL_RendererFlip flipType = SDL_FLIP_NONE);
	void setRotate(TextureList textureId, double angle = 0.0, SDL_Point* center = NULL);

	// SpriteNumber is counted from Left to right e.g:
	// 0 1 2 3 4 
	// 5 6 7 8 9
	void animate(TextureList textureId, int spriteNumber, float speed = 12.0f);
	void changeText(TextureList textureId, std::string text); // Can add Color and Font changeability	 
	void setCurrentClip(TextureList textureId, int frameNumber);
	SDL_Rect* getRect(TextureList textureId);

private:

	// Init:
	void setAllPaths();
	void setPath(TextureList textureId, const char* text);
	void setClipProp(TextureList textureId, SDL_Rect clipProp);

	bool createTextureFromSurface(TextureList textureId);

	void initText(TextureList textureId, const char* text, int ptSize = 24, SDL_Color color = { 255, 255, 255 });
	void setFont(TextureList textureId);
	//Animation:
	 
	// Even rows with even framesPerAnim OR just one row
	void setClipList(TextureList textureId, int rowAmount, int columnAmount, int framesPerAnimation);


	void freeResources();


	// Not every texture will be animated/text etc. Right now every texture has all the variables -> not good
	struct TextureProperties
	{
		// Base propeties
		SDL_Rect m_Rect; // Pos + size
		float m_Scale;	
		SDL_Rect* m_CurrentClip;
		std::string m_Path;
		
		// Orientation
		SDL_Point* m_RotationCenter;
		double m_RotationDegrees;
		SDL_RendererFlip m_FlipType;

		// Colors
		SDL_Color m_Color;
		Uint8 m_Alpha;

		// Animation 
		SDL_Rect m_ClipProp;
		int m_FrameSlow;
		std::vector<SDL_Rect> m_ClipList;
		int m_CurrentFrame;
		int m_FramesPerAnim;

		float m_LastTick;
		float m_FrameCounter;

	
		// Text
		bool m_IsText;
		TTF_Font* m_Font;
		std::string m_Text;
		int m_PtSize;

	};	

	TextureProperties m_TextureProperties[static_cast<int>(TotalTextures)]{};
	SDL_Texture* m_Texture[static_cast<int>(TotalTextures)]{};

	TimeHandler m_Time;
	
	SDL_Rect m_PlaceHolderRect;
};

