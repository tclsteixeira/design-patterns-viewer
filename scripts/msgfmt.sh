#!/bin/bash

#Finally, to use translations in our application, these PO files are converted to MO (Machine Object) files.

msgfmt --output-file=../locale/pt/LC_MESSAGES/DPViewergtk.mo ../locale/pt/DPViewergtk.po
