#include "Common\common.h"
#include "Network\NetworkProc.h"
#include "Content\ContentProc.h"
#include "Content\Sector.h"
bool isQuit = false;
int main()
{

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

		// 정상 종료 체크
		//ServerControl();

		Sleep(0);
	}

	NetworkClear();
	timeEndPeriod(1);


	return 0;
}