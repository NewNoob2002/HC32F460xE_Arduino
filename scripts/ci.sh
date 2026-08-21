#!/usr/bin/env sh
set -eu

repo_dir=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$repo_dir"

cmake --preset host-tests
cmake --build --preset host-tests
ctest --preset host-tests
