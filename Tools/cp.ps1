Set-Location ..
Set-Location .\CsSlave_App

$time = get-date
$new_folder_name = "CsSlave_Custom" + $time.ToString('yyyyMMdd')
$relative_path = "../../" + $new_folder_name

if (test-path $relative_path) {
    write-host "path exist" -Foregroundcolor red
    exit
    
}
else {
    write-host "path ok" -Foregroundcolor green
    mkdir $relative_path
    $inc = $relative_path + "\Inc"
    $src = $relative_path + "\Src"
    $project = $relative_path + "\MDK_ARM"
    $legacy = $inc + "\Legacy"
    $legacy_file = $inc + "\stm32_hal_legacy.h"
    mkdir $inc
    mkdir $src
    mkdir $project
    mkdir $legacy
    Get-ChildItem * -include "*.h" -recurse | foreach-object {Copy-Item $_.FullName $inc}
    Copy-Item Src\main.c $src
    Copy-Item Src\stm32f1xx_it.c $src
    Copy-Item Src\mtp.c $src
    Copy-Item MDK-ARM\startup_stm32f103xe.s $src
    Copy-Item ../Tools/CsSlave_Custom.uvprojx $project
    Move-Item $legacy_file $legacy
    Get-ChildItem * -include "*.lib" -recurse | foreach-object {Copy-Item $_.FullName $src}
}