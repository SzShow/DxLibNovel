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

	if (_isKeyWait ||
	IsStreamEmpty())
	{
		if (CheckHitKeyAll())
		{
			_isKeyWait = FALSE;
		}
	}
	else
	{
		ReadControl();
	}

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

int TextWriter::InsertChar2Buffer(void)
{
	int bytes = GetCharBytes(
		DX_CHARCODEFORMAT_SHIFTJIS,
		&_stringStream[0]
	);

	for (size_t i = 0; i < bytes; i++)
	{
		_stringBuffer[_drawPositionY][_drawPositionX+i] =
			_stringStream[i];
	}
	_drawPositionX+=bytes;

	return 0;
}

int TextWriter::ReadControl(void)
{
	int bytes;
	switch (_stringStream[0]) // ストリームの先頭を参照
	{
	case ControlChar_Return:
		ReturnText();
		bytes = 1;
		break;

	case ControlChar_KeyWait:
		_isKeyWait = TRUE;
		bytes = 1;
		break;

	case ControlChar_End:
		_isComplete = TRUE;
		bytes = 1;
		break;

	case ControlChar_Clear:
		ClearStringBuffer();
		_drawPositionX = 0;
		_drawPositionY = 0;
		bytes = 1;
		break;

	default:
		// Insert2Buffer();
		InsertChar2Buffer();
		bytes = GetCurrentCharBytes();
		if (_drawPositionX >= STRBUF_WIDTH)
		{
			ReturnText();
		}
		break;
	}

	PushStringStream(bytes);
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

int TextWriter::PushStringStream(int num)
{
	for (size_t i = 0; i < num; i++)
	{
		_stringStream.erase(_stringStream.begin()+0);
	}

	return 0;
}

int TextWriter::GetCurrentCharBytes(void)
{
	int bytes = GetCharBytes(
		DX_CHARCODEFORMAT_SHIFTJIS,
		&_stringStream[0]
	);

	return bytes;
}

int TextWriter::IsStreamEmpty(void)
{
	if(_stringStream.size()<=0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
