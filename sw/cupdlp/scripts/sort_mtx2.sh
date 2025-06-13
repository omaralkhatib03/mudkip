#!/bin/bash

for file in mtx2/*.mtx; do
    echo "Processing $file"
    python3.11 ./scripts/sort_mtx_inplace.py "$file" --method nnz
done
