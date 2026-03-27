#!/bin/bash
BASE_DIR="$(pwd)"
for file in "$BASE_DIR"/*; do
    if [ -f "$file" ]; then
        filename=$(basename -- "$file")
        extension="${filename##*.}"
        # Skip files without extension
        if [ "$filename" = "$extension" ]; then
            continue
        fi
        extension=$(echo "$extension" | tr '[:upper:]' '[:lower:]')
        mkdir -p "$BASE_DIR/$extension"
        # Avoid moving the script itself
        if [ "$filename" != "$(basename "$0")" ]; then
            # Extract filename without extension
            name="${filename%.*}"
            # Get file creation/modification time
            timestamp=$(date -r "$file" +"%Y_%m_%d__%H_%M_%S")
            # Create new filename
            newname="${name}_${timestamp}.${extension}"
            mv "$file" "$BASE_DIR/$extension/$newname"
        fi
    fi
done
echo "All files organized and renamed successfully!"