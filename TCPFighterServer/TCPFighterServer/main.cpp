#include "Common\common.h"
#include "Network\NetworkProc.h"
#include "Content\ContentProc.h"
#include "Content\Sector.h"

/*
	Move, Stop, 데드레커닝 파트
	디버깅(뉴 섹터, 딜리트 섹터 전송에 문제)
	-> 어째서 특정한 상황에서 케릭터가 번지는가? (삭제, 갱신이 이상하게 동작)
	공격은 잘됨.
*/

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