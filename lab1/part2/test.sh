#!/bin/bash
(IFS=':'; for path in $PATH; do echo "$path"; done)
