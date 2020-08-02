#include "pch.h"
#include "btconn.h"
#include <stdlib.h>
#include <stdio.h>

// Link to ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#include <Winsock2.h>
#include <Ws2bth.h>

// Link to Bthprops.lib
#pragma comment(lib, "Bthprops.lib")

#include <BluetoothAPIs.h>

#include <initguid.h>

DWORD Connect(long long mac);
DWORD Disconnect(long long mac);
DWORD ChangeConnection(long long mac, DWORD dwServiceFlags);


// {#define HFP_HS_UUID		"0000111e-0000-1000-8000-00805f9b34fb"}
DEFINE_GUID(HFP_HS_UUID, 0x0000111e, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb);

// #define A2DP_SINK_UUID		"0000110b-0000-1000-8000-00805f9b34fb"
DEFINE_GUID(A2DP_SINK_UUID, 0x0000110b, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb);


BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
BLUETOOTH_RADIO_INFO m_bt_info = { sizeof(BLUETOOTH_RADIO_INFO),0, };

BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params = {
  sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
  1,
  0,
  1,
  1,
  1,
  15,
  NULL
};



BLUETOOTH_DEVICE_INFO m_device_info = { sizeof(BLUETOOTH_DEVICE_INFO),0, };
// Note:
// Radio - is the thing plugged in/attached to the local machine.
// Device - is the thing that is connected to via the Bluetooth connection.


DWORD ChangeConnection(long long mac, DWORD dwServiceFlags)
{
	HANDLE hRadio, hDeviceFind;
	BLUETOOTH_DEVICE_INFO_STRUCT deviceInfo;
	BLUETOOTH_DEVICE_SEARCH_PARAMS deviceSearchParams;
	BLUETOOTH_RADIO_INFO radioInfo;
	GUID guidServices[15];
	DWORD numServices, result;
	int i = 0;
	DWORD error = 0;
	BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&btfrp, &hRadio);

	if (hFind != NULL)
	{
		do
		{
			// Do something with the radio handle...
			radioInfo.dwSize = sizeof(radioInfo);

			if (BluetoothGetRadioInfo(hRadio, &radioInfo) == ERROR_SUCCESS)
			{
				printf("Radio found: %S\n", radioInfo.szName);
			}
			else
				error += 10;
			//printf("BluetoothGetRadioInfo() failed with error code %d\n", GetLastError());

			deviceInfo.dwSize = sizeof(deviceInfo);
			memset(&deviceSearchParams, 0, sizeof(deviceSearchParams));

			deviceSearchParams.dwSize = sizeof(deviceSearchParams);
			deviceSearchParams.fReturnRemembered = TRUE;
			deviceSearchParams.hRadio = hRadio;

			hDeviceFind = BluetoothFindFirstDevice(&deviceSearchParams, &deviceInfo);

			if (hDeviceFind != NULL)
			{
				do
				{
					//Skip if mac wrong
					if (deviceInfo.Address.ullLong != mac)
						continue;

					i += 1;
					result = BluetoothSetServiceState(hRadio, &deviceInfo, &A2DP_SINK_UUID, dwServiceFlags);
					if (result != ERROR_SUCCESS)
						//printf("A2DP_SINK_UUID failed with error code %d\n", result);
						error += 100;

					result = BluetoothSetServiceState(hRadio, &deviceInfo, &HFP_HS_UUID, dwServiceFlags);
					if (result != ERROR_SUCCESS)
						//printf("HFP_HS_UUID failed with error code %d\n", result);
						error += 1000;

				} while (BluetoothFindNextDevice(hDeviceFind, &deviceInfo));
				BluetoothFindDeviceClose(hDeviceFind);
			}
			CloseHandle(hRadio);

		} while (BluetoothFindNextRadio(hFind, &hRadio));
		BluetoothFindRadioClose(hFind);
	}
	else
	{
		//printf("Error BluetoothFindFirstRadio() return null\n");
		error += 1;
	}
	if (i == 0)
		//printf("Device not found\n");
		error += 10000;
	return error;
}

DWORD Connect(long long mac)
{	
	return ChangeConnection(mac, BLUETOOTH_SERVICE_ENABLE);	
}

DWORD Disconnect(long long mac)
{
	return ChangeConnection(mac, BLUETOOTH_SERVICE_DISABLE);
}