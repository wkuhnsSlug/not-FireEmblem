#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Extras.h"
#include "Game.h"
#include "KeyboardBase.h"
#include "Mouse.h"
#include <cstdlib>

static KeyboardBase kbd;
static MouseBase mouse;

void MsgProc( sf::Event event );

//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
int main()
{
	srand( (unsigned)time( NULL ) ); // seeding rand

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::Image img;
	img.loadFromFile( "Assets\\Icon3.png" );
	sf::RenderWindow window( sf::VideoMode( SCREENWIDTH, SCREENHEIGHT ), "Red Dawn", 
		/** /sf::Style::Fullscreen,/**/
		/**/sf::Style::Close | sf::Style::Titlebar,/**/
		settings );
	window.setIcon( img.getSize().x, img.getSize().y, img.getPixelsPtr() );
	//window.setFramerateLimit( 30 );
	window.setVerticalSyncEnabled( true );
	window.setMouseCursorVisible( false );

	Game game( window, kbd, mouse );

	MSG msg = { 0 };
	sf::Event event;
	while ( window.isOpen() )
	{
		while ( window.pollEvent( event ) )
		{
			if ( event.type == sf::Event::Closed
				|| ( event.type == sf::Event::EventType::KeyPressed  && event.key.code == sf::Keyboard::Escape ) )
			{
				window.close();
				break;
			}
			MsgProc( event );
		}


		if ( !game.execute() )
		{
			window.close();
			break;
		}
		kbd.update();
		mouse.update();
	}

	return 0;
}

