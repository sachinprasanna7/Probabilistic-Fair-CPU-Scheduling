@echo off

REM Compile and run main.cpp
g++ main.cpp -o main.exe
main.exe

REM Run plot_timequantum.py
python plot_timequantum.py

REM Run plot_k.py
python plot_k.py