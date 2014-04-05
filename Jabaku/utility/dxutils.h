#ifndef JBK_DX_UTILS_H
#define JBK_DX_UTILS_H

#include <Unknwn.h>

#include "utility/assertion.h"

#define DXCall(exp) do{ ASSERT((exp) == S_OK, "D3D Error while executing \"" \
	#exp \
	"\"!\n") } while(0);

__inline void SafeRelease(void** p) {
	if (*p) {
		((IUnknown*)(*p))->lpVtbl->Release(*p);
		*p = NULL;
	}
}

#endif JBK_DX_UTILS_H