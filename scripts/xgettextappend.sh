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

# If you added new strings or source files to translate this will preserve existing translations.
# Just added the -j option.
xgettext -j --keyword=_ -o ../locales/DPViewergtk.pot  -language=C $sources

