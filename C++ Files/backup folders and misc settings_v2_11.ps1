# SMUD User Migration - Optimized for Large Profiles
# Notes:
# - Replaces Copy-Item with robocopy for multi-threaded, resumable, delta copies
# - Consolidates deletions and reduces Test-Path calls
# - Switches WMI to CIM for faster queries
# - Adds lightweight logging via Start-Transcript
# - Preserves original GUI flow and features (Folders, Settings, Both)

# ===================== Initialization =====================
# Cache environment/user paths up front to reduce repeated lookups
$UserName  = $env:USERNAME
$UserRoot  = [Environment]::GetFolderPath("UserProfile")
$Documents = Join-Path $UserRoot "Documents"
$Desktop   = Join-Path $UserRoot "Desktop"
$Downloads = Join-Path $UserRoot "Downloads"

$NetShare = '\\desktop\apps$\WindowsMigration'
$dest     = Join-Path -Path $NetShare -ChildPath $UserName

# Ensure required .NET assemblies are available once
Add-Type -AssemblyName System.Windows.Forms

# Robocopy default args: fast retries, multi-threaded, quiet logs
$RoboArgsCommon = @("/E", "/MT:32", "/R:1", "/W:1", "/NFL", "/NDL", "/NJH", "/NJS", "/NC", "/NS")

# ===================== Helpers =====================
function Start-Log {
    try {
        if (-not (Test-Path -LiteralPath $dest)) {
            New-Item -ItemType Directory -Path $dest -Force | Out-Null
        }
        $global:LogPath = Join-Path $dest "backup.log"
        Start-Transcript -Path $global:LogPath -ErrorAction SilentlyContinue | Out-Null
    } catch {}
}

function Stop-Log {
    try { Stop-Transcript | Out-Null } catch {}
}

