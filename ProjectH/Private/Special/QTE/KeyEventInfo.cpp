#include "Special/QTE/KeyEventInfo.h"



void FKeyEventInfo::SetValues(EQTEState St, FKey K, FQTEWidgetPosition SP, float TT, float ET)
{
	State = St;
	Key = K;
	ScreenPosition = SP;
	TotalTime = TT;
	ExpiredTime = ET;
}