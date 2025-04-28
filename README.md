# Rebase++: an interactive rebase editor project to learn myself some C++ & Qt

## Build

```shell
> make configure
> make install INSTALL_ROOT=/somewhere/in/your/path
```

It generates:

- `/somewhere/in/your/path/newbase/` the folder with all the project's artifacts
- `/somewhere/in/your/path/newbase.bat` the launcher

Now you can call `newbase` as a drop-in replacement of `git rebase`, e.g.

```shell
> newbase HEAD~5
```

## Platforms

- Developed and tested on and for Windows