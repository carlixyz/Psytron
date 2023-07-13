#ifndef UTIL_H
#define	UTIL_H

//#if defined(_WIN32)         // If (for some reason) I need to use DEBUG_OUT Enable ALL this !
//#define NOGDI				// All GDI defines and routines
//#define NOUSER				// All USER defines and routines
//#include <Windows.h>		// or any library that uses Windows.h
//#endif

//
//bool IsNumber(const std::string& s)
//{
//	return (!s.empty() && s.find_first_not_of("-.0123456789") == std::string::npos);
//}

#ifdef _DEBUG
#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>

#define DEBUG_COUT( x ) {  std::ostringstream os; os << x; std::cout << os.str(); }
#define DEBUG_OUT( x ) {  std::ostringstream os; os << x; ::OutputDebugStringA(os.str().c_str()); }
#define DEBUG_FILE( NAME, x ) { std::ofstream os( NAME, std::ios::out | std::ios::app  ); os << x;  }
#define CHECK( x ){ assert(x);}

#define DAY  __DATE__ << ", " << __TIME__ << " - "  
#define PLACE   __FILE__ << "(" << __LINE__ << ") "  
#define FIELD( x ) #x << " = [" << x << "]\n"

#define SET_BUILD_NUM( NAME ) { int add;  std::ifstream inFile( NAME); inFile >> add; std::ofstream outFile( NAME, std::ios::trunc); outFile << ++add; }
inline int GetCompNum(std::string name) { int x; std::ifstream inFile(name); inFile >> x; return x; }
#define GET_BUILD_NUM(NAME) GetCompNum(NAME)

#define MSG_BOX( x ) {MessageBoxA(NULL, x, "ERROR", MB_OK | MB_ICONSTOP);}

//... etc
#else 
#include <sstream>


#define DEBUG_COUT( x )
//#define DEBUG_OUT( x ){  std::ostringstream os; os << x; ::OutputDebugStringA(os.str().c_str()); }
#define DEBUG_OUT( x )
#define DEBUG_FILE( NAME, x)
#define CHECK( x )

#define DAY 
#define PLACE 
#define FIELD 

#define SET_BUILD_NUM( NAME )
#define GET_BUILD_NUM( NAME )

#define MSG_BOX( x ) {MessageBoxA(NULL, x, "ERROR", MB_OK | MB_ICONSTOP);}
//... etc
#endif






#endif // UTIL_H