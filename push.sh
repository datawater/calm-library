#!/usr/bin/env bash

clear;

# ask for the commit message
echo "Enter the commit message: ";
read commitMessage;

git commit -am "$commitMessage";

# ask for the name of the branch
echo "Enter the name of the branch: ";
read branchName;

# push to it
git push origin $branchName;