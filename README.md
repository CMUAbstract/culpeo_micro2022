# Full Application Testing with Culpeo-R

This repo contains the applications, supporting libraries and final results for
the full applications described in the MICRO 2022 paper "An Architectural Charge
Management Interface for Energy-Harvesting Systems", Figures 12 and 13 [0].

The primary contribution of this repo is the library that implements CatNap[1]
for high ESR devices using the Culpeo-R strategy.

## Basic layout

- apps (test appications)
  - responsive\_reporting
  - noise\_montior
  - periodic\_sensing
  - ...
- ext (libraries)
  - libfakecatnap (Catnap scheduler with Culpeo extensions, naming convention
    explained in library)
  - ...
- tools
  - maker (custom build utility)[2]
  - scripts (python scripts to extracts results)
    - data (test run results)

## Cloning the repo

As a heads up, this repo relies heavily on git submodules, so keep that in mind
while hacking on it. To clone the repo, run:

`git clone --recursive git@github.com:CMUAbstract/culpeo_catnap_tests.git `

## Dependencies
- MSPgcc version 8.3 (See instructions [here](Artibeus))
- MSPdebug version 0.25 (See instructions [here](Artibeus))
- Python3 (pandas, numpy, matplotlib)

## Hardware

- Hardware Required
  - Capybara sensor platform[3] with fixed size buffer. This is achieved by
    disconnecting the "VCAP" rail and connecting an external, fixed size
    buffer. Contact us for more specific instructions.
  - MSPFet programmer
- Hardware Used in Testing (more details in [Notes](Notes.md))
  - Saleae Logic Analyzer
  - Arduino (code not provided)
  - 3.3V voltage regulator and potentiometer circuit
- See instructions in previous [tutorial](TODO) for connecting the programmer to
  the Capybara
## Building an application

Run the following commands *from this top level directory* to clean, build and
program an application on the Capybara dev board:

 `make apps/<app name>/bld/gcc/clean;`

 `make apps/<app name>/bld/gcc/depclean;`

 `make apps/<app name>/bld/gcc/all;`

 `make apps/<app name>/bld/gcc/prog;`


E.g.
 `make apps/responsive_reporting/bld/gcc/clean;`

 `make apps/responsive_reporting/bld/gcc/depclean;`

 `make apps/responsive_reporting/bld/gcc/all;`

 `make apps/responsive_reporting/bld/gcc/prog;`


[0] E. Ruppel, M. Surbatovich, H. Desai, K. Maeng and B. Lucia, "An
Architectural Charge Management Interface for Energy-Harvesting Systems," MICRO,
2022.

[1] K. Maeng and B. Lucia, “Adaptive low-overhead scheduling for periodic and
reactive intermittent execution,” in Proceedings of the 41st ACM SIGPLAN
Conference on Programming Language Design and Implementation, 2020, pp.
1005–1021

[2] Maker: a dependency builder for C code. Alexei Colin.
https://github.com/CMUAbstract/maker.git

[3] A. Colin, E. Ruppel, and B. Lucia, “A reconfigurable energy storage
architecture for energy-harvesting devices,” in Proceedings of the Twenty-Third
International Conference on Architectural Support for Programming Languages and
Operating Systems, ser. ASPLOS ’18.  New York, NY, USA: ACM, 2018, pp. 767–781.
