#!/bin/bash

echo "This script applies local configuration "
echo "that will avoid headeaches when updating in the future."


# Config


# Show changes in submodules by default
git config status.submoduleSummary true

# Show diff in submodules by default
git config diff.submodule log

# When pushing, also push submodules
git config push.recurseSubmodules on-demand

# All operations on submodules also propagates to their submodules, if any (currently, none).
git config --global submodule.recurse true


# Action


# In case submodules URL have changed, heed it.
git submodule sync --recursive

# Actually update local files based on what they should be
git submodule update --recursive
