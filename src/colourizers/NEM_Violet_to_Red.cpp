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

#include "NEM_Violet_to_Red.hpp"

using namespace JS;
using namespace Magick;
using namespace std;

NEM_Violet_to_Red::NEM_Violet_to_Red(const boost::shared_ptr<ProgramOptions> &opts)
 : Colourizer(opts){
}

NEM_Violet_to_Red::NEM_Violet_to_Red(const NEM_Violet_to_Red& orig)
 : Colourizer(orig){
}

NEM_Violet_to_Red::~NEM_Violet_to_Red(){
}

bool NEM_Violet_to_Red::generatePalette(){
    this->_s = 1.0;
    this->_l = 0.5;
    this->_chroma = (1.0 - fabs(2.0 * this->_l - 1.0)) * this->_s;

	this->_palette.reserve(this->_opts->number_hue);

    for(this->_idx = 0; this->_idx < this->_opts->number_hue; this->_idx++){
        if(this->_opts->colour_weighting == 0.0){
            this->_h = this->_spectral_diff * (this->_idx + 0.5) / this->_opts->number_hue + this->_opts->spectral_min;
        } else if(this->_opts->colour_weighting > 0.0){
            this->_h = this->_spectral_diff * atan((this->_idx + 0.5) * this->_arctan_horiz_scaler) / this->_arctan_vert_scaler + this->_opts->spectral_min;
        } else {
            this->_h = this->_spectral_diff * (1.0 - atan((this->_opts->number_hue - (this->_idx + 0.5)) * this->_arctan_horiz_scaler) / this->_arctan_vert_scaler) + this->_opts->spectral_min;
        }

        this->_x = this->_chroma * (1.0 - fabs(fmod(this->_h, 2.0) - 1.0));

        if(this->_h < 1.0){
            this->_r = this->_chroma;
            this->_g = this->_x;
            this->_b = 0;
        } else if(this->_h < 2.0){
            this->_r = this->_x;
            this->_g = this->_chroma;
            this->_b = 0;
        } else if(this->_h < 3.0){
            this->_r = 0;
            this->_g = this->_chroma;
            this->_b = this->_x;
        } else if(this->_h < 4.0){
            this->_r = 0;
            this->_g = this->_x;
            this->_b = this->_chroma;
        } else if(this->_h < 5.0){
            this->_r = this->_x;
            this->_g = 0;
            this->_b = this->_chroma;
        } else {
            this->_r = this->_chroma;
            this->_g = 0;
            this->_b = this->_x;
        }

        this->_m = this->_l - this->_chroma / 2.0;
        this->_r = this->_r + this->_m;
        this->_g = this->_g + this->_m;
        this->_b = this->_b + this->_m;

        this->_palette.push_back(ColorRGB(this->_r, this->_g, this->_b));
		this->paletteProgressTick(this->_idx);
    }

    return true;
}
