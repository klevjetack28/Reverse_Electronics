#! /bin/bash

RECIPE_FILE="$1"

echo "" > "$RECIPE_FILE"

find "./Data/" -type f -exec cat {} + >> "$RECIPE_FILE"

echo -e "\nSignalEntry signalMap[] = {" >> "$RECIPE_FILE"

grep -aPo 'const\s+uint16_t\s+\K\w+(?=\s*\[\s*\]\s*=)' "$RECIPE_FILE" | while read -r name; do
    echo -e "\t{\"$name\", $name, sizeof($name) / sizeof(uint16_t) }, " >> "$RECIPE_FILE"
done

echo -e "};\n" >> "$RECIPE_FILE"
cat "$RECIPE_FILE" | xclip -selection clipboard
