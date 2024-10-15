#!/bin/bash

cd src
g++ -std=c++17 'main.cpp tokeniser.cpp throw.cpp main.cpp' -o '../build/osync'
