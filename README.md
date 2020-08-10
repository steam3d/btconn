# btconn
Connect to Bluetooth device by MAC

# Usage c#

```
[DllImport("/dll/btconn.dll", EntryPoint = "Connect", CallingConvention = CallingConvention.Cdecl)]
public static extern uint Connect(ulong mac);

[DllImport("/dll/btconn.dll", EntryPoint = "Disconnect", CallingConvention = CallingConvention.Cdecl)]
public static extern uint Disconnect(ulong mac);

// lastError = 0 - success action
var lastError = Connect(mac); 
```
