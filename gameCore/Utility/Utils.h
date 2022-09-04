#ifndef UTIL_H
#define	UTIL_H

//#if defined(_WIN32)           
//#define NOGDI				// All GDI defines and routines
//#define NOUSER				// All USER defines and routines
//#endif
////#include <Windows.h>		// or any library that uses Windows.h


#ifdef _DEBUG
#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>


//#define DEBUG_ERR(x) (std::cerr << (x);)
//#define DEBUG_OUT(x) (std::cout << (x);)
#define CHECK( x ){ assert(x);}
#define DEBUG_FILE( NAME, x ) { std::ofstream os( NAME, std::ios::out | std::ios::app  ); os << x;  }
#define DEBUG_OUT( x ) {  std::ostringstream os; os << x; ::OutputDebugString(os.str().c_str()); }
#define DEBUG_COUT( x ) {  std::ostringstream os; os << x; std::cout << os.str(); }

#define DAY  __DATE__ << ", " << __TIME__ << " - "  
#define PLACE   __FILE__ << "(" << __LINE__ << ") "  
#define FIELD( x ) #x << " = [" << x << "]\n"

#define SET_COMP_NUM( NAME ) { int add;  std::ifstream inFile( NAME); inFile >> add; std::ofstream outFile( NAME, std::ios::trunc); outFile << ++add; }
inline int GetCompNum(std::string name) { int x; std::ifstream inFile(name); inFile >> x; return x; }
#define GET_COMP_NUM(NAME) GetCompNum(NAME)

#define MSG_BOX( x ) {MessageBox(NULL, x, "ERROR", MB_OK | MB_ICONSTOP);}

//... etc
#else 
//#define DEBUG_ERR(x)
//#define DEBUG_OUT(x)
#define DEBUG_FILE( NAME, x)
#define DEBUG_OUT( x )
#define DEBUG_COUT( x )
#define CHECK( x )

#define DAY 
#define PLACE 
#define FIELD 

#define SET_COMP_NUM( NAME )
#define GET_COMP_NUM( NAME )

#define MSG_BOX( x )
//... etc
#endif






#endif // UTIL_H