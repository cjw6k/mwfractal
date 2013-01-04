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

namespace bpo = boost::program_options;
using namespace JS;
using namespace std;

ProgramOptions::ProgramOptions( int argc, char **argv ) {
	this->showuniques = this->drawgraph = this->autoopen = false;

    bpo::options_description generic( "Generic Options" );
    generic.add_options()
        ( "help,h", "print help and exit" )
        ( "version,v", "print version information and exit" );

    bpo::options_description model( "Mathematical Model Options" );
    model.add_options()
        ( "generator", bpo::value<int>(&this->generator)->default_value(1), "Generator model - 0: Mandelbrot, 1: Julia" )
        ( "iterations", bpo::value<int>(&this->max_iterations)->default_value(32), "Number of iterations per pixel" )
        ( "c_real", bpo::value<float>(&this->cr)->default_value(-0.835), "Real part of c" )
        ( "c_imag", bpo::value<float>(&this->ci)->default_value(-0.2321), "Imaginary part of c" )
        ( "dp_real", bpo::value<float>(&this->dprx)->default_value(4), "" )
        ( "dp_imag", bpo::value<float>(&this->dpix)->default_value(4), "" )
        ( "min_real", bpo::value<float>(&this->min_re)->default_value(-2), "" )
        ( "max_real", bpo::value<float>(&this->max_re)->default_value(2), "" )
        ( "min_imag", bpo::value<float>(&this->min_im)->default_value(-1.125), "" )
        ( "max_imag", bpo::value<float>(&this->max_im)->default_value(1.125), "" )
        ( "z_cutoff", bpo::value<float>(&this->cutoff)->default_value(4096), "" )
        ( "show_uniques", bpo::value<bool>(&this->showuniques)->zero_tokens() );

    bpo::options_description graphics( "Graphical Options" );
    graphics.add_options()
        ( "colourizer", bpo::value<int>(&this->colourizer)->default_value(1), "Colourizer (1 = violet(inner) to red(outer), 2 = violet(inner) to red(outer) w/ lightness, 3 = white(inner) to black(outer), 4 = black(inner) to white(outer))" )
        ( "number_hue", bpo::value<int>(&this->number_hue)->default_value(65536), "Number of (diverging) hue values" )
        ( "number_lightness", bpo::value<int>(&this->number_lightness)->default_value(100), "Number of (diverging) lightness values" )
        ( "spectral_minimum", bpo::value<float>(&this->spectral_min)->default_value(0.0), "Minimum spectral colour (red = 0.0, yellow = 1.0, green = 2.0, cyan = 3.0, royal = 4.0, violet = 5.0)" )
        ( "spectral_maximum", bpo::value<float>(&this->spectral_max)->default_value(5.0), "Maximum spectral colour (red = 0.0, yellow = 1.0, green = 2.0, cyan = 3.0, royal = 4.0, violet = 5.0)" )
        ( "lightness_minimum", bpo::value<float>(&this->lightness_min)->default_value(0.3), "Minimum lightness (0.0 = black, 1.0 = white)" )
        ( "lightness_maximum", bpo::value<float>(&this->lightness_max)->default_value(0.7), "Maximum lightness (0.0 = black, 1.0 = white)" )
        ( "colour_weighting", bpo::value<float>(&this->colour_weighting)->default_value(0.0), "Colour weighting is complicated...try a real floating pt number (+ or -) and see what happens" )
        ( "drawgraph", bpo::value<bool>(&this->drawgraph)->zero_tokens(), "Overlay a graph on the image" )
        ( "graphxplot", bpo::value<int>(&this->gx)->default_value(50), "Size in pixels of the x axis plot lines" )
        ( "graphyplot", bpo::value<int>(&this->gy)->default_value(50), "Size in pixels of the y axis plot lines" )
        ( "autoopen,a", bpo::value<bool>(&this->autoopen)->zero_tokens(), "Automatically open the generated image in the default viewer" );

    bpo::options_description hidden( "Hidden" );

    bpo::options_description cmdline;
    cmdline.add( generic ).add( model ).add( graphics ).add( hidden );

    bpo::options_description visible( "Available Options" );
    visible.add( generic ).add( model ).add( graphics );

    bpo::variables_map vm;
    try{
        bpo::store( bpo::command_line_parser( argc, argv ).options( cmdline ).run(), vm );
    } catch( exception &e ) {
        cout << "ERROR: " << e.what() << endl << "Use -h for help" << endl;
        this->_status = 2;
        return;
    }

    bpo::notify( vm );

    if( vm.count( "help" ) ) {
        cout << endl << visible << endl;
        this->_status = 1;
        return;
    }

    if( vm.count( "version" ) ) {
        cout << endl << "\tMoore & Willcock" << endl << "\tVersion: 1.0 + 0.1i" << endl;
        this->_status = 1;
        return;
    }
    this->_status = 0;

    return;
}

ProgramOptions::ProgramOptions( const ProgramOptions& orig ) {
}

ProgramOptions::~ProgramOptions() {
}

int ProgramOptions::getStatus() {
    return this->_status;
}