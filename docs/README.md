# Docs

## Helpful Links
- [Mozilla - Writing WebSocket Servers](https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_servers)
- [Wikipedia - WebSockets](en.wikipedia.org/wiki/WebSocket)

## Capture local traffic

### Enable
```
# Find the network configuration that has the default gateway.
$defaultAdapter = Get-WMIObject Win32_NetworkAdapterConfiguration | ? {$_.DefaultIPGateway}
if (@($defaultAdapter).Length -ne 1) {throw "You don't have 1 default gateway, your network configuration is not supported" } 
# Route local IP address via the default gateway
route add $defaultAdapter.IPAddress[0] $defaultAdapter.DefaultIPGateway
Write-Host "Start capturing on localhost by connecting to $($defaultAdapter.IPAddress[0])"
```

### Disable
```
# Find the network configuration that has the default gateway.
$defaultAdapter = Get-WMIObject Win32_NetworkAdapterConfiguration | ? {$_.DefaultIPGateway}
if (@($defaultAdapter).Length -ne 1) {throw "You don't have 1 default gateway, your network configuration is not supported" } 
 
# Stop routing localhost traffic to the router.
route delete $defaultAdapter.IPAddress[0]
```