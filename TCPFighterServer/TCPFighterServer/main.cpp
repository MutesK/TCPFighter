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

		// ������ ó��
		Update();

		// ����͸� üũ
		Monitoring();

		// ���� ���� üũ
		//ServerControl();

		Sleep(0);
	}

	NetworkClear();
	timeEndPeriod(1);


	return 0;
}