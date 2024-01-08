#!/bin/bash

# It contains every localized text. We use it to create files containing other language versions.
# If you added new strings or source files to translate this will merge with existing ones.
msgmerge --input=../locale/DPViewergtk.pot --locale=pt --output-file=../locale/pt/DPViewergtk.po
