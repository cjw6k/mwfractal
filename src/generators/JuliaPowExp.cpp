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

#include "JuliaPowExp.hpp"

using namespace std;
using namespace JS;

JuliaPowExp::JuliaPowExp(boost::shared_ptr<ProgramOptions> opts)
 : Julia::Julia(opts){
}

JuliaPowExp::JuliaPowExp(const JuliaPowExp& orig)
 : Julia::Julia(orig){
}

JuliaPowExp::~JuliaPowExp(){
}

void JuliaPowExp::_iterate(){
    this->_z = pow(this->_z, this->_opts->g4_power) * exp(pow(this->_z, this->_opts->g4_exponent)) + (std::complex<double>)this->_c;
}

void JuliaPowExp::_postOrbit(){
    if(this->_idx == this->_opts->max_iterations){
        this->_uniques[0]++;
    }
    if(this->_bailoutTest()){
        //this->_iterate_fraction = log(this->_ln_cutoff / log(this->_zabs)) / this->_ln_2;
		this->_iterate_fraction = (log(this->_zabs) - this->_ln_cutoff) / ((this->_opts->g4_power - 1) * this->_ln_cutoff + pow(this->_opts->cutoff, this->_opts->g4_exponent));
        this->_argument = this->_idx + 1 + this->_iterate_fraction;
        this->results[this->_row].push_back(this->_argument);
    } else {
        this->results[this->_row].push_back(-1);
    }
}