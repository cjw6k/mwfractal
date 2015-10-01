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

#ifndef NEM_VIOLET_TO_RED_LIGHTNESS_HPP
#define	NEM_VIOLET_TO_RED_LIGHTNESS_HPP

#include "Colourizer.hpp"

namespace JS{

class NEM_Violet_to_Red_Lightness : public Colourizer{

public:
    NEM_Violet_to_Red_Lightness(boost::shared_ptr<ProgramOptions> opts);
    NEM_Violet_to_Red_Lightness(const NEM_Violet_to_Red_Lightness& orig);
    virtual ~NEM_Violet_to_Red_Lightness();
    bool generatePalette();
	bool run();

private:

};

}

#endif
