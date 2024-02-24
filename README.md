# 1brc-challenge

Code sharing and development environment for Meetups attendees :coffee:

The main purpose of this repo is to share code and knowledge: the opportunity is given by the [One Billion Row Challenge (1brc)](https://github.com/gunnarmorling/1brc). The challenge simply consists on parsing a table and computing statistics in the shortest time, the trouble factor being the 1 BILLION rows making up the table :scream:

## Repo Structure

- `example` : 
    - possible implementations of the challenge, e.g. `frederic_1.cpp`. 
    - **Note** each `attendee_name_XYZ.cpp` file is meant to be own by the sole attendee, please do not modify their script, please do not fix their broken toys :joy:
- `example/data`: contains light versions of the data table. 
- `src/include` : whatever header-only lib we end up with (maybe ... maybe not ...) 
- `test/unit-test`: if some components of the putative lib need dedicated unit tests, that will be the place to write them!
- `.devcontainer` : a Docker + Visual Studio Code integration supporting
    - gcc-12
    - cmake
    - vcpkg
    - jdk-21
    - please do not break :sweat_smile:

## How To Use

1. Install Docker
2. Install VSC 
3. Make sure the proposed [Devcontainer Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) is installed in VSC
3. `git clone https://github.com/Meetup-Cpp-Warm-Coffee-in-Paris/1brc-challenge.git`
4. In VSC with Docker running in the background, press F1 to bring up the Command Palette and type in `Dev Containers` for a full list of commands
5. `Dev Container: Open Folder In Container`
6. Once the image is built, use the command palette `CMD + Maj + P` > `Run Task` > `Run Unit Test (Debug)`
7. If this does not work (it never does), please let us know ahaha !

## To generate the full dataset

```
git clone https://github.com/gunnarmorling/1brc.git gunnarmorling-1brc
cd gunnarmorling-1brc
./mvnw clean verify
./create_measurements.sh 1000000000
./calculate_average_baseline.sh
```