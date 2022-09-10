#include "Process.hpp"
#include "Offsets.hpp"

int main()
{
	GetProcess(L"hl2.exe");
	const auto dwClient = GetModule(L"client.dll");
	const auto dwLocal = Read<DWORD>(dwClient + dwLocalPlayer);

	while (1)
	{
		Sleep(40);

		const auto CrossID = Read<int>(dwLocal + m_iCrosshairId);

		if (!GetAsyncKeyState(VK_XBUTTON1) || !CrossID || CrossID > 64)
			continue;

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		Sleep(100);
	}
}