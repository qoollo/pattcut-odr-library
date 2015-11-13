#pragma once


// Check windows
#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif


#if defined(ENV32BIT)
	#if defined (_MSC_VER) || defined(__INTEL_COMPILER)
		#define FASTCALL( __Declaration__ ) __fastcall __Declaration__
	#elif defined(__GNUC__)
		#define FASTCALL( __Declaration__ ) __Declaration__ __attribute__((fastcall))
	#else
		#define FASTCALL( __Declaration__ ) __Declaration__ 
	#endif
#else
	#define FASTCALL( __Declaration__ ) __Declaration__
#endif


#if defined (_MSC_VER) || defined(__INTEL_COMPILER)
	#define ALIGN16( __Declaration__ ) __declspec(align(16)) __Declaration__
#elif defined(__GNUC__)
	#define ALIGN16( __Declaration__ ) __Declaration__ __attribute__ ((aligned (16)))
#else
	#define ALIGN16( __Declaration__ ) __Declaration__ 
#endif


