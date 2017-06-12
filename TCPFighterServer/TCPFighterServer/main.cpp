#include "Common\common.h"
#include "Network\NetworkProc.h"
#include "Content\ContentProc.h"
#include "Content\Sector.h"

/*
	Move, Stop, ���巹Ŀ�� ��Ʈ
	�����(�� ����, ����Ʈ ���� ���ۿ� ����)
	-> ��°�� Ư���� ��Ȳ���� �ɸ��Ͱ� �����°�? (����, ������ �̻��ϰ� ����)
	������ �ߵ�.
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