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

#include "MW_Darts_Violet_to_Red.hpp"

using namespace JS;
using namespace Magick;
using namespace std;

MW_Darts_Violet_to_Red::MW_Darts_Violet_to_Red(  boost::shared_ptr<ProgramOptions> opts )
 : Colourizer::Colourizer( opts ) {
	srand( time( NULL ) );
} 

MW_Darts_Violet_to_Red::MW_Darts_Violet_to_Red( const MW_Darts_Violet_to_Red& orig )
 : Colourizer::Colourizer( orig ) {
}

MW_Darts_Violet_to_Red::~MW_Darts_Violet_to_Red() {
}

bool MW_Darts_Violet_to_Red::generatePalette() {
    this->_palette_progress_diff = (float)this->_opts->number_lightness / 80;
    this->_s = 1.0;

	this->_palette.reserve( this->_opts->number_lightness * this->_opts->number_hue );

    for( this->_idy = 0; this->_idy < this->_opts->number_lightness; this->_idy++ ) {
        this->_l = this->_lightness_diff / this->_opts->number_lightness * ( this->_idy + 0.5 ) + this->_opts->lightness_min;
        this->_chroma = ( 1.0 - fabs( 2.0 * this->_l - 1.0 ) ) * this->_s;

        for( this->_idx = 0; this->_idx < this->_opts->number_hue; this->_idx++ ) {
            if( this->_opts->colour_weighting == 0.0 ) {
                this->_h = this->_spectral_diff * ( this->_idx + 0.5 ) / this->_opts->number_hue + this->_opts->spectral_min;
            } else if( this->_opts->colour_weighting > 0.0 ) {
                this->_h = this->_spectral_diff * atan( ( this->_idx + 0.5 ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler + this->_opts->spectral_min;
            } else {
                this->_h = this->_spectral_diff * ( 1.0 - atan( ( this->_opts->number_hue - ( this->_idx + 0.5 ) ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler ) + this->_opts->spectral_min;
            }

            this->_x = this->_chroma * ( 1.0 - fabs( fmod( this->_h, 2.0 ) - 1.0 ) );

            if( this->_h < 1.0 ) {
                this->_r = this->_chroma;
                this->_g = this->_x;
                this->_b = 0;
            } else if( this->_h < 2.0 ) {
                this->_r = this->_x;
                this->_g = this->_chroma;
                this->_b = 0;
            } else if( this->_h < 3.0 ) {
                this->_r = 0;
                this->_g = this->_chroma;
                this->_b = this->_x;
            } else if( this->_h < 4.0 ) {
                this->_r = 0;
                this->_g = this->_x;
                this->_b = this->_chroma;
            } else if( this->_h < 5.0 ) {
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

            this->_palette.push_back( ColorRGB( this->_r, this->_g, this->_b ) );
        }
		this->paletteProgressTick( this->_idy );
    }

    return true;
}

bool MW_Darts_Violet_to_Red::run() {
    this->generateScores();

    int palette_size = this->_palette.size() - 1;

    PixelPacket *pixel_cache = this->_image.getPixels( 0, 0, this->_px, this->_py );
    PixelPacket *next_pixel = pixel_cache;

    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            if( (*this->results)[this->_idy][this->_idx] != -1 ) {
                this->_ln_pixel_score = log( this->_gamescores.at( this->_idy * this->_px + this->_idx ) + 1 );
                this->_frac_part = ( this->_ln_pixel_score - this->_ln_lo_score ) / this->_ln_score_diff;
                if( this->_opts->invertspectrum ) {
                    *next_pixel = this->_palette.at( palette_size - ( int )floor( ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler ) - this->_opts->number_hue * ( int )floor( this->_opts->number_lightness * this->_frac_part ) );
                } else {
					*next_pixel = this->_palette.at( ( int )floor( ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler ) + this->_opts->number_hue * ( int )floor( (this->_opts->number_lightness -1)  * this->_frac_part ) );
                }
            }
            *next_pixel++;
        }
		if(!this->_opts->quiet){
			this->_current_iteration += this->_px;
			this->_temp = floor( this->_current_iteration / this->_progress_diff );
			if( this->_temp > this->_progress ) {
				while( this->_progress < this->_temp ) {
					this->_progress++;
					cout << ".";
				}
				cout.flush();
			}
		}
    }
    this->_image.syncPixels();

	if(!this->_opts->quiet){
		cout << endl << endl << "Completed " << this->_total_iterations << " pixels" << endl;
	}
    cout.flush();
	
    return true;
}

void MW_Darts_Violet_to_Red::generateScores() {
    this->_gamescores.reserve( this->_px * this->_py );

    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            this->_gamescores.push_back( this->game() );
        }
    }

    std::vector<int>::iterator min_score, max_score;
    min_score = min_element( this->_gamescores.begin(), this->_gamescores.end() );
    max_score = max_element( this->_gamescores.begin(), this->_gamescores.end() );
    this->_lo_score = *min_score;
    this->_hi_score = *max_score;
    this->_ln_lo_score = log( this->_lo_score + 1 );
    this->_ln_hi_score = log( this->_hi_score + 1 );

    this->_ln_score_diff = this->_ln_hi_score - this->_ln_lo_score;
}

int MW_Darts_Violet_to_Red::game() {
    int score = 0;
    vector<complex<float> >::iterator orbit_itr;
    for( orbit_itr = this->orbits->at(this->_idy).at(this->_idx).begin(); orbit_itr != this->orbits->at(this->_idy).at(this->_idx).end(); orbit_itr++ ) {
        score += this->getScore( (*orbit_itr) );
    }

    return score;
}

int MW_Darts_Violet_to_Red::getScore( std::complex<float> shot ) {
//  The radii of a dart board are as follows:
//  (Presume the bullseye radius is given by "bullseye")
//  -bullsring = 2.5 * bullseye
//  -inner treble ring = 15 * bullseye
//  -outer treble ring = 16.5 * bullseye
//  -inner double ring = 25 * bullseye
//  -outer double ring = 26.5 * bullseye

    float bullseye = 0.5;
    if( abs( shot ) > 26.5 * bullseye ) return 0;
    if( abs( shot ) < bullseye ) return 50;
    if( abs( shot ) < 2.5 * bullseye ) return 25;

    int phase_index = ( int )ceil( arg( shot ) * 20 / M_PI );
    int score = 0;

    switch( phase_index ) {
        case -19:
            score = 11;
            break;
        case -18:
        case -17:
            score = 8;
            break;
        case -16:
        case -15:
            score = 16;
            break;
        case -14:
        case -13:
            score = 7;
            break;
        case -12:
        case -11:
            score = 19;
            break;
        case -10:
        case -9:
            score = 3;
            break;
        case -8:
        case -7:
            score = 17;
            break;
        case -6:
        case -5:
            score = 2;
            break;
        case -4:
        case -3:
            score = 15;
            break;
        case -2:
        case -1:
            score = 10;
            break;
        case 0:
        case 1:
            score = 6;
            break;
        case 2:
        case 3:
            score = 13;
            break;
        case 4:
        case 5:
            score = 4;
            break;
        case 6:
        case 7:
            score = 18;
            break;
        case 8:
        case 9:
            score = 1;
            break;
        case 10:
        case 11:
            score = 20;
            break;
        case 12:
        case 13:
            score = 5;
            break;
        case 14:
        case 15:
            score = 12;
            break;
        case 16:
        case 17:
            score = 9;
            break;
        case 18:
        case 19:
            score = 14;
            break;
        default:
            score = 11;
    }

    if( abs( shot ) > 25 * bullseye ) return score * 2;
    if( ( abs( shot ) > 15 * bullseye ) && ( abs( shot ) < 16.5 * bullseye ) ) return score * 3;

    return score;
}
