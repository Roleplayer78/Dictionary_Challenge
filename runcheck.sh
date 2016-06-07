#!/bin/bash

time ./spellcheck dictionary_short.txt document_short.txt

read -t 240 -p "Press a key to run long test"

time ./spellcheck dictionary_long.txt document_long.txt