void MsgProc( sf::Event event )
{

	switch ( event.type )
	{
		/*case WM_DESTROY:
		PostQuitMessage( 0 );
		break;*/
	case sf::Event::EventType::KeyPressed:
		switch ( event.key.code )
		{
			// alphabet
		case sf::Keyboard::A:
			kbd.setKey( KBD_A, true );
			break;
		case sf::Keyboard::B:
			kbd.setKey( KBD_B, true );
			break;
		case sf::Keyboard::C:
			kbd.setKey( KBD_C, true );
			break;
		case sf::Keyboard::D:
			kbd.setKey( KBD_D, true );
			break;
		case sf::Keyboard::E:
			kbd.setKey( KBD_E, true );
			break;
		case sf::Keyboard::F:
			kbd.setKey( KBD_F, true );
			break;
		case sf::Keyboard::G:
			kbd.setKey( KBD_G, true );
			break;
		case sf::Keyboard::H:
			kbd.setKey( KBD_H, true );
			break;
		case sf::Keyboard::I:
			kbd.setKey( KBD_I, true );
			break;
		case sf::Keyboard::J:
			kbd.setKey( KBD_J, true );
			break;
		case sf::Keyboard::K:
			kbd.setKey( KBD_K, true );
			break;
		case sf::Keyboard::L:
			kbd.setKey( KBD_L, true );
			break;
		case sf::Keyboard::M:
			kbd.setKey( KBD_M, true );
			break;
		case sf::Keyboard::N:
			kbd.setKey( KBD_N, true );
			break;
		case sf::Keyboard::O:
			kbd.setKey( KBD_O, true );
			break;
		case sf::Keyboard::P:
			kbd.setKey( KBD_P, true );
			break;
		case sf::Keyboard::Q:
			kbd.setKey( KBD_Q, true );
			break;
		case sf::Keyboard::R:
			kbd.setKey( KBD_R, true );
			break;
		case sf::Keyboard::S:
			kbd.setKey( KBD_S, true );
			break;
		case sf::Keyboard::T:
			kbd.setKey( KBD_T, true );
			break;
		case sf::Keyboard::U:
			kbd.setKey( KBD_U, true );
			break;
		case sf::Keyboard::V:
			kbd.setKey( KBD_V, true );
			break;
		case sf::Keyboard::W:
			kbd.setKey( KBD_W, true );
			break;
		case sf::Keyboard::X:
			kbd.setKey( KBD_X, true );
			break;
		case sf::Keyboard::Y:
			kbd.setKey( KBD_Y, true );
			break;
		case sf::Keyboard::Z:
			kbd.setKey( KBD_Z, true );
			break;
			// numbers
		case sf::Keyboard::Num0:
			kbd.setKey( KBD_0, true );
			break;
		case sf::Keyboard::Num1:
			kbd.setKey( KBD_1, true );
			break;
		case sf::Keyboard::Num2:
			kbd.setKey( KBD_2, true );
			break;
		case sf::Keyboard::Num3:
			kbd.setKey( KBD_3, true );
			break;
		case sf::Keyboard::Num4:
			kbd.setKey( KBD_4, true );
			break;
		case sf::Keyboard::Num5:
			kbd.setKey( KBD_5, true );
			break;
		case sf::Keyboard::Num6:
			kbd.setKey( KBD_6, true );
			break;
		case sf::Keyboard::Num7:
			kbd.setKey( KBD_7, true );
			break;
		case sf::Keyboard::Num8:
			kbd.setKey( KBD_8, true );
			break;
		case sf::Keyboard::Num9:
			kbd.setKey( KBD_9, true );
			break;
			// special characters
		case sf::Keyboard::Tilde:
			kbd.setKey( KBD_TILDE, true );
			break;
		case sf::Keyboard::Dash:
			kbd.setKey( KBD_DASH, true );
			break;
		case sf::Keyboard::Equal:
			kbd.setKey( KBD_EQUALS, true );
			break;
		case sf::Keyboard::LBracket:
			kbd.setKey( KBD_OPEN_BRACKET, true );
			break;
		case sf::Keyboard::RBracket:
			kbd.setKey( KBD_CLOSED_BRACKET, true );
			break;
		case sf::Keyboard::BackSlash:
			kbd.setKey( KBD_BACKSLASH, true );
			break;
		case sf::Keyboard::SemiColon:
			kbd.setKey( KBD_SEMICOLON, true );
			break;
		case sf::Keyboard::Quote:
			kbd.setKey( KBD_APOSTROPHE, true );
			break;
		case sf::Keyboard::Comma:
			kbd.setKey( KBD_COMMA, true );
			break;
		case sf::Keyboard::Period:
			kbd.setKey( KBD_PERIOD, true );
			break;
		case sf::Keyboard::Slash:
			kbd.setKey( KBD_FORWARD_SLASH, true );
			break;
			// modifiers
		case sf::Keyboard::Return:
			kbd.setKey( KBD_ENTER, true );
			break;
			/*case VK_SHIFT:
			kbd.setKey( KBD_SHIFT, true );
			break;*/
		case sf::Keyboard::LShift:
			kbd.setKey( KBD_SHIFT_LEFT, true );
			kbd.setKey( KBD_SHIFT, true );
			break;
		case sf::Keyboard::RShift:
			kbd.setKey( KBD_SHIFT_RIGHT, true );
			kbd.setKey( KBD_SHIFT, true );
			break;
			/*case VK_CONTROL:
			kbd.setKey( KBD_CONTROL, true );
			break;*/
		case sf::Keyboard::LControl:
			kbd.setKey( KBD_CONTROL_LEFT, true );
			kbd.setKey( KBD_CONTROL, true );
			break;
		case sf::Keyboard::RControl:
			kbd.setKey( KBD_CONTROL_RIGHT, true );
			kbd.setKey( KBD_CONTROL, true );
			break;
		case sf::Keyboard::LAlt:
		case sf::Keyboard::RAlt:
			kbd.setKey( KBD_ALT, true );
			break;
			/*case VK_CAPITAL:							SFML DOESN'T SUPPORT CAPSLOCK WHAT FUCKING IDIOTS THOUGHT THAT WAS A GOOD IDEA?!
			kbd.setKey( KBD_CAPSLOCK, true );
			break;*/
		case sf::Keyboard::Tab:
			kbd.setKey( KBD_TAB, true );
			break;
			// others
		case sf::Keyboard::Escape:
			kbd.setKey( KBD_ESCAPE, true );
			break;
		case sf::Keyboard::Space:
			kbd.setKey( KBD_SPACEBAR, true );
			break;
		case sf::Keyboard::BackSpace:
			kbd.setKey( KBD_BACKSPACE, true );
			break;
		case sf::Keyboard::Insert:
			kbd.setKey( KBD_INSERT, true );
			break;
		case sf::Keyboard::Delete:
			kbd.setKey( KBD_DELETE, true );
			break;
		case sf::Keyboard::Home:
			kbd.setKey( KBD_HOME, true );
			break;
		case sf::Keyboard::End:
			kbd.setKey( KBD_END, true );
			break;
		case sf::Keyboard::PageUp:
			kbd.setKey( KBD_PAGEUP, true );
			break;
		case sf::Keyboard::PageDown:
			kbd.setKey( KBD_PAGEDOWN, true );
			break;
			// arrow keys
		case sf::Keyboard::Up:
			kbd.setKey( KBD_ARROWUP, true );
			break;
		case sf::Keyboard::Down:
			kbd.setKey( KBD_ARROWDOWN, true );
			break;
		case sf::Keyboard::Left:
			kbd.setKey( KBD_ARROWLEFT, true );
			break;
		case sf::Keyboard::Right:
			kbd.setKey( KBD_ARROWRIGHT, true );
			break;
			// numpad
		case sf::Keyboard::Numpad0:
			kbd.setKey( KBD_NUMPAD_0, true );
			break;
		case sf::Keyboard::Numpad1:
			kbd.setKey( KBD_NUMPAD_1, true );
			break;
		case sf::Keyboard::Numpad2:
			kbd.setKey( KBD_NUMPAD_2, true );
			break;
		case sf::Keyboard::Numpad3:
			kbd.setKey( KBD_NUMPAD_3, true );
			break;
		case sf::Keyboard::Numpad4:
			kbd.setKey( KBD_NUMPAD_4, true );
			break;
		case sf::Keyboard::Numpad5:
			kbd.setKey( KBD_NUMPAD_5, true );
			break;
		case sf::Keyboard::Numpad6:
			kbd.setKey( KBD_NUMPAD_6, true );
			break;
		case sf::Keyboard::Numpad7:
			kbd.setKey( KBD_NUMPAD_7, true );
			break;
		case sf::Keyboard::Numpad8:
			kbd.setKey( KBD_NUMPAD_8, true );
			break;
		case sf::Keyboard::Numpad9:
			kbd.setKey( KBD_NUMPAD_9, true );
			break;
			/*case VK_DECIMAL:
			kbd.setKey( KBD_NUMPAD_DECIMAL, true );
			break;*/
		case sf::Keyboard::Add:
			kbd.setKey( KBD_NUMPAD_PLUS, true );
			break;
		case sf::Keyboard::Subtract:
			kbd.setKey( KBD_NUMPAD_MINUS, true );
			break;
		case sf::Keyboard::Multiply:
			kbd.setKey( KBD_NUMPAD_MULTIPLY, true );
			break;
		case sf::Keyboard::Divide:
			kbd.setKey( KBD_NUMPAD_DIVIDE, true );
			break;
			/*case VK_SEPARATOR:
			kbd.setKey( KBD_NUMPAD_DASH, true );
			break;*/
			// function keys
		case sf::Keyboard::F1:
			kbd.setKey( KBD_F1, true );
			break;
		case sf::Keyboard::F2:
			kbd.setKey( KBD_F2, true );
			break;
		case sf::Keyboard::F3:
			kbd.setKey( KBD_F3, true );
			break;
		case sf::Keyboard::F4:
			kbd.setKey( KBD_F4, true );
			break;
		case sf::Keyboard::F5:
			kbd.setKey( KBD_F5, true );
			break;
		case sf::Keyboard::F6:
			kbd.setKey( KBD_F6, true );
			break;
		case sf::Keyboard::F7:
			kbd.setKey( KBD_F7, true );
			break;
		case sf::Keyboard::F8:
			kbd.setKey( KBD_F8, true );
			break;
		case sf::Keyboard::F9:
			kbd.setKey( KBD_F9, true );
			break;
		case sf::Keyboard::F10:
			kbd.setKey( KBD_F10, true );
			break;
		case sf::Keyboard::F11:
			kbd.setKey( KBD_F11, true );
			break;
		case sf::Keyboard::F12:
			kbd.setKey( KBD_F12, true );
			break;
		}
		break;
	case sf::Event::EventType::KeyReleased:
		switch ( event.key.code )
		{
			// alphabet
		case sf::Keyboard::A:
			kbd.setKey( KBD_A, false );
			break;
		case sf::Keyboard::B:
			kbd.setKey( KBD_B, false );
			break;
		case sf::Keyboard::C:
			kbd.setKey( KBD_C, false );
			break;
		case sf::Keyboard::D:
			kbd.setKey( KBD_D, false );
			break;
		case sf::Keyboard::E:
			kbd.setKey( KBD_E, false );
			break;
		case sf::Keyboard::F:
			kbd.setKey( KBD_F, false );
			break;
		case sf::Keyboard::G:
			kbd.setKey( KBD_G, false );
			break;
		case sf::Keyboard::H:
			kbd.setKey( KBD_H, false );
			break;
		case sf::Keyboard::I:
			kbd.setKey( KBD_I, false );
			break;
		case sf::Keyboard::J:
			kbd.setKey( KBD_J, false );
			break;
		case sf::Keyboard::K:
			kbd.setKey( KBD_K, false );
			break;
		case sf::Keyboard::L:
			kbd.setKey( KBD_L, false );
			break;
		case sf::Keyboard::M:
			kbd.setKey( KBD_M, false );
			break;
		case sf::Keyboard::N:
			kbd.setKey( KBD_N, false );
			break;
		case sf::Keyboard::O:
			kbd.setKey( KBD_O, false );
			break;
		case sf::Keyboard::P:
			kbd.setKey( KBD_P, false );
			break;
		case sf::Keyboard::Q:
			kbd.setKey( KBD_Q, false );
			break;
		case sf::Keyboard::R:
			kbd.setKey( KBD_R, false );
			break;
		case sf::Keyboard::S:
			kbd.setKey( KBD_S, false );
			break;
		case sf::Keyboard::T:
			kbd.setKey( KBD_T, false );
			break;
		case sf::Keyboard::U:
			kbd.setKey( KBD_U, false );
			break;
		case sf::Keyboard::V:
			kbd.setKey( KBD_V, false );
			break;
		case sf::Keyboard::W:
			kbd.setKey( KBD_W, false );
			break;
		case sf::Keyboard::X:
			kbd.setKey( KBD_X, false );
			break;
		case sf::Keyboard::Y:
			kbd.setKey( KBD_Y, false );
			break;
		case sf::Keyboard::Z:
			kbd.setKey( KBD_Z, false );
			break;
			// numbers
		case sf::Keyboard::Num0:
			kbd.setKey( KBD_0, false );
			break;
		case sf::Keyboard::Num1:
			kbd.setKey( KBD_1, false );
			break;
		case sf::Keyboard::Num2:
			kbd.setKey( KBD_2, false );
			break;
		case sf::Keyboard::Num3:
			kbd.setKey( KBD_3, false );
			break;
		case sf::Keyboard::Num4:
			kbd.setKey( KBD_4, false );
			break;
		case sf::Keyboard::Num5:
			kbd.setKey( KBD_5, false );
			break;
		case sf::Keyboard::Num6:
			kbd.setKey( KBD_6, false );
			break;
		case sf::Keyboard::Num7:
			kbd.setKey( KBD_7, false );
			break;
		case sf::Keyboard::Num8:
			kbd.setKey( KBD_8, false );
			break;
		case sf::Keyboard::Num9:
			kbd.setKey( KBD_9, false );
			break;
			// special characters
		case sf::Keyboard::Tilde:
			kbd.setKey( KBD_TILDE, false );
			break;
		case sf::Keyboard::Dash:
			kbd.setKey( KBD_DASH, false );
			break;
		case sf::Keyboard::Equal:
			kbd.setKey( KBD_EQUALS, false );
			break;
		case sf::Keyboard::LBracket:
			kbd.setKey( KBD_OPEN_BRACKET, false );
			break;
		case sf::Keyboard::RBracket:
			kbd.setKey( KBD_CLOSED_BRACKET, false );
			break;
		case sf::Keyboard::BackSlash:
			kbd.setKey( KBD_BACKSLASH, false );
			break;
		case sf::Keyboard::SemiColon:
			kbd.setKey( KBD_SEMICOLON, false );
			break;
		case sf::Keyboard::Quote:
			kbd.setKey( KBD_APOSTROPHE, false );
			break;
		case sf::Keyboard::Comma:
			kbd.setKey( KBD_COMMA, false );
			break;
		case sf::Keyboard::Period:
			kbd.setKey( KBD_PERIOD, false );
			break;
		case sf::Keyboard::Slash:
			kbd.setKey( KBD_FORWARD_SLASH, false );
			break;
			// modifiers
		case sf::Keyboard::Return:
			kbd.setKey( KBD_ENTER, false );
			break;
			/*case VK_SHIFT:
			kbd.setKey( KBD_SHIFT, false );
			break;*/
		case sf::Keyboard::LShift:
			kbd.setKey( KBD_SHIFT_LEFT, false );
			kbd.setKey( KBD_SHIFT, false );
			break;
		case sf::Keyboard::RShift:
			kbd.setKey( KBD_SHIFT_RIGHT, false );
			kbd.setKey( KBD_SHIFT, false );
			break;
			/*case VK_CONTROL:
			kbd.setKey( KBD_CONTROL, false );
			break;*/
		case sf::Keyboard::LControl:
			kbd.setKey( KBD_CONTROL_LEFT, false );
			kbd.setKey( KBD_CONTROL, false );
			break;
		case sf::Keyboard::RControl:
			kbd.setKey( KBD_CONTROL_RIGHT, false );
			kbd.setKey( KBD_CONTROL, false );
			break;
		case sf::Keyboard::LAlt:
		case sf::Keyboard::RAlt:
			kbd.setKey( KBD_ALT, false );
			break;
			/*case VK_CAPITAL:							SFML DOESN'T SUPPORT CAPSLOCK WHAT FUCKING IDIOTS THOUGHT THAT WAS A GOOD IDEA?!
			kbd.setKey( KBD_CAPSLOCK, false );
			break;*/
		case sf::Keyboard::Tab:
			kbd.setKey( KBD_TAB, false );
			break;
			// others
		case sf::Keyboard::Escape:
			kbd.setKey( KBD_ESCAPE, false );
			break;
		case sf::Keyboard::Space:
			kbd.setKey( KBD_SPACEBAR, false );
			break;
		case sf::Keyboard::BackSpace:
			kbd.setKey( KBD_BACKSPACE, false );
			break;
		case sf::Keyboard::Insert:
			kbd.setKey( KBD_INSERT, false );
			break;
		case sf::Keyboard::Delete:
			kbd.setKey( KBD_DELETE, false );
			break;
		case sf::Keyboard::Home:
			kbd.setKey( KBD_HOME, false );
			break;
		case sf::Keyboard::End:
			kbd.setKey( KBD_END, false );
			break;
		case sf::Keyboard::PageUp:
			kbd.setKey( KBD_PAGEUP, false );
			break;
		case sf::Keyboard::PageDown:
			kbd.setKey( KBD_PAGEDOWN, false );
			break;
			// arrow keys
		case sf::Keyboard::Up:
			kbd.setKey( KBD_ARROWUP, false );
			break;
		case sf::Keyboard::Down:
			kbd.setKey( KBD_ARROWDOWN, false );
			break;
		case sf::Keyboard::Left:
			kbd.setKey( KBD_ARROWLEFT, false );
			break;
		case sf::Keyboard::Right:
			kbd.setKey( KBD_ARROWRIGHT, false );
			break;
			// numpad
		case sf::Keyboard::Numpad0:
			kbd.setKey( KBD_NUMPAD_0, false );
			break;
		case sf::Keyboard::Numpad1:
			kbd.setKey( KBD_NUMPAD_1, false );
			break;
		case sf::Keyboard::Numpad2:
			kbd.setKey( KBD_NUMPAD_2, false );
			break;
		case sf::Keyboard::Numpad3:
			kbd.setKey( KBD_NUMPAD_3, false );
			break;
		case sf::Keyboard::Numpad4:
			kbd.setKey( KBD_NUMPAD_4, false );
			break;
		case sf::Keyboard::Numpad5:
			kbd.setKey( KBD_NUMPAD_5, false );
			break;
		case sf::Keyboard::Numpad6:
			kbd.setKey( KBD_NUMPAD_6, false );
			break;
		case sf::Keyboard::Numpad7:
			kbd.setKey( KBD_NUMPAD_7, false );
			break;
		case sf::Keyboard::Numpad8:
			kbd.setKey( KBD_NUMPAD_8, false );
			break;
		case sf::Keyboard::Numpad9:
			kbd.setKey( KBD_NUMPAD_9, false );
			break;
			/*case VK_DECIMAL:
			kbd.setKey( KBD_NUMPAD_DECIMAL, false );
			break;*/
		case sf::Keyboard::Add:
			kbd.setKey( KBD_NUMPAD_PLUS, false );
			break;
		case sf::Keyboard::Subtract:
			kbd.setKey( KBD_NUMPAD_MINUS, false );
			break;
		case sf::Keyboard::Multiply:
			kbd.setKey( KBD_NUMPAD_MULTIPLY, false );
			break;
		case sf::Keyboard::Divide:
			kbd.setKey( KBD_NUMPAD_DIVIDE, false );
			break;
			/*case VK_SEPARATOR:
			kbd.setKey( KBD_NUMPAD_DASH, false );
			break;*/
			// function keys
		case sf::Keyboard::F1:
			kbd.setKey( KBD_F1, false );
			break;
		case sf::Keyboard::F2:
			kbd.setKey( KBD_F2, false );
			break;
		case sf::Keyboard::F3:
			kbd.setKey( KBD_F3, false );
			break;
		case sf::Keyboard::F4:
			kbd.setKey( KBD_F4, false );
			break;
		case sf::Keyboard::F5:
			kbd.setKey( KBD_F5, false );
			break;
		case sf::Keyboard::F6:
			kbd.setKey( KBD_F6, false );
			break;
		case sf::Keyboard::F7:
			kbd.setKey( KBD_F7, false );
			break;
		case sf::Keyboard::F8:
			kbd.setKey( KBD_F8, false );
			break;
		case sf::Keyboard::F9:
			kbd.setKey( KBD_F9, false );
			break;
		case sf::Keyboard::F10:
			kbd.setKey( KBD_F10, false );
			break;
		case sf::Keyboard::F11:
			kbd.setKey( KBD_F11, false );
			break;
		case sf::Keyboard::F12:
			kbd.setKey( KBD_F12, false );
			break;
		}
		break;
	case sf::Event::EventType::MouseButtonPressed:
		switch ( event.key.code )
		{
		case sf::Mouse::Button::Left:
			mouse.setKey( (KEYTYPE)LEFT_CLICK, true );
			break;
		case sf::Mouse::Button::Right:
			mouse.setKey( (KEYTYPE)RIGHT_CLICK, true );
			break;
		}
		break;
	case sf::Event::EventType::MouseButtonReleased:
		switch ( event.key.code )
		{
		case sf::Mouse::Button::Left:
			mouse.setKey( (KEYTYPE)LEFT_CLICK, false );
			break;
		case sf::Mouse::Button::Right:
			mouse.setKey( (KEYTYPE)RIGHT_CLICK, false );
			break;
		}
		break;
	case sf::Event::MouseWheelScrolled:
		if ( event.mouseWheelScroll.delta > 0 )
		{
			mouse.setMouseWheelVert( true );
		}
		else if ( event.mouseWheelScroll.delta < 0 )
		{
			mouse.setMouseWheelVert( false );
		}
		// don't really care about horizontal wheel
		break;
	}
}