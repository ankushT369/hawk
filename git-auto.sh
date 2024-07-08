#!/bin/bash

# Add all changes
git add .

# Commit with a default message or prompt for a message
commit_message="Your default commit message here"
read -p "Enter commit message: " custom_message
commit_message="${custom_message:-$commit_message}"
git commit -m "$commit_message"

# Push changes to origin main
git push origin main

