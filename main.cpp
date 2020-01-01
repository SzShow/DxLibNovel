#include "DxLib.h"
#include "TextWriter.h"
#include <math.h>

#define CHAR_SIZE 24

using namespace std;

string str =
	"¹Ş°ÑÌßÛ¸Ş×Ñ‚ğK“¾‚·‚é‚½‚ß‚Ìˆê”Ô‚Ì‹ß“¹‚Í‚Æ‚É‚©‚­‘òRÌßÛ¸Ş×Ñ‚ğ‘g‚Ş‚±‚Æ‚Å‚ ‚é";

int initialize(void)
{
	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

int loop(void)
{
	TextWriter textWriter = TextWriter(CHAR_SIZE);
	textWriter << str;
	int frameCount = 0;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		{
			int color = (int)(sin(frameCount / 100.0) * 80.0 + 125.0);
			DrawBox(0, 0, 640, 480, GetColor(color, 0, 0), TRUE);
			frameCount++;
		}

		textWriter.WriteChar();

		ScreenFlip();

		if (textWriter.IsComplete() == TRUE)
		{
			break;
		}
	}

	return 0;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	initialize();
	loop();

	DxLib_End();
	return 0;
}