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
	switch (_stringStream[0]) // �X�g���[���̐擪���Q��
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
