#pragma once

#include <iostream>

namespace rvn {
	typedef enum class KeyCode : uint16_t
	{
		/* Printable keys */
		Space = 32,
		Apostrophe = 39,  /* ' */
		Comma = 44,  /* , */
		Minus = 45,  /* - */
		Period = 46,  /* . */
		Slash = 47,  /* / */
		Num0 = 48,
		Num1 = 49,
		Num2 = 50,
		Num3 = 51,
		Num4 = 52,
		Num5 = 53,
		Num6 = 54,
		Num7 = 55,
		Num8 = 56,
		Num9 = 57,
		Semicolon = 59,  /* ; */
		Equal = 61,  /* = */
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */
		World_1 = 161, /* non-US #1 */
		World_2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		Page_Up = 266,
		Page_Down = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348,
	} Key;
	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}
#define KEY_SPACE              ::rvn::KeyCode::Space           
#define KEY_APOSTROPHE         ::rvn::KeyCode::Apostrophe      
#define KEY_COMMA              ::rvn::KeyCode::Comma           
#define KEY_MINUS              ::rvn::KeyCode::Minus           
#define KEY_PERIOD             ::rvn::KeyCode::Period          
#define KEY_SLASH              ::rvn::KeyCode::Slash           
#define KEY_0                  ::rvn::KeyCode::Num0
#define KEY_1                  ::rvn::KeyCode::Num1
#define KEY_2                  ::rvn::KeyCode::Num2
#define KEY_3                  ::rvn::KeyCode::Num3
#define KEY_4                  ::rvn::KeyCode::Num4
#define KEY_5                  ::rvn::KeyCode::Num5
#define KEY_6                  ::rvn::KeyCode::Num6
#define KEY_7                  ::rvn::KeyCode::Num7
#define KEY_8                  ::rvn::KeyCode::Num8
#define KEY_9                  ::rvn::KeyCode::Num9
#define KEY_SEMICOLON          ::rvn::KeyCode::Semicolon       
#define KEY_EQUAL              ::rvn::KeyCode::Equal           
#define KEY_A                  ::rvn::KeyCode::A
#define KEY_B                  ::rvn::KeyCode::B
#define KEY_C                  ::rvn::KeyCode::C
#define KEY_D                  ::rvn::KeyCode::D
#define KEY_E                  ::rvn::KeyCode::E
#define KEY_F                  ::rvn::KeyCode::F
#define KEY_G                  ::rvn::KeyCode::G
#define KEY_H                  ::rvn::KeyCode::H
#define KEY_I                  ::rvn::KeyCode::I
#define KEY_J                  ::rvn::KeyCode::J
#define KEY_K                  ::rvn::KeyCode::K
#define KEY_L                  ::rvn::KeyCode::L
#define KEY_M                  ::rvn::KeyCode::M
#define KEY_N                  ::rvn::KeyCode::N
#define KEY_O                  ::rvn::KeyCode::O
#define KEY_P                  ::rvn::KeyCode::P
#define KEY_Q                  ::rvn::KeyCode::Q
#define KEY_R                  ::rvn::KeyCode::R
#define KEY_S                  ::rvn::KeyCode::S
#define KEY_T                  ::rvn::KeyCode::T
#define KEY_U                  ::rvn::KeyCode::U
#define KEY_V                  ::rvn::KeyCode::V
#define KEY_W                  ::rvn::KeyCode::W
#define KEY_X                  ::rvn::KeyCode::X
#define KEY_Y                  ::rvn::KeyCode::Y
#define KEY_Z                  ::rvn::KeyCode::Z
#define KEY_LEFT_BRACKET       ::rvn::KeyCode::LeftBracket
#define KEY_BACKSLASH          ::rvn::KeyCode::Backslash
#define KEY_RIGHT_BRACKET      ::rvn::KeyCode::RightBracket
#define KEY_GRAVE_ACCENT       ::rvn::KeyCode::GraveAccent
#define KEY_WORLD_1            ::rvn::KeyCode::World_1
#define KEY_WORLD_2            ::rvn::KeyCode::World_2

