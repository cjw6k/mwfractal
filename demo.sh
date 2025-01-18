#!/usr/bin/env bash

./mwfractal -q -o demo.png "$@"
test -f demo.png && asciiart demo.png
