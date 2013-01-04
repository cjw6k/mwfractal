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

#include "NEM_Violet_to_Red_Lightness.hpp"
#include "Colourizer.hpp"

using namespace JS;
using namespace Magick;
using namespace std;

NEM_Violet_to_Red_Lightness::NEM_Violet_to_Red_Lightness(  boost::shared_ptr<ProgramOptions> opts )
 : Colourizer::Colourizer( opts ) {
}

NEM_Violet_to_Red_Lightness::NEM_Violet_to_Red_Lightness( const NEM_Violet_to_Red_Lightness& orig )
 : Colourizer::Colourizer( orig ) {
}

NEM_Violet_to_Red_Lightness::~NEM_Violet_to_Red_Lightness() {
}

bool NEM_Violet_to_Red_Lightness::generatePalette() {
    for( this->_idy = 1; this->_idy <= this->_opts->number_lightness; this->_idy++ ) {
        for( this->_idx = 1; this->_idx <= this->_opts->number_hue; this->_idx++ ) {
            if( this->_opts->colour_weighting == 0.0 ) {
                this->_h = this->_spectral_diff * this->_idx / this->_opts->number_hue + this->_opts->spectral_min;
            } else if( this->_opts->colour_weighting > 0.0 ) {
                this->_h = this->_spectral_diff * atan( this->_idx * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler + this->_opts->spectral_min;
            } else {
                this->_h = this->_spectral_diff * ( 1.0 - atan( ( this->_opts->number_hue - this->_idx ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler ) + this->_opts->spectral_min;
            }

            this->_s = 1.0;
            this->_l = this->_lightness_diff / ( this->_opts->number_lightness - 1 ) * ( this->_idy - 1 ) + this->_opts->lightness_min;

            this->_chroma = ( 1.0 - fabs( 2.0 * this->_l - 1.0 ) ) * this->_s;
            this->_x = this->_chroma * ( 1.0 - fabs( fmod( this->_h, 2.0 ) - 1.0 ) );

            if( ( 0.0 <= this->_h ) && ( this->_h < 1.0 ) ) {
                this->_r = this->_chroma;
                this->_g = this->_x;
                this->_b = 0;
            } else if( ( 1.0 <= this->_h ) && ( this->_h < 2.0 ) ) {
                this->_r = this->_x;
                this->_g = this->_chroma;
                this->_b = 0;
            } else if( ( 2.0 <= this->_h ) && ( this->_h < 3.0 ) ) {
                this->_r = 0;
                this->_g = this->_chroma;
                this->_b = this->_x;
            } else if( ( 3.0 <= this->_h ) && ( this->_h < 4.0 ) ) {
                this->_r = 0;
                this->_g = this->_x;
                this->_b = this->_chroma;
            } else {
                this->_r = this->_x;
                this->_g = 0;
                this->_b = this->_chroma;
            }

            this->_m = this->_l - this->_chroma / 2.0;
            this->_r = this->_r + this->_m;
            this->_g = this->_g + this->_m;
            this->_b = this->_b + this->_m;

            this->_palette[this->_idx + ( this->_idy - 1 ) * this->_opts->number_hue] = ColorRGB( this->_r, this->_g, this->_b );
        }
    }

    return true;
}