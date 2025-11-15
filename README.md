 # Pigeons v0.1
Just a simple combat with a potential storyline but currently just working on the combat and little bit of graphics as well.

# Building on windows

* Note : You need MSVC with C++20 support 

If you are MSVC compiler , on your developer powershell , navigate to the project directory and run 
``` cl /std:c++20 .\build.cpp ```
``` .\build.exe```
The build folder should have been created and populated with the game

## Building with minGW

If using GNU c++ compiler , please use [https://github.com/skeeto/w64devkit/]
Navigate to the project source tree root and run 
``` g++ build.cpp -o build```
``` ./build```
The build folder should have been created and populated with the game

[!WARNING]
Clang is not currently supported and is getting worked on 

# Building on Linux 

In the root of the source tree , run  
``` g++ build.cpp -o build ```
``` ./build ```

or if on clang , run 
``` clang++ build.cpp -o build ```
``` ./build ``` 

current progress:
<img width="1079" height="759" alt="image" src="https://github.com/user-attachments/assets/d5ae27c6-d14e-44e7-b867-2a224cb52dc2" />
  
