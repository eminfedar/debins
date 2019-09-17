# Debins
a Simple One-Click .Deb Package Installer

### You can `install` a .deb file:
![Image](https://raw.githubusercontent.com/eminfedar/debins/dev-unstable/debins2.png)

### You can `reinstall` or `remove` a package from a .deb file:
![Image](https://raw.githubusercontent.com/eminfedar/debins/dev-unstable/debins1.png)

### You can `upgrade` or `downgrade` a package from a .deb file:
![Image](https://raw.githubusercontent.com/eminfedar/debins/dev-unstable/debins3.png) ![Image](https://raw.githubusercontent.com/eminfedar/debins/dev-unstable/debins4.png)

## Building:

### Requirements:

    Qt 5.9 (or >=)
    QtCreator (optional, you can build it from qtcreator just by opening the project on it and building it)

### Building WITHOUT QtCreator:
```
cd debins/ #path of the .pro file
mkdir build
cd build
qmake ../
make
```
