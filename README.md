# Lost Asset Packager
Lost Asset Packager is software that can pack individual files into one file and then extract it. It is a lossless system as it puts all the files bytes one after another.

## Usage
To use this software, build with cmake and in your terminal run 
```
lapw --batch  <assets source> <output name>"
```

## How it works
It works by making a header that has a version and points to the footer location

Then it makes a footer that has the sum of all the assets and also an entry for each asset which contains:

Name length (2 bytes)

Name string

Offset in file

Size of data

Then it seeks back to the beginning of the file and patches the placeholder value in the header.