#define KEY_ESCAPE             ::rvn::KeyCode::Escape
#define KEY_ENTER              ::rvn::KeyCode::Enter
#define KEY_TAB                ::rvn::KeyCode::Tab
#define KEY_BACKSPACE          ::rvn::KeyCode::Backspace
#define KEY_INSERT             ::rvn::KeyCode::Insert
#define KEY_DELETE             ::rvn::KeyCode::Delete
#define KEY_RIGHT              ::rvn::KeyCode::Right
#define KEY_LEFT               ::rvn::KeyCode::Left
#define KEY_DOWN               ::rvn::KeyCode::Down            
#define KEY_UP                 ::rvn::KeyCode::Up              
#define KEY_PAGE_UP            ::rvn::KeyCode::Page_Up         
#define KEY_PAGE_DOWN          ::rvn::KeyCode::Page_Down       
#define KEY_HOME               ::rvn::KeyCode::Home            
#define KEY_END                ::rvn::KeyCode::End             
#define KEY_CAPS_LOCK          ::rvn::KeyCode::CapsLock        
#define KEY_SCROLL_LOCK        ::rvn::KeyCode::ScrollLock      
#define KEY_NUM_LOCK           ::rvn::KeyCode::NumLock         
#define KEY_PRINT_SCREEN       ::rvn::KeyCode::PrintScreen     
#define KEY_PAUSE              ::rvn::KeyCode::Pause           
#define KEY_F1                 ::rvn::KeyCode::F1              
#define KEY_F2                 ::rvn::KeyCode::F2              
#define KEY_F3                 ::rvn::KeyCode::F3              
#define KEY_F4                 ::rvn::KeyCode::F4              
#define KEY_F5                 ::rvn::KeyCode::F5              
#define KEY_F6                 ::rvn::KeyCode::F6              
#define KEY_F7                 ::rvn::KeyCode::F7              
#define KEY_F8                 ::rvn::KeyCode::F8              
#define KEY_F9                 ::rvn::KeyCode::F9              
#define KEY_F10                ::rvn::KeyCode::F10             
#define KEY_F11                ::rvn::KeyCode::F11             
#define KEY_F12                ::rvn::KeyCode::F12             
#define KEY_F13                ::rvn::KeyCode::F13             
#define KEY_F14                ::rvn::KeyCode::F14             
#define KEY_F15                ::rvn::KeyCode::F15             
#define KEY_F16                ::rvn::KeyCode::F16             
#define KEY_F17                ::rvn::KeyCode::F17             
#define KEY_F18                ::rvn::KeyCode::F18             
#define KEY_F19                ::rvn::KeyCode::F19             
#define KEY_F20                ::rvn::KeyCode::F20             
#define KEY_F21                ::rvn::KeyCode::F21             
#define KEY_F22                ::rvn::KeyCode::F22             
#define KEY_F23                ::rvn::KeyCode::F23             
#define KEY_F24                ::rvn::KeyCode::F24             
#define KEY_F25                ::rvn::KeyCode::F25             
#define KEY_KP_0               ::rvn::KeyCode::KP0             
#define KEY_KP_1               ::rvn::KeyCode::KP1             
#define KEY_KP_2               ::rvn::KeyCode::KP2             
#define KEY_KP_3               ::rvn::KeyCode::KP3             
#define KEY_KP_4               ::rvn::KeyCode::KP4             
#define KEY_KP_5               ::rvn::KeyCode::KP5             
#define KEY_KP_6               ::rvn::KeyCode::KP6             
#define KEY_KP_7               ::rvn::KeyCode::KP7             
#define KEY_KP_8               ::rvn::KeyCode::KP8             
#define KEY_KP_9               ::rvn::KeyCode::KP9             
#define KEY_KP_DECIMAL         ::rvn::KeyCode::KPDecimal       
#define KEY_KP_DIVIDE          ::rvn::KeyCode::KPDivide        
#define KEY_KP_MULTIPLY        ::rvn::KeyCode::KPMultiply      
#define KEY_KP_SUBTRACT        ::rvn::KeyCode::KPSubtract      
#define KEY_KP_ADD             ::rvn::KeyCode::KPAdd           
#define KEY_KP_ENTER           ::rvn::KeyCode::KPEnter         
#define KEY_KP_EQUAL           ::rvn::KeyCode::KPEqual         
#define KEY_LEFT_SHIFT         ::rvn::KeyCode::LeftShift       
#define KEY_LEFT_CONTROL       ::rvn::KeyCode::LeftControl     
#define KEY_LEFT_ALT           ::rvn::KeyCode::LeftAlt         
#define KEY_LEFT_SUPER         ::rvn::KeyCode::LeftSuper       
#define KEY_RIGHT_SHIFT        ::rvn::KeyCode::RightShift      
#define KEY_RIGHT_CONTROL      ::rvn::KeyCode::RightControl    
#define KEY_RIGHT_ALT          ::rvn::KeyCode::RightAlt        
#define KEY_RIGHT_SUPER        ::rvn::KeyCode::RightSuper      
#define KEY_MENU               ::rvn::KeyCode::Menu      