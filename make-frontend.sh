​#!/bin/bash

# this just minimize the html css js files

# Cleanup
rm -rf data
mkdir data


# Minify external JS
uglifyjs wserver/script.js -o data/script.js --compress --mangle

# Minify external CSS
cleancss -o data/style.css wserver/style.css

# Minify HTML (with inline JS/CSS minified too)
html-minifier --collapse-whitespace --remove-comments \
              --minify-css true --minify-js true \
              -o data/index.html wserver/index.html

echo "Done!"