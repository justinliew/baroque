@echo off

set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4211 -wd4100 -wd4189 -wd4505 -wd4244 -DBAROQUE_INTERNAL=1 -DBAROQUE_SLOW=1 -DBAROQUE_WIN32=1 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib

REM TODO - can we just build both with one exe?

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM 32-bit build
REM cl %CommonCompilerFlags% ..\code\win32_baroque.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

REM 64-bit build
del *.pdb > NUL 2> NUL
REM Optimization Switches /O2 /Oi /fp:fast
set PDBNAME=baroque_%random%.pdb
set PDBNAME=%PDBNAME: =0%
cl %CommonCompilerFlags% ..\code\baroque.cpp -Fmwbaroque.map /LD /link -incremental:no -PDB:%PDBNAME% /DLL /EXPORT:GameUpdateAndRender /EXPORT:GameGetSoundSamples
cl %CommonCompilerFlags% ..\code\win32_baroque.cpp -Fmwin32_baroque.map /link %CommonLinkerFlags%
popd
