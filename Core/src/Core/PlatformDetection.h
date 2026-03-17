
#ifdef _WIN32

	#ifdef  _WIN64
	
		#define NGN_PLATFORM_WINDOWS
	
	#else // ! _WIN64

		#error "x86 Builds not supported"

	#endif 
	
#endif
