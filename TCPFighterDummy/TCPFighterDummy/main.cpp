#include "Common\common.h"
#include "Network\NetworkProc.h"
#include "Content\ContentProc.h"
bool isQuit = false;
int main()
{
	// 연결량 조절
	if (!NetworkInit())
		return false;
	
	timeBeginPeriod(1);

	while (!isQuit)
	{

		NetworkProcess();

		// 컨텐츠 처리
		Update();

		// 모니터링 체크
		Monitoring();


		Sleep(0);
	}

	NetworkClear();
	timeEndPeriod(1);


	return 0;
}