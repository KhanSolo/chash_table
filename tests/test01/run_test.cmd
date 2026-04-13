@echo off
setlocal

:: Путь к программе
set APP="../../build/debug/chash_table.exe"

:: Файлы вход/выход/ожидаемый результат
set INPUT=input.txt
set OUTPUT=actual.txt
set EXPECTED=expected.txt

:: 1. Запуск программы: вход из файла, вывод в файл
echo Start Time: %time%
%APP% < "%INPUT%" > "%OUTPUT%" 2>&1
echo End Time: %time%

:: 2. Сравнение вывода с ожидаемым
fc "%OUTPUT%" "%EXPECTED%" > nul
if %ERRORLEVEL% == 0 (
    echo OK: output matches expected
) else (
    echo FAIL: output differs from expected
    echo You can inspect:
    echo   Input file:  %INPUT%
    echo   Actual:      %OUTPUT%
    echo   Expected:    %EXPECTED%
)

endlocal