SET "TKPATH=../MexTK/"
SET "FSPATH=../UnmeleeFS/files/"

:: compile main code
"%TKPATH%MexTK.exe" -ff -i "MatchCodes.c" -s grFunction -t "%TKPATH%grfunction.txt" -l "%TKPATH%melee.link" -dat "%FSPATH%GrNLa.dat" -ow

pause