SET SRC_DIR=C:\Users\Boris\Work\yzg
SET DEST_DIR=E:\Dagor4\Prog\1stPartyLibs\daScript

echo syncing examples
call make_empty_folder.bat %DEST_DIR%\examples
call sync_folder.bat %SRC_DIR%\examples %DEST_DIR%\examples

echo syncing generated
call make_empty_folder.bat %DEST_DIR%\generated
call sync_folder.bat %SRC_DIR%\generated %DEST_DIR%\generated

echo syncing utils
call make_empty_folder.bat"%DEST_DIR%\utils
call sync_folder.bat %SRC_DIR%\utils %DEST_DIR%\utils

echo syncing include
call make_empty_folder.bat %DEST_DIR%\include
call sync_folder.bat %SRC_DIR%\include %DEST_DIR%\include
call remove_folder.bat %DEST_DIR%\include\dag_noise
call remove_folder.bat %DEST_DIR%\include\vecmath

echo syncing src
call make_empty_folder.bat %DEST_DIR%\src
call sync_folder.bat %SRC_DIR%\src %DEST_DIR%\src

echo applying patch
xcopy %DEST_DIR%\patch %DEST_DIR% /D /I /E /F /Y /H /R
