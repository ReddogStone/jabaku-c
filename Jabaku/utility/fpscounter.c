#include "fpscounter.h"

void JBKFPSCounter_Init(JBKFPSCounter* inst, double timeToAverage) {
	inst->m_currentFPS = 0.0f;
	inst->m_duration = 0.0f;
	inst->m_frameCounter = 0;
	inst->m_timeToAverage = timeToAverage;
}

void JBKFPSCounter_Destroy(JBKFPSCounter* inst) {}

void JBKFPSCounter_Update(JBKFPSCounter* inst, double deltaTime) {
	inst->m_frameCounter++;
    inst->m_duration += deltaTime;

	if (inst->m_duration >= inst->m_timeToAverage) {
        inst->m_currentFPS = (float)((double)inst->m_frameCounter / inst->m_duration);

        inst->m_frameCounter = 0;
        inst->m_duration = 0.0;
    }
}

float JBKFPSCounter_GetFPS(JBKFPSCounter* inst) {
	return inst->m_currentFPS;
}
