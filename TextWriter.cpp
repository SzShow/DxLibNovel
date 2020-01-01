#include "TextWriter.h"

using namespace std;

char Text[][256] =
{
	"�@�ް���۸��т��K�����邽�߂̈�Ԃ̋ߓ��͂Ƃɂ�����R��۸��т�g��",
	"���Ƃł���BB" ,
	"@�@��۸��т̎Q�l���ɂ̓Q�[������۸��т̕��@�Ȃ�ĉ���������Ă��Ȃ��AB",
	"�ϐ��AB�z��AB�֐��ABٰ�߁AB��������cB����炷�ׂĂ̐����̓Q�[���ŉ��Ɏg����",
	"�Ȃ�Ăǂ��ɂ������Ă��Ȃ��AB���������Z���^���ނɂ����Ⴊ������x�ł���BB" ,
	"C�@��۸��т͏K����芵���Ȃ̂���۸��т�g�ނɓ������ď����ł��m����",
	"���Ό�͂�������͌@�艺���A�L���Ă����Ηǂ��킯�ŁAB��۸��т̎Q�l����",
	"�ǂ�ł��ď����ł������o����悤�ȋC�������炻���łƂɂ�����۸���",
	"��ł��Ă݂邱�Ƃ��厖�ł���BE",
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
