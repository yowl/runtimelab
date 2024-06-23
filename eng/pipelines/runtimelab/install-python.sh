#!/usr/bin/env bash

as --version

#echo Adding Python to path     $1/python/bin
echo '##vso[task.setvariable variable=EMSDK_PYTHON]'/usr/bin/python3