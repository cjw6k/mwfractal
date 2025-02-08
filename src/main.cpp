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

#include <boost/shared_ptr.hpp>

#include <cstdlib>
#include <iostream>

#include "ProgramOptions.hpp"
#include "generators/JuliaPowExp.hpp"
#include "generators/Julia3.hpp"
#include "generators/Julia.hpp"
#include "generators/Mandelbrot.hpp"
#include "colourizers/NEM_Violet_to_Red.hpp"
#include "colourizers/NEM_Violet_to_Red_Lightness.hpp"
#include "colourizers/NEM_White_to_Black.hpp"
#include "colourizers/MW_Darts_Violet_to_Red.hpp"

using namespace std;
using namespace JS;

void handleException(const std::exception& exception) {
  cout << "Exception: " << exception.what() << endl;
  exit(EXIT_FAILURE);
}

std::string generateFilename(const boost::shared_ptr<ProgramOptions>& opts) {
	std::ostringstream filename;
	if (opts->output_filename.empty()) {
		filename << "fractal-cl(" << opts->colourizer << ")_g(" << opts->generator << ")"
				 << "_c(" << opts->cr << "," << opts->ci << ")"
				 << "_dp(" << (opts->max_re - opts->min_re) / opts->width << ","
				 << (opts->max_im - opts->min_im) / opts->height << ")"
				 << "_i(" << opts->max_iterations << ")"
				 << "_s(" << (opts->max_re - opts->min_re) << ","
				 << (opts->max_im - opts->min_im) << ")"
				 << "_o(" << (opts->max_re + opts->min_re) / 2 << ","
				 << (opts->max_im + opts->min_im) / 2 << ")"
				 << "." << opts->fileformat;
	} else {
		filename << opts->output_filename;
	}

	return filename.str();
}

int main(int argc, char** argv){
    boost::shared_ptr<ProgramOptions> opts;
	try {
		opts.reset( new ProgramOptions(argc, argv));
	} catch (exception e){
        handleException(e);
	}
    if(opts->status != 0){
        if(opts->status > 1){
            exit(EXIT_FAILURE);
        } else {
            exit(EXIT_SUCCESS);
        }
    }

    boost::shared_ptr<Generator> generator;

    switch(opts->generator){
        case 1:
            generator.reset(new Julia(opts));
            break;

        case 2:
            generator.reset(new Mandelbrot(opts));
            break;

		case 3:
            generator.reset(new Julia3(opts));
            break;

		case 4:
            generator.reset(new JuliaPowExp(opts));
            break;

        default:
            cout << " (!) Invalid Generator '" << opts->generator << "'" << endl;
            exit(EXIT_FAILURE);
    }

	if(!opts->quiet){
	    cout << endl << "==== Phase 1 ====: Mathematical Modelling..." << endl;
	    cout << endl << "|0%------------------------------------|-50%-------------------------------100%|" << endl;
	}
	
    if(!generator->run()){
        cout << "x" << endl << endl << " (!) Generator run failed." << endl;
        exit(EXIT_FAILURE);
    }

    if(!opts->quiet){
		cout << endl;
	}
	
    boost::shared_ptr<Colourizer> colourizer;

    switch(opts->colourizer){
        case 1:
            colourizer.reset(new NEM_Violet_to_Red(opts));
            break;

        case 2:
            colourizer.reset(new NEM_Violet_to_Red_Lightness(opts));
            break;

        case 3:
            colourizer.reset(new NEM_White_to_Black(opts));
            break;

        case 4:
            colourizer.reset(new MW_Darts_Violet_to_Red(opts));
            break;

        default:
            cout << " (!) Invalid Colourizer '" << opts->colourizer << "'" << endl;
            exit(EXIT_FAILURE);
    }

    colourizer->setResults(&generator->results);
    colourizer->setOrbits(&generator->orbits);

	if(opts->profile){
		colourizer->profile();
		exit(EXIT_SUCCESS);
	}	
	
	if(!opts->quiet){
	    cout << endl << "==== Phase 2 ====: Palette Selection..." << endl;
	    cout << endl << "|0%------------------------------------|-50%-------------------------------100%|" << endl;
	}
    if(!colourizer->generatePalette()){
        cout << "x" << endl << endl << " (!) Palette generation failed." << endl;
        exit( EXIT_FAILURE );
    }

	if(!opts->quiet){
		cout << endl;
	}

	if(!opts->quiet){
	    cout << endl << "==== Phase 3 ====: Colouring..." << endl;
	    cout << endl << "|0%------------------------------------|-50%-------------------------------100%|" << endl;
	}
    if(!colourizer->run()){
        cout << "x" << endl << endl << " (!) Colouring run failed." << endl;
        exit(EXIT_FAILURE);
    }

    if(!opts->quiet){
		cout << endl;
	}

	if(!opts->quiet){
	    cout << endl << "Writing File '";
	}
    try {
        const string filename = generateFilename(opts);

		if(!opts->quiet){
	        cout << filename << "' ... ";
	        cout.flush();
		}

        colourizer->writeImage(filename.c_str());
        if(!opts->quiet){
			cout << "Done!" << endl << endl;
		}

        if(opts->autoopen){
            ostringstream cmd;
            cmd << "xdg-open \"" << filename << "\" 2>/dev/null &";

			if(system(cmd.str().c_str()) != 0){
				exit(EXIT_FAILURE);
			}
        }
    } catch(exception e){
        handleException(e);
	}

    exit(EXIT_SUCCESS);
}
