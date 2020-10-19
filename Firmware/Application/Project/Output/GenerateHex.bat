@ECHO OFF

ECHO.
ECHO /**********************
ECHO  * CREATING HEX FILE  *
ECHO  **********************/
ECHO.

ECHO Change Directory...
CD Output

SETLOCAL
SET Line = ""
SET FirstQuote = 0

FOR /F "tokens=* delims=" %%X in ('type ".\..\Inc\vichydro\config\Version.h"^| Find /i "#define FW_VERSION "') DO SET Line=%%X > NUL

SET SStrg="^""
SET StrgTemp=%Line%&SET Position=0
:loop
SET /a Position+=1
ECHO %StrgTemp%|FINDSTR /b /c:"%SStrg%" > NUL
IF ERRORLEVEL 1 (
SET StrgTemp=%StrgTemp:~1%
IF DEFINED StrgTemp GOTO loop
SET Position=0
)
SET /a FirstQuote=%Position%

ECHO.%Line%>SVersion
FOR %%a IN (SVersion) DO SET /a SVersion=%%~za -2
SET /a SVersion=%SVersion%-%FirstQuote%-1

CALL SET FirmVersion=%%Line:~%FirstQuote%,%SVersion%%%
SET FirmVersion=%FirmVersion:~0,-1%
DEL SVersion

REM Generate Package Zip
ECHO.
ECHO Copy Hex file...
xcopy /y ..\Debug\vichydro.hex ..\Output\hex\vichydro_v%FirmVersion%.hex

ECHO vichydro_v%FirmVersion%.hex

ECHO.
