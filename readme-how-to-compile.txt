install SMFL 2.6.2 (https://www.sfml-dev.org/download/sfml/2.6.2/) for Visual C++
(all directories listed below must be changed to the ones you're using of course)

open the the program directory in x64 Native Tools Command Prompt for VS 2022
|
cd C:\Users\Pranav\Code\projects\raytracer\

before compilation, paste these commands into the command prompt
|
set SFML_DIR=C:\Users\Pranav\Libraries\SFML-2.6.2
set INCLUDE=%SFML_DIR%\include;%INCLUDE%
set LIB=%SFML_DIR%\lib;%LIB%

then, run the following command:
|
cl source-files/main.cpp source-files/canvasHandler.cpp source-files/rayTracer.cpp source-files/sceneDefinition.cpp source-files/vectorMath.cpp source-files/postProcessing.cpp source-files/BMPhandler.cpp /std:c++17 /EHsc /I"%SFML_DIR%\include" /link /LIBPATH:"%SFML_DIR%\lib" sfml-graphics.lib sfml-window.lib sfml-system.lib

you can now run the newly compiled main.exe file for the render output.



in order to change the output resolution:
|
go to the header file (canvas.hpp) and change the WIDTH and HEIGHT under the #define preprocessor.
it has to be in a 1.5:1 aspect ratio for now.