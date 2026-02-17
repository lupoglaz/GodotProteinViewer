#!/bin/bash

EXTENSION_NAME="BioTools"
LIB_NAME="lib$EXTENSION_NAME"
MIN_VERSION="4.2"

/bin/cat > plugin.cfg <<EOF
[plugin]
name="$EXTENSION_NAME"
description="Tools for bioinformatics"
author="Georgy Derevyanko"
version="$MIN_VERSION"
script="main.gd"
EOF

if [[ -z "$1" ]]; then

scons
/bin/cat >"$LIB_NAME".gdextension << EOF
[configuration]

entry_symbol = "gameplay_library_init"
compatibility_minimum = $MIN_VERSION

[libraries]

linux.debug.x86_64 = "res://addons/$EXTENSION_NAME/$LIB_NAME.linux.template_debug.x86_64.so"
linux.release.x86_64 = "res://addons/$EXTENSION_NAME/$LIB_NAME.linux.template_debug.x86_64.so"
EOF

elif [[$1="debug" || $1="d"]]; then

scons target=template_debug debug_symbols=yes dev_build=yes
/bin/cat >"$LIB_NAME".gdextension << EOF
[configuration]

entry_symbol = "gameplay_library_init"
compatibility_minimum = $MIN_VERSION

[libraries]

linux.debug.x86_64 = "res://addons/$EXTENSION_NAME/$LIB_NAME.linux.template_debug.dev.x86_64.so"
linux.release.x86_64 = "res://addons/$EXTENSION_NAME/$LIB_NAME.linux.template_debug.dev.x86_64.so"
EOF

fi