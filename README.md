# PSYTRON
### A love story of delirium and madness with Artificial Inteligence (and a capybara)

## Images

![Your job is to control a hot robot.](/works_17.png "destroy everything on the screen!")

## Summary

PsyTron is a **VisualNovel** + **ShmUp** _bullet hell mix_, Made in my spare time along the last 2 years, struggling against boredom.
You can change the language setup to Spanish by uncommenting the line _"DialogFile: Data/prologo.yml"_ and adding a **#** character before the _"DialogFile: Data/intro.yml"_ line in **config.yml** file (along Fullscreen mode and some other options)
## Links

A full running build of the game can be found here 
[Psytron.Zip](https://drive.google.com/file/d/1MwiKKg_xmkjoCQsSBUNAeMbdteO-aVVh/view?usp=drive_link).

## Inputs:

| Key       |                        Action |
|----------:|:------------------------------|
| W         | Up                            |
| S         | Down                          |
| D         | Right                         |
| A         | Left                          |
| Ctrl/Space| Shoot/Select option           |
| Escape    | Return Title Menu             |
| Q         | Quit                          |



### MVS Project required libraries:
1. Psytron\gameCore\Libraries\\**raylib** (lib used to handle window, sprites and sound FX, etc)
2. Psytron\gameCore\Libraries\\**yaml-cpp** (required to load & read _**.YAML**_ config and dialog files)
- Both static libs are already built & included in the project folder ;)


#### Visual Studio project folder setup:
* General Properties:
	* Output Folder:
	    * `$(SolutionDir)$(Platform)\$(Configuration)\`
	* Intermediate Folder:
		* `$(ProjectDir)\Intermediate\$(Platform)\$(Configuration)\`

* C/C++:
	* Additional include Folders:
		* `Libraries/raylib/include; %(AdditionalIncludeDirectories)`
		* `Libraries/raylib/include/external/glfw/include;`

* Linker:
	* Additional libraries Folders
		* `Libraries/raylib/bin/$(Platform)/$(Configuration)/`

	* Input:
		* `raylib.lib;winmm.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)`


* To include in **main.cpp**:
    ```
    #if defined _WIN32 && !_DEBUG
    #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
    #endif
    ```

* copy `/y ".\resources\*.*" "$(OutDir)"`

* Just in case Original raylib project Linker Input gets modified:
    ```
    ..\raylib\MinSizeRel\raylib.lib
    opengl32.lib
    glu32.lib
    winmm.lib
    ..\raylib\external\glfw\src\MinSizeRel\glfw3.lib
    kernel32.lib
    user32.lib
    gdi32.lib
    winspool.lib
    shell32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
    comdlg32.lib
    advapi32.lib
    ```
