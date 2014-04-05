#ifndef JBK_FPS_COUNTER_H
#define JBK_FPS_COUNTER_H

#include <stdint.h>

typedef struct JBKFPSCounter {
	double m_duration;
	uint32_t m_frameCounter;
	double m_timeToAverage;
	float m_currentFPS;
} JBKFPSCounter;

void JBKFPSCounter_Init(JBKFPSCounter* inst, double timeToAverage);
void JBKFPSCounter_Destroy(JBKFPSCounter* inst);

void JBKFPSCounter_Update(JBKFPSCounter* inst, double deltaTime);
float JBKFPSCounter_GetFPS(JBKFPSCounter* inst);

#endif // JBK_FPS_COUNTER_H
