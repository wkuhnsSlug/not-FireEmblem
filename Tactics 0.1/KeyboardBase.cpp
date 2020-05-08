#include "KeyboardBase.h"
#include <cstdlib> // needed for calloc and free
#include <cstring> // needed for memset
#include <cassert>

KeyboardBase::KeyboardBase()
{
	arrPressedThisFrame = (bool*)calloc( KEYTYPE_MAX_SIZE, sizeof( bool ) );
	arrReleasedThisFrame = (bool*)calloc( KEYTYPE_MAX_SIZE, sizeof( bool ) );
	arrCurrentKeyState = (bool*)calloc( KEYTYPE_MAX_SIZE, sizeof( bool ) );
	assert( arrPressedThisFrame && arrCurrentKeyState && arrReleasedThisFrame );
}

KeyboardBase::~KeyboardBase()
{
	free( arrPressedThisFrame );
	free( arrReleasedThisFrame );
	free( arrCurrentKeyState );
}


KeyboardBase::KeyboardBase( int x )
{
	// only called in child
}
/***********************************************/


void KeyboardBase::setKey( KEYTYPE key, bool condition )
{
	//if ( condition == true )
	//{
	//	if ( arrCurrentKeyState[key] == true ) // key was down on last frame
	//	{
	//		return;
	//	}
	//	// key was up last frame
	//	arrCurrentKeyState[key] = true;
	//	arrPressedThisFrame[key] = true;
	//}
	//else
	//{
	//	arrCurrentKeyState[key] = false;
	//}
	if ( condition == true && arrCurrentKeyState[key] == false ) // key up on last frame and down on this
	{
		arrPressedThisFrame[key] = true;
	}
	else if ( condition == false && arrCurrentKeyState[key] == true ) // key down on last frame and up on this
	{
		arrReleasedThisFrame[key] = true;
	}
	arrCurrentKeyState[key] = condition;
}

bool KeyboardBase::getKey( KEYTYPE key ) const
{
	return arrCurrentKeyState[key];
}

bool KeyboardBase::getKeyPressedThisFrame( KEYTYPE key ) const
{
	return arrPressedThisFrame[key];
}

bool KeyboardBase::getKeyReleasedThisFrame( KEYTYPE key ) const
{
	return arrReleasedThisFrame[key];
}

void KeyboardBase::update() // MUST CALL THIS AT END OF EACH FRAME!
{
	memset( arrPressedThisFrame, 0, sizeof( bool ) * KEYTYPE_MAX_SIZE );
	memset( arrReleasedThisFrame, 0, sizeof( bool ) * KEYTYPE_MAX_SIZE );
	/*for ( int i = 0; i < KEYTYPE_MAX_SIZE; i++ )
	{
	arrPressedThisFrame[i] = false;
	}*/
}