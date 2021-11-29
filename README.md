# sddm-conf

## Overview

Configuration editor for [SDDM][1] similar to [sddm-config-editor][2],
but made in C++ only.

![Screenshot](resources/screenshot.png)

## Build

Runtime dependencies are SDDM, Qt5 base and [polkit][3] for saving settings.
Build dependencies are CMake, Qt5LinguistTools
and optionally Git to pull latest VCS checkouts.

CMake variable `CMAKE_INSTALL_PREFIX` has to be set to `/usr` on most operating systems.

```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -B build
make -C build
make DESTDIR="$(pwd)/package" install -C build
```

## Packages

[![Packaging status](https://repology.org/badge/vertical-allrepos/sddm-conf.svg)][4]


[1]: https://github.com/sddm/sddm/
[2]: https://github.com/lxqt/sddm-config-editor/
[3]: https://gitlab.freedesktop.org/polkit/polkit/
[4]: https://repology.org/project/sddm-conf/versions
