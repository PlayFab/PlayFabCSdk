# Send-Action.ps1 — Send an action to AdminHelper via named pipe
param(
    [Parameter(Mandatory)][string]$Action,
    [hashtable]$Parameters = @{},
    [int]$TimeoutMs = 10000
)

$pipeName = "PlayFabTestAdminHelper"

try {
    $pipe = New-Object System.IO.Pipes.NamedPipeClientStream(".", $pipeName, [System.IO.Pipes.PipeDirection]::InOut)
    $pipe.Connect($TimeoutMs)

    $request = @{ action = $Action }
    if ($Parameters.Count -gt 0) { $request.parameters = $Parameters }
    $json = ($request | ConvertTo-Json -Compress) + "`n"
    $bytes = [System.Text.Encoding]::UTF8.GetBytes($json)
    $pipe.Write($bytes, 0, $bytes.Length)
    $pipe.Flush()

    $sb = New-Object System.Text.StringBuilder
    while ($true) {
        $b = $pipe.ReadByte()
        if ($b -eq -1 -or $b -eq 10) { break }
        [void]$sb.Append([char]$b)
    }
    $pipe.Close()

    $response = $sb.ToString().TrimEnd() | ConvertFrom-Json
    if ($response.success) {
        Write-Host "OK: $($response.message)" -ForegroundColor Green
    } else {
        Write-Host "FAIL: $($response.message)" -ForegroundColor Red
        exit 1
    }
}
catch [System.TimeoutException] {
    Write-Host "ERROR: AdminHelper not running (connection timed out after ${TimeoutMs}ms)" -ForegroundColor Red
    exit 1
}
catch {
    Write-Host "ERROR: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}
