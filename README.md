# How to Build

Requires:
- gcc
- GTK 3 (or GTK 4)

## Steps to Build
1. cd path-to-your-project
2. gcc main.c fighters.c -o run.exe `pkg-config --cflags --libs gtk+-3.0`
3. ./run.exe
