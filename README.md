# sddm-conf

[![CI]](https://github.com/qtilities/sddm-conf/actions/workflows/build.yml)

## Overview

Configuration editor for [SDDM] similar to [sddm-config-editor],
but written in C++.

![Screenshot](resources/screenshot.png)

## Dependencies

Runtime:

- Qt5/6 base
- SDDM
- [polkit] (to save the settings)

Build:

- CMake
- Qt Linguist Tools
- [Qtilitools] CMake modules
- Git (optional, to pull latest VCS checkouts)

## Build

`CMAKE_BUILD_TYPE` is usually set to `Release`, though `None` might be a valid [alternative].<br>
`CMAKE_INSTALL_PREFIX` has to be set to `/usr` on most operating systems.<br>
Using `sudo make install` is discouraged, instead use the system package manager where possible.

```bash
cmake -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr -W no-dev
cmake --build build --verbose
DESTDIR="$(pwd)/package" cmake --install build
```

## Packages

[![Packaging status]](https://repology.org/project/sddm-conf/versions)

## Translations

For contributing translations the [LXQt Weblate] platform can be used.

[![Translation status]](https://translate.lxqt-project.org/widgets/qtilities/)


[alternative]:        https://wiki.archlinux.org/title/CMake_package_guidelines#Fixing_the_automatic_optimization_flag_override
[CI]:                 https://github.com/qtilities/sddm-conf/actions/workflows/build.yml/badge.svg
[LXQt Weblate]:       https://translate.lxqt-project.org/projects/qtilities/sddm-conf/
[Packaging status]:   https://repology.org/badge/vertical-allrepos/sddm-conf.svg
[polkit]:             https://gitlab.freedesktop.org/polkit/polkit/
[Qtilitools]:         https://github.com/qtilities/qtilitools/
[SDDM]:               https://github.com/sddm/sddm/
[sddm-config-editor]: https://github.com/lxqt/sddm-config-editor/
[Translation status]: https://translate.lxqt-project.org/widgets/qtilities/-/sddm-conf/multi-auto.svg
