/*  Copyright 2013 Neil E. Moore, Christopher J. Willcock

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */

#ifndef PROGRAMOPTIONS_HPP
#define	PROGRAMOPTIONS_HPP

#include <string>

namespace JS{

class ProgramOptions{

public:
    ProgramOptions(int argc, char **argv);
    ProgramOptions(const ProgramOptions& orig);
    virtual ~ProgramOptions();
    int getStatus();

    bool drawgraph, showuniques, showresults, autoopen, showorbits, skiporbits, invertspectrum, quiet, profile;
    int max_iterations, gx, gy, colourizer, max_colours, generator, number_hue, number_lightness, width, height;
    double max_re, max_im, min_re, min_im, cutoff, cr, ci, dprx, dpix;
    double spectral_min, spectral_max, lightness_min, lightness_max, colour_weighting, low_escape, high_escape;
    std::string output_filename, fileformat, convergecolour;

private:
    int _status;

};


}

#endif

