#!/bin/bash

run()
{
    echo "here's the result of a basic regexp search for pointers in headers:"
    echo
    ag '\w \*\w' `find . -name '*.h'`
}

run | less

