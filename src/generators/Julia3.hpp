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

#ifndef JULIA3_HPP
#define	JULIA3_HPP

#include "Julia.hpp"

#include <iostream>

#include "../ProgramOptions.hpp"

namespace JS{

class Julia3 : public Julia{

public:
    Julia3(boost::shared_ptr<ProgramOptions> opts);
    Julia3(const Julia3& orig);
    virtual ~Julia3();

private:

protected:
    void _iterate();    

};


}
#endif

