
#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include "DxLib.h"
#include <string>

#define STRBUF_WIDTH 24
#define STRBUF_HEIGHT 20

enum ControlChar :char
{
	ControlChar_Return = '@',
	ControlChar_KeyWait = 'B',
	ControlChar_End = 'E',
	ControlChar_Clear = 'C',
};

class TextWriter
{
public:
	TextWriter(int fontSize);
	~TextWriter(void);

	int InputString(const std::string& input);
	int WriteChar(void);

	int IsComplete(void);

	TextWriter& operator<<(const std::string& str);

protected:
	int InsertChar2Buffer(void);
	int ReadControl(void);
	int OutputChar(void);
	int ReturnText(void);

	int ClearStringBuffer(void);
	int PushStringStream(int num);

	int GetCurrentCharBytes(void);
	int IsStreamEmpty(void);

protected:
	char _charBuffer[3];
	char _stringBuffer[STRBUF_HEIGHT][STRBUF_WIDTH*2+1];
	std::string _stringStream;	//Œã‚Å\‘¢‘Ì‚É‚·‚éH

	int _isComplete;
	int _isKeyWait;

	int _fontSize;

	int _rowIndex;
	int _columnIndex;
	int _strIndex;

	int _drawPositionX;
	int _drawPositionY;

	int _timeCounter;
	int _frameTime;
	int _frameCounter;
	
};


#endif
