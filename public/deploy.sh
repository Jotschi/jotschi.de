#!/bin/bash

jekyll
rsync -rav _site root@www:/var/www/jotschi.github.com

