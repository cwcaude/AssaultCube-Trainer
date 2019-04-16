// AssaultCubeTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>


DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
void WriteToMem(HANDLE hProcHandle);

std::string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;

bool IsGameAvail;
bool UpdateONR;

//Ammo variables
bool AmmoStatus;
BYTE AmmoValue[] = { 0xa3, 0x1c, 0x0, 0x0 };
DWORD AmmoBaseAddress = { 0x00509B74 };
DWORD AmmoOffsets[] = { 0x384, 0x14, 0x0 };

//Health variables
bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5, 0x0, 0x0 };
DWORD HealthBaseAddress = { 0x00509B74 };
DWORD HealthOffsets[] = { 0xf8 };


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
		if (clock() - GameAvailTMR > 100)
		{
			GameAvailTMR = clock();
			IsGameAvail = false;

			hGameWindow = FindWindowA(NULL, LGameWindow);
			
			if (hGameWindow)
			{
				dwProcID = GetWindowThreadProcessId(hGameWindow, &dwProcID);
					if (dwProcID)
					{
						hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);

						if (FALSE)
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

			if (UpdateONR = TRUE || clock() - timeSinceLU > 5000)
			{
				system("cls");
				std::cout << "---------------------------------------------" << std::endl;
				std::cout << "          AssaultCube Memory Hackz" << std::endl;
				std::cout << "---------------------------------------------" << std::endl;
				std::cout << "Game STATUS:" << GameStatus << std::endl << std::endl;
				std::cout << "[F1] Unlimited Ammo -> " << sAmmoStatus << " <-" << std::endl << std::endl;
				std::cout << "[F1] Unlimited Health -> " << sHealthStatus << " <-" << std::endl << std::endl;
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
	CloseHandle(hGameWindow);

	return ERROR_SUCCESS;
}

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
	DWORD pTemp;

	DWORD pointerAddr;
	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
		}

		pointerAddr = pTemp + Offsets[i];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);

	}
	return pointerAddr;
}
void WriteToMem(HANDLE hProcHandle)
{

	DWORD AddressToWrite;

	if (AmmoStatus)
	{
		AddressToWrite = FindDmaAddy(3, hProcHandle, AmmoOffsets, AmmoBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &AmmoValue, sizeof(AmmoValue), NULL);
	}

	if (HealthStatus)
	{
		AddressToWrite = FindDmaAddy(1, hProcHandle, HealthOffsets, HealthBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &HealthValue, sizeof(HealthValue), NULL);
	}
}
