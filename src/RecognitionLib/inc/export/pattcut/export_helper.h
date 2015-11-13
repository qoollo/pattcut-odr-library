#ifndef _PATCUT_EXPORT_HELPER_H_
#define _PATCUT_EXPORT_HELPER_H_

/* Cmake will define MyLibrary_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MyLibrary_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32) 
  #if defined(RecognitionLib_EXPORTS)
    #define  DLL_EXPORT __declspec(dllexport)
  #else
	#define  DLL_EXPORT __declspec(dllimport)
  #endif /* MyLibrary_EXPORTS */


  #if defined(RecognitionLib_EXPORTS)
    #define  DLL_EXPORT_TEST __declspec(dllexport)
  #else
    #define  DLL_EXPORT_TEST __declspec(dllimport)
  #endif /* MyLibrary_EXPORTS */

#else /* defined (_WIN32) */
	#define DLL_EXPORT
    #define DLL_EXPORT_TEST
#endif



#if defined (_MSC_VER)
	#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#elif defined(__GNUC__)
	#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#else
	#define PACK( __Declaration__ ) __Declaration__ 
#endif


#endif