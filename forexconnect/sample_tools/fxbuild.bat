@echo off
@REM ****************************************************************************
@REM * Description: Build sample_tools library
@REM * Parameters:
@REM *      %1 (optional) debug|release. Default value is release.
@REM *      %2 (optional) x86|x64. Default value is x86.
@REM *
@REM ****************************************************************************


IF /i "%1" == "debug" (
    set config=Debug
) ELSE (
    IF /i "%1" == "release" (
        set config=Release
    ) ELSE (
        set config=Release
    )
)

IF /i "%2" == "x64" (
    set platf=x64
) ELSE (
    set platf=Win32
)

call "%VS80COMNTOOLS%vsvars32.bat"

vcbuild sample_tools.vcproj "%config%|%platf%"
