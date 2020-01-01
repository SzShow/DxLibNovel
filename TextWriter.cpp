#include "TextWriter.h"

using namespace std;

char Text[][256] =
{
	"　ｹﾞｰﾑﾌﾟﾛｸﾞﾗﾑを習得するための一番の近道はとにかく沢山ﾌﾟﾛｸﾞﾗﾑを組む",
	"ことである。B" ,
	"@　ﾌﾟﾛｸﾞﾗﾑの参考書にはゲームのﾌﾟﾛｸﾞﾗﾑの方法なんて何も書かれていない、B",
	"変数、B配列、B関数、Bﾙｰﾌﾟ、B条件分岐…Bこれらすべての説明はゲームで何に使うか",
	"なんてどこにも書いていない、Bせいぜい住所録を題材にした例がある程度である。B" ,
	"C　ﾌﾟﾛｸﾞﾗﾑは習うより慣れろなのでﾌﾟﾛｸﾞﾗﾑを組むに当たって少しでも知識が",
	"つけば後はそこからは掘り下げ、広げていけば良いわけで、Bﾌﾟﾛｸﾞﾗﾑの参考書を",
	"読んでいて少しでも何か出来るような気がしたらそこでとにかくﾌﾟﾛｸﾞﾗﾑ",
	"を打ってみることが大事である。E",
};

// public

TextWriter::TextWriter(int fontSize) :
	_fontSize(fontSize),
	_isComplete(0),
	_isKeyWait(0),
	_rowIndex(0),
	_columnIndex(0),
	_strIndex(0),
	_drawPositionX(0),
	_drawPositionY(0),
	_timeCounter(0),
	_frameTime(0),
	_frameCounter(0)
{

	for (int index = 0; index < 3; index++)
	{
		_charBuffer[index] = 0;
	}

	ClearStringBuffer();
}


TextWriter::~TextWriter()
{
	delete &_stringStream;
}


int TextWriter::InputString(const string& input)
{
	_stringStream += input;
	return 0;
}

int TextWriter::WriteChar()
{
	if (_isKeyWait)
	{
		if (CheckHitKeyAll())
		{
			_isKeyWait = FALSE;
		}
	}
	else
	{
		ReadControl(Text[_rowIndex][_columnIndex]);
		if (Text[_rowIndex][_columnIndex] == '\0')
		{
			_rowIndex++;
			_columnIndex = 0;
		}
	}

	//_timeCounter = 2;
	OutputChar();

	return 0;
}


int TextWriter::IsComplete()
{
	return _isComplete;
}


TextWriter& TextWriter::operator<<(const string& str)
{
	this->_stringStream += str;
	return *this;
}


// protected

int TextWriter::ReadControl(char target)
{
	switch (target)
	{
	case ControlChar_Return:
		ReturnText();
		_columnIndex++;
		break;

	case ControlChar_KeyWait:
		_isKeyWait = TRUE;
		_columnIndex++;
		break;

	case ControlChar_End:
		_isComplete = TRUE;
		_columnIndex++;
		break;

	case ControlChar_Clear:
		ClearStringBuffer();
		_drawPositionX = 0;
		_drawPositionY = 0;
		_columnIndex++;
		break;

	default:
		Insert2Buffer();
		if (_drawPositionX >= STRBUF_WIDTH)
		{
			ReturnText();
		}
		break;
	}
	return 0;
}

int TextWriter::Insert2Buffer(void)
{
	int bytes = GetCharBytes(
		DX_CHARCODEFORMAT_SHIFTJIS, 
		&Text[_rowIndex][_columnIndex]);

	for (size_t i = 0; i < bytes; i++)
	{
		_stringBuffer[_drawPositionY][_drawPositionX + i] =
			Text[_rowIndex][_columnIndex + i];
	}
	_columnIndex += bytes;
	_drawPositionX += bytes;

	return 0;
}

int TextWriter::OutputChar(void)
{
	for (size_t i = 0; i < STRBUF_HEIGHT; i++)
	{
		DrawString(8, i * _fontSize, _stringBuffer[i], GetColor(255, 255, 255));
	}

	return 0;
}

int TextWriter::ReturnText(void)
{

	_drawPositionY++;
	_drawPositionX = 0;

	if (_drawPositionY  >= STRBUF_HEIGHT)
	{
		for (size_t i = 1; i < STRBUF_HEIGHT; i++)
		{
			for (size_t j = 0; j < STRBUF_WIDTH*2; j++)
			{
				_stringBuffer[i - 1][j] = _stringBuffer[i][j];
			}
		}

		_drawPositionY--;
	}

	return 0;
}

int TextWriter::ClearStringBuffer(void)
{
	for (int col = 0; col < STRBUF_HEIGHT; col++)
	{
		for (int row = 0; row < STRBUF_WIDTH * 2 + 1; row++)
		{
			_stringBuffer[col][row] = 0;
		}
	}

	return 0;
}
