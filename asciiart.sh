#!/usr/bin/env bash

mwfractal -q -o ascii.png "$@"
test -f ascii.png && asciiart -c ascii.png
