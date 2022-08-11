@echo off

set timer_name=sc7.ctm

@rem ctime -begin %timer_name%

@rem We put the relative path to external headers here
set IncludeFlags=-I..\external 

set comp_flags1=/Od /FC /GR- /EHa- /TC /std:c11 /MTd  /nologo /Zi
set comp_flags2=%IncludeFlags% /wd4576

set link_flags=/NODEFAULTLIB:MSVCRT ../external/raylibdll.lib winmm.lib

@rem here we go into the build folder to run the compile command, relative include paths should work here.
IF NOT EXIST .\build mkdir .\build
pushd .\build

cl %comp_flags1% %comp_flags2% ..\src\main.c /link %link_flags% 
set LastError=%ERRORLEVEL%
popd

@rem ctime -end %timer_name% %LastError%
IF NOT %LastError%==0 GOTO :end


:end