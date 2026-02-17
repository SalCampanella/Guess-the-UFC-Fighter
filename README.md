# About the Project
Thanks so much for viewing my project. This was a passion project for me and something I built for myself to play and enjoy. However, I feel like other people can find the same enjoyment from it. More updates to the GUI are on the way. 

# How to Play
When you run the game, it will choose a random UFC fighter from the list, and your goal is to attempt to guess that fighter in as little amounts of guesses as possible. Each guess you make will provide you with information to help you with the next guess you make. You must use that information and your own knowledge of UFC fighters to guess the correct fighter. Good Luck and Enjoy.

# How to Build

Requires:
- gcc
- GTK 3 (or GTK 4)

## Steps to Build
1. cd path-to-your-project
2. gcc main.c fighters.c -o run.exe 'pkg-config --cflags --libs gtk+-3.0'
3. ./run.exe
