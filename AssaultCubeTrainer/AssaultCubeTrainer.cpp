/* 
*	Assault Cube v1.2 Trainer
*	Clay Caudell
*	4/16/2019
*	
*	Functionality:
*
*	Ammo Hack - Enabled by 'F1' 
*		- Grants the user Unlimited Ammo
*		- Known Bug: Fails memory write until first round is fired.
*
*	Health Hack - Enabled by 'F2'
*		- Grants the user Unlimited Health
*
*	More to come...
*
*/

#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>

#define _WIN32_WINNT _WIN32_WINNT_WINXP	//Allows Full Access to new Process
//#define DEBUG TRUE	//Enable Debugging output if TRUE

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);	
void WriteToMem(HANDLE hProcHandle);
BOOL PrivilegeSet();

std::string GameName = "AssaultCube";		//Sets the GameName, and Game Window Name for identifying ProcessID
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;						

bool IsGameAvail;
bool UpdateONR;

//Ammo variables
bool AmmoStatus;
BYTE AmmoValue[] = { 0xa3, 0x1c, 0x0, 0x0 };	//Value written to the memory address of Ammo
DWORD AmmoBaseAddress = { 0x00509B74 };			//Static Address found to point to Ammo
DWORD AmmoOffsets[] = { 0x384, 0x14, 0x0 };		//Offsets found to Navigate by Pointer Arithmetic to the Address containing Ammo

//Health variables
bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5, 0x0, 0x0 };	//Value written to the memory addres of Health
DWORD HealthBaseAddress = { 0x00509B74 };		//Static Address found to point to Health
DWORD HealthOffsets[] = { 0xf8 };				//Offsets found to Navigate by Pointer Arithmetic to the Address containing Ammo


int main()
{

	HWND hGameWindow = NULL;
	
	int timeSinceLU = clock();			
	int GameAvailTMR = clock();
	int onePressTMR = clock();

	DWORD dwProcID = NULL;
	HANDLE hProcHandle = NULL;

	UpdateONR = true;

	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";

	while (!GetAsyncKeyState(VK_INSERT))		
	{
		if (clock() - GameAvailTMR > 100)		//Checks if the time since last Update has been at least 100ms
		{
			GameAvailTMR = clock();
			IsGameAvail = false;

			hGameWindow = FindWindowA(NULL, LGameWindow);	//Gets Game Window expression
			
			if (hGameWindow)
			{
				GetWindowThreadProcessId(hGameWindow, &dwProcID);
					if (dwProcID)
					{
						hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);	//Attempts to open a Handle for the Process with All Access Privileges

						if (hProcHandle == NULL)
						{
							GameStatus = "Failed to open process for valid handle";
						}

						else
						{
							GameStatus = "AssaultCube Ready to Hack";
							IsGameAvail = TRUE;
						}
					}
					else
					{
						GameStatus = "Failed to get process ID";
					}
			}
			else
			{
				GameStatus = "AssaultCube Window NOT FOUND";
			}

			if (UpdateONR == TRUE || clock() - timeSinceLU > 5000)
			{
				system("cls");
				std::cout << "---------------------------------------------" << std::endl;
				std::cout << "          AssaultCube Memory Hackz" << std::endl;
				std::cout << "---------------------------------------------" << std::endl;
				std::cout << "Game STATUS:" << GameStatus << std::endl << std::endl;
				std::cout << "[F1] Unlimited Ammo -> " << sAmmoStatus << " <-" << std::endl << std::endl;
				std::cout << "[F2] Unlimited Health -> " << sHealthStatus << " <-" << std::endl << std::endl;
				std::cout << "[INSERT] Exit" << std::endl;
				UpdateONR = FALSE;
				timeSinceLU = clock();
			}
		
			if (IsGameAvail)
			{
				WriteToMem(hProcHandle);
			}
		}
		if (clock() - onePressTMR > 400)
		{
			if (IsGameAvail)
			{
				if (GetAsyncKeyState(VK_F1))
				{
					onePressTMR = clock();
					AmmoStatus = !AmmoStatus;
					UpdateONR = TRUE;
					if (AmmoStatus) sAmmoStatus = "ON";
					else sAmmoStatus = "OFF";
				}

				else if (GetAsyncKeyState(VK_F2))
				{
					onePressTMR = clock();
					HealthStatus = !HealthStatus;
					UpdateONR = TRUE;
					if (HealthStatus) sHealthStatus = "ON";
					else sHealthStatus = "OFF";
				}
			}
		}
	}
	CloseHandle(hProcHandle);

	return ERROR_SUCCESS;
}

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
	DWORD pTemp = 69;

	DWORD pointerAddr;
	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == 0)
		{
			if (!(ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, 4, NULL))) std::cout << "FAILED" << std::endl;
		}

		pointerAddr = pTemp + Offsets[i];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, 4, NULL);

#ifdef DEBUG
		std::cout << std::endl << pTemp << "---" << Offsets[i] << std::endl;
#endif

	}
	return pointerAddr;
}
void WriteToMem(HANDLE hProcHandle)
{
	bool Success;
	DWORD AddressToWrite;

	if (AmmoStatus)
	{
		AddressToWrite = FindDmaAddy(3, hProcHandle, AmmoOffsets, AmmoBaseAddress);
		Success = WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &AmmoValue, sizeof(AmmoValue), NULL);

#ifdef DEBUG
		if(Success) std::cout << "Ammo Memory Write Successful" << std::endl;
		else std::cout << "Ammo Memory Write Failed -- " << GetLastError() << std::endl;
#endif
	}

	if (HealthStatus)
	{
		AddressToWrite = FindDmaAddy(1, hProcHandle, HealthOffsets, HealthBaseAddress);
		Success = WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &HealthValue, sizeof(HealthValue), NULL);

#ifdef DEBUG
		if(Success) std::cout << "Health Memory Write Successful" << std::endl;
		else std::cout << "Health Memory Write Failed-- " << GetLastError() << std::endl;
#endif
	}
}