function Invoke-Robocopy {
    param(
        [Parameter(Mandatory)][string]$Source,
        [Parameter(Mandatory)][string]$Destination,
        [string[]]$ExtraArgs = @()
    )
    try {
        if (-not (Test-Path -LiteralPath $Destination)) {
            New-Item -ItemType Directory -Path $Destination -Force | Out-Null
        }
        $args = @("`"$Source`"", "`"$Destination`"") + $RoboArgsCommon + $ExtraArgs
        $proc = Start-Process -FilePath robocopy.exe -ArgumentList $args -Wait -PassThru -WindowStyle Hidden
        return $proc.ExitCode
    } catch {
        Write-Output "Robocopy failed: $Source -> $Destination. $_"
        return 16
    }
}

# ===================== Start of function to set permissions on roaming profile folder if it exists =====================
function Set-RPFolderPermissions {
    param (
        [string]$server = "server01",
        [string]$share  = "profiles$",
        [string]$group  = "corporate\Endpoint Security"
    )
    $folderPath = "\\$server\$share\$UserName.v6"
    try {
        if (Test-Path -LiteralPath $folderPath) {
            icacls.exe $folderPath /grant "${group}:(OI)(CI)F" /C | Out-Null
            if ($LASTEXITCODE -eq 0) {
                Write-Output "Permissions set successfully for $folderPath"
            } else {
                Write-Output "Failed to set permissions for $folderPath (exit $LASTEXITCODE)"
            }
        } else {
            Write-Output "Folder $folderPath does not exist."
        }
    } catch {
        Write-Output "An error occurred setting permissions on $folderPath : $_"
    }
}
# ===================== End of function =====================

# ===================== Start of function to Create the Destination folder =====================
function Create-Dest {
    try {
        if (Test-Path -LiteralPath $dest) {
            $datetime = Get-Date -Format yyyy-MM-dd-HH-mm-ss
            $newName  = "$($dest)_$($datetime)"
            Rename-Item -Path "$dest" -NewName "$newName" -ErrorAction SilentlyContinue
        }
        if (-not (Test-Path -LiteralPath $dest)) {
            New-Item -Path $dest -ItemType Directory -Force | Out-Null
        }
    } catch {
        Write-Output "Create-Dest error: $_"
    }
}
# ===================== End of function =====================

# ===================== Create a function to empty the recycle bin (unused by default) =====================
function Empty-RecycleBin {
    try {
        $Shell = New-Object -ComObject Shell.Application
        $RecycleBin = $Shell.Namespace(0x000a)
        $RecycleBin.Items() | ForEach-Object {
            try { Remove-Item $_.path -Recurse -Force -ErrorAction SilentlyContinue } catch {}
        }
    } catch {}
}
# ===================== End of function =====================

# ===================== Create function to backup users known folders (Documents, Desktop, Downloads) =====================
function Backup-Folders {
    Write-Output "Starting folder backup..."

    # Determine final destination subfolders
    $DestDocuments = Join-Path -Path $dest -ChildPath "Documents"
    $DestDesktop   = Join-Path -Path $dest -ChildPath "Desktop"
    $DestDownloads = Join-Path -Path $dest -ChildPath "Downloads"

    # Copy each with robocopy (multi-threaded, delta copy)
    Invoke-Robocopy -Source $Documents -Destination $DestDocuments | Out-Null
    Invoke-Robocopy -Source $Desktop   -Destination $DestDesktop   | Out-Null
    Invoke-Robocopy -Source $Downloads -Destination $DestDownloads | Out-Null

    # One-pass junk cleanup across destination tree
    try {
        Get-ChildItem -Path $dest -Recurse -Force -ErrorAction SilentlyContinue |
            Where-Object { $https://urldefense.com/v3/__http://_.Name__;!!B5ObAA!FF0JNkBmVVGYKntI5NW5TSTgvhmK_nRVguFHFzHih_a3A44iFt3qvI6r3ITsWxdpzjw8vk10aSWq0tVqxkeX756Z$  -match '(^\$Recycle\.Bin$|Thumbs\.db|desktop\.ini)' } |
            ForEach-Object {
                try { Remove-Item -LiteralPath $_.FullName -Force -Recurse -ErrorAction SilentlyContinue } catch {}
            }
    } catch {}

    Write-Output "Known folders backed up to $dest"
}
# ===================== End of function =====================

# ===================== Create function to backup misc settings =====================
function Backup-Settings {
    Write-Output "Starting settings backup..."

    # ---- Drive mappings (CIM) ----
    try {
        $backupFile = Join-Path $dest "MappingsBackup.txt"
        $driveMappings = Get-CimInstance Win32_NetworkConnection | Where-Object { $_.LocalName }
        if ($driveMappings) {
            $driveMappings | ForEach-Object {
                "$($_.RemoteName)`t$($_.LocalName)" | Out-File -Append -FilePath $backupFile -Encoding UTF8
            }
            Write-Output "Drive mappings with drive letters backed up to $backupFile"
        } else {
            Write-Output "No drive mappings with drive letters found to back up."
        }
    } catch {
        Write-Output "Drive mappings backup error: $_"
    }

    # ---- Local Admin group scan for 'LA*' groups (excluding LA-InfoSecOps) ----
    try {
        $LADest = Join-Path $dest "LA_Groups.txt"
        $localAdminsGroup = [ADSI]"WinNT://./Administrators,group"
        $laGroups = @()
        foreach ($member in $localAdminsGroup.psbase.Invoke("Members")) {
            $memberObject = [ADSI]$member
            if ($memberObject.Class -eq "Group" -and $https://urldefense.com/v3/__http://memberObject.Name__;!!B5ObAA!FF0JNkBmVVGYKntI5NW5TSTgvhmK_nRVguFHFzHih_a3A44iFt3qvI6r3ITsWxdpzjw8vk10aSWq0tVqxodc5smL$  -like "LA*" -and $https://urldefense.com/v3/__http://memberObject.Name__;!!B5ObAA!FF0JNkBmVVGYKntI5NW5TSTgvhmK_nRVguFHFzHih_a3A44iFt3qvI6r3ITsWxdpzjw8vk10aSWq0tVqxodc5smL$  -ne "LA-InfoSecOps") {
                $laGroups += $https://urldefense.com/v3/__http://memberObject.Name__;!!B5ObAA!FF0JNkBmVVGYKntI5NW5TSTgvhmK_nRVguFHFzHih_a3A44iFt3qvI6r3ITsWxdpzjw8vk10aSWq0tVqxodc5smL$ 
            }
        }
        if ($laGroups.Count -gt 0) {
            $laGroups | Out-File -FilePath $LADest -Encoding UTF8
            Write-Output "Group names saved to $LADest"
        } else {
            Write-Output "No extra groups starting with 'LA' found in the local Administrators group."
        }
    } catch {
        Write-Output "Local Admin group scan error: $_"
    }

    # ---- Export SAP registry keys ----
    try {
        $regfile      = Join-Path $dest "SAPCodeBackup.reg"
        $registryPath = "HKEY_CURRENT_USER\Software\SAP\SAPGUI Front\SAP Frontend Server\Code"
        & reg.exe export "$registryPath" "$regfile" /y | Out-Null
        Write-Output "Registry keys for Users SAP Codes backed up to $regfile"
    } catch {
        Write-Output "SAP registry export error: $_"
    }

    # ---- Chrome bookmarks ----
    try {
        $bookmarkFile = Join-Path $UserRoot "AppData\Local\Google\Chrome\User Data\Default\bookmarks"
        if (Test-Path -LiteralPath $bookmarkFile) {
            $bookmarkDest = Join-Path $dest "ChromeBookmarks"
            if (-not (Test-Path -LiteralPath $bookmarkDest)) {
                New-Item -Path $bookmarkDest -ItemType Directory -Force | Out-Null
            }
            Copy-Item -Path $bookmarkFile -Destination $bookmarkDest -Force -ErrorAction SilentlyContinue
        }
    } catch {
        Write-Output "Chrome bookmark backup error: $_"
    }

    # ---- Google Earth KML files ----
    try {
        $kmlFolder = Join-Path $UserRoot "AppData\LocalLow\Google\GoogleEarth"
        if (Test-Path -LiteralPath $kmlFolder) {
            $kmlDest = Join-Path $dest "GoogleEarthkmlFiles"
            Invoke-Robocopy -Source $kmlFolder -Destination $kmlDest -ExtraArgs @("/XF","*.log") | Out-Null
        }
    } catch {
        Write-Output "Google Earth backup error: $_"
    }

    # ---- Snagit DataStore ----
    try {
        $snagitFolder = Join-Path $UserRoot "AppData\Local\TechSmith\Snagit\DataStore"
        if (Test-Path -LiteralPath $snagitFolder) {
            $snagitDest = Join-Path $dest "SnagIt\DataStore"
            Invoke-Robocopy -Source $snagitFolder -Destination $snagitDest | Out-Null
        }
    } catch {
        Write-Output "Snagit backup error: $_"
    }

    # ---- Microsoft Sticky Notes ----
    try {
        $snFolder = Join-Path $UserRoot "AppData\Local\Packages\Microsoft.MicrosoftStickyNotes_8wekyb3d8bbwe\LocalState"
        if (Test-Path -LiteralPath $snFolder) {
            $snDest = Join-Path $dest "Microsoft.MicrosoftStickyNotes_8wekyb3d8bbwe\LocalState"
            Invoke-Robocopy -Source $snFolder -Destination $snDest | Out-Null
        }
    } catch {
        Write-Output "Sticky Notes backup error: $_"
    }

    # ---- Quick Access automatic destinations ----
    try {
        $qaPath = Join-Path $UserRoot "AppData\Roaming\Microsoft\Windows\Recent\AutomaticDestinations\f01b4d95cf55d32a.automaticDestinations-ms"
        if (Test-Path -LiteralPath $qaPath) {
            $qaDest = Join-Path $dest "QuickAccess"
            if (-not (Test-Path -LiteralPath $qaDest)) {
                New-Item -Path $qaDest -ItemType Directory -Force | Out-Null
            }
            Copy-Item -Path $qaPath -Destination $qaDest -Force -ErrorAction SilentlyContinue
        }
    } catch {
        Write-Output "Quick Access backup error: $_"
    }

    # ---- Network printers (CIM) ----
    try {
        $prtpath  = Join-Path $dest "PrintersBackup.csv"
        $printers = Get-CimInstance Win32_Printer | Where-Object { $https://urldefense.com/v3/__http://_.Network__;!!B5ObAA!FF0JNkBmVVGYKntI5NW5TSTgvhmK_nRVguFHFzHih_a3A44iFt3qvI6r3ITsWxdpzjw8vk10aSWq0tVqxixIy4_w$  -eq $true }
        if ($printers) {
            $printers | Select-Object Name, ServerName, ShareName | Export-Csv -Path $prtpath -NoTypeInformation -Encoding UTF8
            Write-Output "Network printers have been backed up to $prtpath"
        } else {
            Write-Output "No network printers found. No CSV file created."
        }
    } catch {
        Write-Output "Printer backup error: $_"
    }
}
# ===================== End of function =====================

# ===================== GUI: Present user with choice of what to backup =====================
# Create a new form
$form = New-Object System.Windows.Forms.Form
$form.Text = "User Migration"
$form.Size = New-Object System.Drawing.Size(320, 200)
$form.StartPosition = [System.Windows.Forms.FormStartPosition]::CenterScreen

# Multi-line label
$label = New-Object System.Windows.Forms.Label
$label.Text = "Please choose one of the following options:`nBoth - Backup Folders and Settings`nFolders - Only Backup Folders`nSettings - Only Backup Settings."
$label.Location = New-Object System.Drawing.Point(10, 20)
$label.Size = New-Object System.Drawing.Size(300, 60)
$label.AutoSize = $true
$form.Controls.Add($label)

# Result var
$global:result = $null

# Buttons
$bothButton = New-Object System.Windows.Forms.Button
$bothButton.Text = "Both"
$bothButton.Location = New-Object System.Drawing.Point(10, 100)
$bothButton.Size = New-Object System.Drawing.Size(90, 30)
$bothButton.Add_Click({ $global:result = "Both"; $form.Close() })
$form.Controls.Add($bothButton)

$foldersButton = New-Object System.Windows.Forms.Button
$foldersButton.Text = "Folders"
$foldersButton.Location = New-Object System.Drawing.Point(110, 100)
$foldersButton.Size = New-Object System.Drawing.Size(90, 30)
$foldersButton.Add_Click({ $global:result = "Folders"; $form.Close() })
$form.Controls.Add($foldersButton)

$settingsButton = New-Object System.Windows.Forms.Button
$settingsButton.Text = "Settings"
$settingsButton.Location = New-Object System.Drawing.Point(210, 100)
$settingsButton.Size = New-Object System.Drawing.Size(90, 30)
$settingsButton.Add_Click({ $global:result = "Settings"; $form.Close() })
$form.Controls.Add($settingsButton)

$form.AcceptButton = $bothButton

# Show the form
$form.ShowDialog() > $null

# ===================== Action Selection =====================
Add-Type -AssemblyName PresentationFramework

if ($global:result -eq $null) {
    [System.Windows.MessageBox]::Show("You canceled the dialog to select backup type`nNo Backup Done", "User Data Migration") | Out-Null
    New-Item -Path "C:\temp" -Name "backupdone.txt" -ItemType File -Force | Out-Null
    return
}

# Prepare destination and logging
Create-Dest
Start-Log
Set-RPFolderPermissions

switch ($global:result) {
    'Both' {
        Write-Output "User chose Both."
        Backup-Folders
        Backup-Settings
        [System.Windows.MessageBox]::Show("Backup of Folders and Settings is complete`nLocation: $dest", "User Data Migration") | Out-Null
    }
    'Folders' {
        Write-Output "User chose Folders."
        Backup-Folders
        [System.Windows.MessageBox]::Show("Backup of Folders is complete`nLocation: $dest", "User Data Migration") | Out-Null
    }
    'Settings' {
        Write-Output "User chose Settings."
        Backup-Settings
        [System.Windows.MessageBox]::Show("Backup of Settings is complete`nLocation: $dest", "User Data Migration") | Out-Null
    }
}

# Drop detector file and stop logging
try { New-Item -Path "C:\temp" -Name "backupdone.txt" -ItemType File -Force | Out-Null } catch {}
Stop-Log
