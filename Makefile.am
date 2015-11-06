#
#   Copyright 2013 Neil E. Moore, Christopher J. Willcock
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#

bin_PROGRAMS=mwfractal

AM_CPPFLAGS=$(MAGICK_CFLAGS)

mwfractal_LDADD=$(MAGICK_LIBS)

mwfractal_SOURCES=src/main.cpp src/utils.cpp src/utils.hpp src/ProgramOptions.cpp src/ProgramOptions.hpp src/generators/Generator.cpp src/generators/Generator.hpp src/generators/JuliaPowExp.cpp src/generators/JuliaPowExp.hpp src/generators/Julia3.cpp src/generators/Julia3.hpp src/generators/Julia.cpp src/generators/Julia.hpp src/generators/Mandelbrot.cpp src/generators/Mandelbrot.hpp src/colourizers/Colourizer.cpp src/colourizers/Colourizer.hpp src/colourizers/NEM_Violet_to_Red.cpp src/colourizers/NEM_Violet_to_Red.hpp src/colourizers/NEM_Violet_to_Red_Lightness.cpp src/colourizers/NEM_Violet_to_Red_Lightness.hpp src/colourizers/NEM_White_to_Black.cpp src/colourizers/NEM_White_to_Black.hpp src/colourizers/MW_Darts_Violet_to_Red.cpp src/colourizers/MW_Darts_Violet_to_Red.hpp

EXTRA_DIST = bootstrap LICENSE NOTICE
