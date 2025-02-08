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

MW_Darts_Violet_to_Red::MW_Darts_Violet_to_Red(const boost::shared_ptr<ProgramOptions> &opts ) :
		Colourizer( opts ),
		_lo_score(),
		_hi_score(),
		_ln_lo_score(),
		_ln_hi_score(),
		_ln_score_diff(),
		_ln_pixel_score()
	{
	srand( time( NULL ) );
} 

bool MW_Darts_Violet_to_Red::generatePalette() {
    const size_t POINTS_PER_PROGRESS_TICK = 80;
    this->_palette_progress_diff = static_cast<float>(this->_opts->number_lightness) / POINTS_PER_PROGRESS_TICK;
    this->_s = 1.0;

	this->_palette.reserve( static_cast<unsigned long>(this->_opts->number_lightness) * this->_opts->number_hue );

    for( this->_idy = 0; this->_idy < this->_opts->number_lightness; this->_idy++ ) {
        const double HALF_PIXEL_OFFSET = 0.5;
        this->_l = this->_lightness_diff / this->_opts->number_lightness * ( this->_idy + HALF_PIXEL_OFFSET ) + this->_opts->lightness_min;
        this->_chroma = ( 1.0 - fabs( 2 * this->_l - 1.0 ) ) * this->_s;

        for( this->_idx = 0; this->_idx < this->_opts->number_hue; this->_idx++ ) {
            if( this->_opts->colour_weighting == 0.0 ) {
                this->_h = this->_spectral_diff * ( this->_idx + HALF_PIXEL_OFFSET ) / this->_opts->number_hue + this->_opts->spectral_min;
            } else if( this->_opts->colour_weighting > 0.0 ) {
                this->_h = this->_spectral_diff * atan( ( this->_idx + HALF_PIXEL_OFFSET ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler + this->_opts->spectral_min;
            } else {
                this->_h = this->_spectral_diff * ( 1.0 - atan( ( this->_opts->number_hue - ( this->_idx + HALF_PIXEL_OFFSET ) ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler ) + this->_opts->spectral_min;
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

    int palette_size = static_cast<int>(this->_palette.size()) - 1;

    PixelPacket *pixel_cache = this->_image.getPixels( 0, 0, this->_px, this->_py );
    PixelPacket *next_pixel = pixel_cache;

    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            if( (*this->results)[this->_idy][this->_idx] != -1 ) {
                this->_ln_pixel_score = static_cast<float>(log( this->_gamescores.at( this->_idy * this->_px + this->_idx ) + 1 ));
                this->_frac_part = ( this->_ln_pixel_score - this->_ln_lo_score ) / this->_ln_score_diff;
                if( this->_opts->invertspectrum ) {
                    *next_pixel = this->_palette.at( palette_size - static_cast<int>(floor(((*this->results)[this->_idy][this->_idx] - this->_lo_iteration) * this->_colour_scaler)) - this->_opts->number_hue * static_cast<int>(floor(this->_opts->number_lightness * this->_frac_part)) );
                } else {
					*next_pixel = this->_palette.at( static_cast<int>(floor(((*this->results)[this->_idy][this->_idx] - this->_lo_iteration) * this->_colour_scaler)) + this->_opts->number_hue * static_cast<int>(floor((this->_opts->number_lightness - 1) * this->_frac_part)) );
                }
            }
            next_pixel++; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }
		if(!this->_opts->quiet){
			this->_current_iteration += this->_px;
			this->_temp = floor( static_cast<float>(this->_current_iteration) / this->_progress_diff );
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
    this->_gamescores.reserve( static_cast<unsigned long>(this->_px) * this->_py );

    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            this->_gamescores.push_back( this->game() );
        }
    }

    const std::vector<int>::iterator min_score = min_element(this->_gamescores.begin(), this->_gamescores.end());
    const std::vector<int>::iterator max_score = max_element(this->_gamescores.begin(), this->_gamescores.end());
    this->_lo_score = *min_score;
    this->_hi_score = *max_score;
    this->_ln_lo_score = static_cast<float>(log( this->_lo_score + 1 ));
    this->_ln_hi_score = static_cast<float>(log( this->_hi_score + 1 ));

    this->_ln_score_diff = this->_ln_hi_score - this->_ln_lo_score;
}

int MW_Darts_Violet_to_Red::game() const {
    int score = 0;
    for( vector<complex<float> >::iterator orbit_itr = this->orbits->at(this->_idy).at(this->_idx).begin(); orbit_itr != this->orbits->at(this->_idy).at(this->_idx).end(); ++orbit_itr ) {
        score += this->getScore( (*orbit_itr) );
    }

    return score;
}

int MW_Darts_Violet_to_Red::getScore(const std::complex<float> shot ) {
//  The radii of a dart board are as follows:
//  (Presume the bullseye radius is given by "bullseye")
//  -bullsring = 2.5 * bullseye
//  -inner treble ring = 15 * bullseye
//  -outer treble ring = 16.5 * bullseye
//  -inner double ring = 25 * bullseye
//  -outer double ring = 26.5 * bullseye

    const float BULLS_RING_RADIUS_FACTOR = 2.5;
    const float INNER_TREBLE_RING_RADIUS_FACTOR = 15;
    const float OUTER_TREBLE_RING_RADIUS_FACTOR = 16.5;
    const float INNER_DOUBLE_RING_RADIUS_FACTOR = 25;
    const float OUTER_DOUBLE_RING_RADIUS_FACTOR = 26.5;
    const float BULLSEYE = 0.5;
    const int BULLSEYE_SCORE = 50;
    const int BULLRING_SCORE = 25;
    if( abs( shot ) > OUTER_DOUBLE_RING_RADIUS_FACTOR * BULLSEYE ) return 0;
    if( abs( shot ) < BULLSEYE ) return BULLSEYE_SCORE;
    if( abs( shot ) < BULLS_RING_RADIUS_FACTOR * BULLSEYE ) return BULLRING_SCORE;

    const double SEGMENT_ANGLE = M_PI / 10; // 18 degrees per segment
    const int SEGMENT_SCORES[20] = { 6, 13, 4, 18, 1, 20, 5, 12, 9, 14, 11, 8, 16, 7, 19, 3, 17, 2, 15, 10};

    const unsigned int segment_hit = static_cast<unsigned int>(floor((arg(shot) + M_PI) / SEGMENT_ANGLE)) % 20;

    const int score = SEGMENT_SCORES[segment_hit];

    if( abs( shot ) > INNER_DOUBLE_RING_RADIUS_FACTOR * BULLSEYE ) return score * 2;
    if( ( abs( shot ) > INNER_TREBLE_RING_RADIUS_FACTOR * BULLSEYE ) && ( abs( shot ) < OUTER_TREBLE_RING_RADIUS_FACTOR * BULLSEYE ) ) return score * 3;

    return score;
}
