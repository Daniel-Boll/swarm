#!/usr/bin/env bash

xmake && xmake project -k compile_commands
if [ "$1" = "r" ] || [ "$1" = "run" ]; then
	xmake run "${@:2}"
fi
