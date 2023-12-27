#!/bin/bash

# Specify the directories
directories=(
    "./"
    "./models"
    "./controllers"
	"./views"
)

# Use find to get all *.c files in the specified directories
c_files=$(find "${directories[@]}" -type f -regex ".*\.\(c\|h\)" -exec printf "%s " {} + | sed 's/,$//')

# Print the list of *.c *.h files
echo "List of *.c and *.h files:"
echo "$c_files"
