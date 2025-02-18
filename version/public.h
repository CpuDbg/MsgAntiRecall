
#include <stdlib.h>
#include <io.h>
#include <CRTDBG.H>

#define Debug2
#ifdef Debug2
#define _Log0(x)					_RPT0(_CRT_WARN, x)
#define _Log1(x, y)					_RPT1(_CRT_WARN, x, y)
#define _Log2(x, y, z)				_RPT2(_CRT_WARN, x, y, z)
#define _Log3(x, y, z, w)			_RPT3(_CRT_WARN, x, y, z, w)
#define _Log4(x, y, z, w, b)		_RPT4(_CRT_WARN, x, y, z, w, b)
#else
#define _Log0(x)
#define _Log1(x, y)
#define _Log2(x, y, z)
#define _Log3(x, y, z, w)
#define _Log4(x, y, z, w, b)
#endif