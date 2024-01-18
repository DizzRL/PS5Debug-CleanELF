# PS5Debug CleanELF

# Info
This is a project for a clean elf to inject into a game using [ps5debug](https://github.com/DizzRL/ps5debug)

I have included 2 formats, a Visual Studio project format that uses WSL to build, and just a vanilla project


# WARNING
This clean elf assumes you are using ps5-kstuff, as it has an mprotect patch in it so we can use `sceKernelMProtect` function, which is used in the `DetourFunction` stuff to make the `malloc` address RWX

## Building
Building is pretty simple, if using the Visual Studio project, just hit Build -> Rebuild Solution.
If using the vanilla project, just run `make`
