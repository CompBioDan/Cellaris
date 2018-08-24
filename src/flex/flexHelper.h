

#ifndef FLEXHELPER_H_
#define FLEXHELPER_H_

#include "include/NvFlex.h"
//#include "include/helpers.h"
//#include "include/scenes.h"
//#include "include/benchmark.h"


class flexHelper
{

public:

	void Init(int scene, bool centerCamera = true);

	void Shutdown();

	void Reset();

	void UpdateEmitters();

	void ErrorCallBack(NvFlexErrorSeverity, const char* msg, const char* file, int line);

	struct Emitter;

	vector<Emitter> g_emitters(1);	// first emitter is the camera 'gun'


};


#endif /* FLEXHELPER_H _ */