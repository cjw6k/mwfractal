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

#ifndef COLOURIZER_HPP
#define	COLOURIZER_HPP

#include <boost/shared_ptr.hpp>
#include <ImageMagick-6/Magick++.h>

#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#include "../ProgramOptions.hpp"

namespace JS{

class Colourizer{

public:
    explicit Colourizer(boost::shared_ptr<ProgramOptions> opts);
    virtual ~Colourizer();
    virtual bool generatePalette();
	bool paletteProgressTick(int current);
    virtual bool run();
    void setResults(std::vector<std::vector<float> >* results);
    void setOrbits(std::vector<std::vector<std::vector<std::complex<float> > > >* orbits);
    void writeImage(const char* filename);
	void profile() const;
	
    std::vector<std::vector<float> >* results;
    std::vector<std::vector<std::vector<std::complex<float> > > >* orbits;

protected:
    boost::shared_ptr<ProgramOptions> _opts;

    Magick::Image _image;

    std::vector<Magick::ColorRGB> _palette;

    int _px, _py, _idy, _idx, _total_iterations, _current_iteration, _temp, _progress, _palette_progress;
    float _progress_diff, _palette_progress_diff, _lo_iteration, _hi_iteration;
    double _r, _g, _b, _h, _s, _l, _chroma, _x, _m;
    double _frac_part, _ones_digit, _spectral_diff, _lightness_diff;
    double _colour_scaler, _arctan_horiz_scaler, _arctan_vert_scaler;

};


}

#endif
