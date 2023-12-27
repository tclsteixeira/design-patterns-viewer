#!/bin/bash

#Finally, to use translations in our application, these PO files are converted to MO (Machine Object) files.

msgfmt --output-file=../locales/pt/LC_MESSAGES/base.mo ../locales/pt/base.po
