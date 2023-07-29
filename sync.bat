@ECHO OFF

SET SRC_DIR=C:\Users\Boris\Work\yzg
SET DEST_DIR=E:\Dagor4\Prog\1stPartyLibs\daScript
SET PATCH_DIR=%SRC_DIR%\patch

echo create patch
call make_empty_folder.bat %PATCH_DIR%
xcopy %DEST_DIR%\jamfile.* %PATCH_DIR% /S > NUL
If %ERRORLEVEL% NEQ 0	ECHO CREATE PATCH %1 FAILED

echo syncing examples
call make_empty_folder.bat %DEST_DIR%\examples
call sync_folder.bat %SRC_DIR%\examples %DEST_DIR%\examples
call remove_folder.bat %DEST_DIR%\examples\profile\tests\angelscript
call remove_folder.bat %DEST_DIR%\examples\profile\tests\squirrel
call remove_folder.bat %DEST_DIR%\examples\profile\tests\lua
call remove_folder.bat %DEST_DIR%\examples\profile\tests\js

echo syncing utils
call make_empty_folder.bat %DEST_DIR%\utils
call sync_folder.bat %SRC_DIR%\utils %DEST_DIR%\utils

echo syncing include
call make_empty_folder.bat %DEST_DIR%\include
call sync_folder.bat %SRC_DIR%\include %DEST_DIR%\include
call remove_folder.bat %DEST_DIR%\include\dag_noise
call remove_folder.bat %DEST_DIR%\include\vecmath

echo syncing src
call make_empty_folder.bat %DEST_DIR%\src
call sync_folder.bat %SRC_DIR%\src %DEST_DIR%\src

echo syncing src
call make_empty_folder.bat %DEST_DIR%\daslib
call sync_folder.bat %SRC_DIR%\daslib %DEST_DIR%\daslib

echo syncing doc
call make_empty_folder.bat %DEST_DIR%\doc
call sync_folder.bat %SRC_DIR%\doc %DEST_DIR%\doc

echo syncing site
call make_empty_folder.bat %DEST_DIR%\site
call sync_folder.bat %SRC_DIR%\site %DEST_DIR%\site

echo applying patch
xcopy %PATCH_DIR% %DEST_DIR% /D /I /E /F /Y /H /R > NUL
If %ERRORLEVEL% NEQ 0	ECHO APPLY PATCH FAILED

echo removing patch
call remove_folder.bat %PATCH_DIR%
