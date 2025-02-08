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

#include "Julia.hpp"

using namespace std;
using namespace JS;

Julia::Julia(const boost::shared_ptr<ProgramOptions> &opts)
 : Generator(opts){
}

void Julia::_preLoop(){
    this->_c = complex<float>(static_cast<float>(this->_opts->cr), static_cast<float>(this->_opts->ci));
}

void Julia::_preOrbit(){
    this->_z = this->_p;
    Generator::_preOrbit();
}

void Julia::_iterate(){
    this->_z = this->_z * this->_z + this->_c;
}
