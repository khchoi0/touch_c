Clear-Host

$files = @(
	".\atms.exe", 
	".\central.exe", 
	".\trans711.txt", 
	".\trans713.txt", 
	".\transSorted711.txt", 
	".\transSorted713.txt", 
	".\transSorted.txt", 
	".\updatedMaster.txt", 
	".\negReport.txt"
)

foreach ( $file in $files ) { if (Test-Path $file) { Remove-Item $file } }

gcc -o atms .\atms.c
gcc -o central .\central.c .\sort.c

Get-Content .\testcase\testcase.txt | .\atms.exe
.\central.exe

if ( Compare-Object (Get-Content .\updatedMaster.txt) (Get-Content .\testcase\reference_updatedMaster.txt) ) {
	Write-Error "TESTCASE FAILED"
} 
else {
	Write-Output "TESTCASE PASSED"
}

foreach ( $file in $files ) { if (Test-Path $file) { Remove-Item $file } }
