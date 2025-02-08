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

#ifndef MW_DARTS_VIOLET_TO_RED_HPP
#define	MW_DARTS_VIOLET_TO_RED_HPP

#include "Colourizer.hpp"

namespace JS{

class MW_Darts_Violet_to_Red : public Colourizer{

public:
    MW_Darts_Violet_to_Red(const boost::shared_ptr<ProgramOptions> &opts);
    bool generatePalette();
    void generateScores();
	bool run();
	int game() const;
	static int getScore(std::complex<float> shot);

private:
    int _lo_score, _hi_score;
    float _ln_lo_score, _ln_hi_score, _ln_score_diff, _ln_pixel_score;
    std::vector<int> _gamescores;

};

}

#endif
