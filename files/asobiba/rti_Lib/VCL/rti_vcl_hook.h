/*

	ƒtƒbƒN

*/

#ifndef ___VCL_HOOKH
#define ___VCL_HOOKH

#define BEGIN_MESSAGE_MAP \
	    virtual LRESULT UserDisPacth \
			(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam) \
			{ 
#define MESSAGE_HANDLER(Message,Proc) \
			case Message: \
				Proc(hwnd , message, wParam,lParam); \
				break;
#define END_MESSAGE_MAP(Base) \
		 	default: \
				Base::UserDisPacth(hwnd , message, wParam,lParam); \
				break; \
		};

#endif

