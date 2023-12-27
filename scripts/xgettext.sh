#!/bin/bash

# Specify the directories
directories=(
    "../src"
    "../src/models"
    "../src/controllers"
	"../src/views"
)

# Use find to get all *.c ans *.h files in the specified directories
sources=$(find "${directories[@]}" -type f -regex ".*\.\(c\|h\)" -exec printf "%s " {} + | sed 's/,$//')

xgettext --keyword=_ -o ../locales/base.pot  --language=C $sources 
