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

#include "ProgramOptions.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace bpo = boost::program_options;
using namespace JS;
using namespace std;

ProgramOptions::ProgramOptions(int argc, char **argv){
    this->profile = this->quiet = this->showresults = this->showorbits = this->skiporbits = this->showuniques = this->drawgraph = this->autoopen = false;

    bpo::options_description generic("Generic Options");
    generic.add_options()
        ("help,h", "print help and exit")
        ("version,v", "print version information and exit")
        ("quiet,q", bpo::value<bool>(&this->quiet)->zero_tokens() )
		("outputfilename,o", bpo::value<string>(&this->output_filename)->default_value(""), "output filename, implies -f")
		("fileformat,f", bpo::value<string>(&this->fileformat)->default_value("png"), "output file format (jpg, gif, png, bmp, tiff)")
        ("autoopen,a", bpo::value<bool>(&this->autoopen)->zero_tokens(), "Automatically open the generated image in the default viewer");

    bpo::options_description model("Mathematical Model Options");
    model.add_options()
        ("generator", bpo::value<int>(&this->generator)->default_value(1), "Generator model - 1: Julia, 2: Mandelbrot")
        ("iterations", bpo::value<int>(&this->max_iterations)->default_value(32), "Number of iterations per pixel")
        ("c_real", bpo::value<double>(&this->cr)->default_value(-0.835), "Real part of c")
        ("c_imag", bpo::value<double>(&this->ci)->default_value(-0.2321), "Imaginary part of c")
        ("min_real", bpo::value<double>(&this->min_re)->default_value(-2), "")
        ("max_real", bpo::value<double>(&this->max_re)->default_value(2), "")
        ("min_imag", bpo::value<double>(&this->min_im)->default_value(-1.125), "")
        ("max_imag", bpo::value<double>(&this->max_im)->default_value(1.125), "")
        ("z_cutoff", bpo::value<double>(&this->cutoff)->default_value(4096), "")
        ("show_uniques", bpo::value<bool>(&this->showuniques)->zero_tokens() )
        ("show_orbits", bpo::value<bool>(&this->showorbits)->zero_tokens() )
        ("skip_orbits", bpo::value<bool>(&this->skiporbits)->zero_tokens() )
        ("show_results", bpo::value<bool>(&this->showresults)->zero_tokens() );

    bpo::options_description graphics("Graphical Options");
    graphics.add_options()
        ("width", bpo::value<int>(&this->width)->default_value(320), "Output image width")
        ("height", bpo::value<int>(&this->height)->default_value(180), "Output image height")
        ("colourizer", bpo::value<int>(&this->colourizer)->default_value(1), "Colourizer (1 = violet(inner) to red(outer), 2 = violet(inner) to red(outer) w/ lightness, 3 = white(inner) to black(outer), 4 = Darts!)")
        ("number_hue", bpo::value<int>(&this->number_hue)->default_value(65536), "Number of (diverging) hue values")
        ("number_lightness", bpo::value<int>(&this->number_lightness)->default_value(100), "Number of (diverging) lightness values")
        ("spectral_minimum", bpo::value<double>(&this->spectral_min)->default_value(0.0), "Minimum spectral colour (red = 0.0, yellow = 1.0, green = 2.0, cyan = 3.0, royal = 4.0, violet = 5.0)")
        ("spectral_maximum", bpo::value<double>(&this->spectral_max)->default_value(5.0), "Maximum spectral colour (red = 0.0, yellow = 1.0, green = 2.0, cyan = 3.0, royal = 4.0, violet = 5.0)")
        ("lightness_minimum", bpo::value<double>(&this->lightness_min)->default_value(0.3), "Minimum lightness (0.0 = black, 1.0 = white)")
        ("lightness_maximum", bpo::value<double>(&this->lightness_max)->default_value(0.7), "Maximum lightness (0.0 = black, 1.0 = white)")
        ("colour_weighting", bpo::value<double>(&this->colour_weighting)->default_value(0.0), "Colour weighting is complicated...try a real floating pt number (+ or -) and see what happens")
        ("low_escape", bpo::value<double>(&this->low_escape)->default_value(-1), "Force the colourizer to use this value as the lowest value in the range of all escape values")
        ("high_escape", bpo::value<double>(&this->high_escape)->default_value(-1), "Force the colourizer to use this value as the highest value in the range of all escape values")
//        ("drawgraph", bpo::value<bool>(&this->drawgraph)->zero_tokens(), "Overlay a graph on the image")
//        ("graphxplot", bpo::value<int>(&this->gx)->default_value(50), "Size in pixels of the x axis plot lines")
//        ("graphyplot", bpo::value<int>(&this->gy)->default_value(50), "Size in pixels of the y axis plot lines")
        ("invertspectrum,i", bpo::value<bool>(&this->invertspectrum)->zero_tokens(), "Invert the colour spectrum of the colourizer")
        ("profile,p", bpo::value<bool>(&this->profile)->zero_tokens(), "Generate profiling information")
        ("convergecolour", bpo::value<std::string>(&this->convergecolour)->default_value("#000000"), "Colour converging pixels, hex triplet (default is black)");

    bpo::options_description hidden("Hidden");

    bpo::options_description cmdline;
    cmdline.add(generic).add(model).add(graphics).add(hidden);

    bpo::options_description visible("Available Options");
    visible.add(generic).add(model).add(graphics);

    bpo::variables_map vm;
    try{
        bpo::store(bpo::command_line_parser(argc, argv).options(cmdline).run(), vm);
    } catch(exception &e){
        cout << "ERROR: " << e.what() << endl << "Use -h for help" << endl;
        this->_status = 2;
        return;
    }

    bpo::notify(vm);

    if(vm.count("help")){
        cout << endl << visible << endl;
        this->_status = 1;
        return;
    }

    if(vm.count("version")){
        cout << endl << "\tMoore & Willcock" << endl << "\tVersion: 1.0 + 0.1i" << endl;
        this->_status = 1;
        return;
    }

    if(this->number_hue <= 0){
        cout << " (!) Number of hues '" << this->number_hue << "' must be a positive integer" << endl;
        this->_status = 2;
        return;
    }

    if(this->number_lightness <= 0){
        cout << " (!) Number of lightnesses '" << this->number_lightness << "' must be a positive integer" << endl;
        this->_status = 2;
        return;
    }

    if((this->spectral_min < 0.0) || (this->spectral_min > 6.0)){
        cout << " (!) The spectral minimum '" << this->spectral_min << "' must be bound by [0.0, 6.0]" << endl;
        this->_status = 2;
        return;
    }

    if((this->spectral_max < 0.0) || (this->spectral_max > 6.0)){
        cout << " (!) The spectral maximum '" << this->spectral_max << "' must be bound by [0.0, 6.0]" << endl;
        this->_status = 2;
        return;
    }

    if(this->spectral_min > this->spectral_max){
        cout << " (!) The spectral minimum '" << this->spectral_min << "' cannot exceed the spectral maximum '" << this->spectral_max << "'" << endl;
        this->_status = 2;
        return;
    }

    if((this->lightness_min < 0.0) || (this->lightness_min > 1.0)){
        cout << " (!) The lightness minimum '" << this->lightness_min << "' must be bound by [0.0, 1.0]" << endl;
        this->_status = 2;
        return;
    }

    if((this->lightness_max < 0.0) || (this->lightness_max > 1.0)){
        cout << " (!) The lightness maximum '" << this->lightness_max << "' must be bound by [0.0, 1.0]" << endl;
        this->_status = 2;
        return;
    }

    if(this->lightness_min > this->lightness_max){
        cout << " (!) The lightness minimum '" << this->lightness_min << "' cannot exceed the lightness maximum '" << this->lightness_max << "'" << endl;
        this->_status = 2;
        return;
    }

	if(this->profile){
		this->quiet = true;
	}
	
    this->_status = 0;

    return;
}

ProgramOptions::ProgramOptions(const ProgramOptions& orig){
}

ProgramOptions::~ProgramOptions(){
}

int ProgramOptions::getStatus(){
    return this->_status;
}
