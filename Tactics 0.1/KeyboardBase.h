#pragma once
#include "KeyboardDefines.h"

class KeyboardBase
{
public:
	KeyboardBase();
	virtual ~KeyboardBase();

	virtual void setKey( KEYTYPE key, bool condition );
	virtual bool getKey( KEYTYPE key ) const;
	virtual bool getKeyPressedThisFrame( KEYTYPE key ) const;
	virtual bool getKeyReleasedThisFrame( KEYTYPE key ) const;
	virtual void update(); // MUST CALL THIS AT END OF EACH FRAME!
	/***********************************************/


protected:
	KeyboardBase( int x );

	bool * arrCurrentKeyState;
	bool * arrPressedThisFrame;
	bool * arrReleasedThisFrame;
};