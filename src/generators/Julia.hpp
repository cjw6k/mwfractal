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

#ifndef JULIA_HPP
#define	JULIA_HPP

#include "Generator.hpp"

#include <iostream>

#include "../ProgramOptions.hpp"

namespace JS{

class Julia : public Generator{

public:
    explicit Julia(const boost::shared_ptr<ProgramOptions> &opts);
    Julia(const Julia& orig);
    ~Julia() override;

private:

protected:
    void _iterate() override;
    void _preLoop() override;
    void _preOrbit() override;
    

};


}
#endif

