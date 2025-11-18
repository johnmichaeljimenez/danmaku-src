#include "menu.h"

static Texture2D gameLogo;
static float gameLogoTweenScale;
static float gameLogoTweenAlpha;
static float gameLogoTweenMain;

static float tweenDelay;
static bool doneTween;

void MenuStart()
{
    FadeScreen(0, true, NULL);

	gameLogo = GetSprite("ui/game-logo");
	tweenDelay = 0;
	doneTween = false;

	TweenManager_AddFloatFrom(&gameLogoTweenMain, 0.1f, 1.0f, 0.3f, EASING_EASEOUTQUAD, "MenuGameLogoMain", NULL);
}

void MenuUpdate(float dt)
{
	if (!doneTween)
	{
		if (tweenDelay >= 0.3f)
		{
			TweenManager_AddFloatFrom(&gameLogoTweenScale, 1.0f, 1.5f, 1.0f, EASING_EASEOUTQUAD, "MenuGameLogoScale", NULL);
			TweenManager_AddFloatFrom(&gameLogoTweenAlpha, 0.3f, 0.0f, 0.5f, EASING_EASEINOUTQUAD, "MenuGameLogoAlpha", NULL);
			doneTween = true;
		}

		tweenDelay += dt;
	}

	if (IsKeyPressed(KEY_SPACE) || IsPointerHold(dt))
		GoToGame(0);
}

void MenuRender(float dt)
{
	DrawText("[HOLD] to start", 250, VIRTUAL_HEIGHT * 0.8, 30, WHITE);

	DrawSpriteScaled(gameLogo, (Vector2){VIRTUAL_WIDTH * 0.5f, 400}, 0, Lerp(1, 0.35, gameLogoTweenMain), (Color){255, 255, 255, 255 * gameLogoTweenMain});

	BeginBlendMode(BLEND_ADDITIVE);
	DrawSpriteScaled(gameLogo, (Vector2){VIRTUAL_WIDTH * 0.5f, 400}, 0, 0.35 * gameLogoTweenScale, (Color){255, 255, 255, 255 * gameLogoTweenAlpha});
	EndBlendMode();
}

void MenuCleanup()
{
}
