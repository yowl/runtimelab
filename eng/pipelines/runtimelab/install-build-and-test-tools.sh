#!/usr/bin/env bash
set -e
dir=$(dirname "$0")
echo; $dir/install-python.sh
echo; $dir/install-lldb.sh
