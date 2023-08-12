# gust

Sample application that can be used to issue a number of http requests to any given URL.

## Dependencies

    libcurl

To install run:

    # osx
    brew install curl
    # linux
    sudo apt-get update && apt-get install -y curl

## Compile

    make

## syntax

    gust [-n count] [-i interval] [-x verb] [-f] [-q] [-v] URL

    -n count    - Number of times to test URL.
    -i interval - Time between requests.
    -x verb     - Http method.
    -f          - Follow redirects.
    -q          - Quite mode.
    -v          - Verbose output.
