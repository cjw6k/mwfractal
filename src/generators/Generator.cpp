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

#include "Generator.hpp"

using namespace std;
using namespace JS;

Generator::Generator(const boost::shared_ptr<ProgramOptions> &opts) :
		_opts(opts),
		_px(opts->width),
		_py(opts->height),
		_ppx(),
		_ppy(),
		_idx(),
		_temp(),
		_row(),
		_col(),
		_progress(),
		_acc_iterations(),
		_current_iteration(),
		_current_point(),
		_zabs(),
		_iterate_fraction(),
		_argument()
	{

    if(this->_px == 0){
		this->_px = 1;
    }
    if(this->_py == 0){
		this->_py = 1;
	}
    this->_dp_re = complex<float>(static_cast<float>(opts->max_re - opts->min_re) / static_cast<float>(opts->width), 0);
    this->_dp_im = complex<float>(0, static_cast<float>(opts->max_im - opts->min_im) / static_cast<float>(opts->height));
    this->_p = complex<float>(static_cast<float>(opts->min_re) + this->_dp_re.real() / 2, static_cast<float>(opts->max_im) - this->_dp_im.imag() / 2);

    this->_total_points = this->_px * this->_py;

    this->_progress_diff = static_cast<float>(this->_total_points) / 80;
    this->_ln_2 = log(2.0);
    this->_ln_cutoff = log(this->_opts->cutoff);

    this->results.resize(this->_py);
    for(int idy = 0; idy < this->_py; idy++){
            this->results[idy].reserve(this->_px);
    }
    if(!this->_opts->skiporbits){
        this->orbits.resize(this->_py);
        for(int idy = 0; idy < this->_py; idy++){
            this->orbits[idy].resize(this->_px);
            for(int idx = 0; idx < this->_px; idx++){
                this->orbits[idy][idx].reserve(this->_opts->max_iterations);
            }
        }
    }
}

Generator::~Generator(){
}

bool Generator::run(){
	this->_preLoop(); // __PRE_LOOP__
//    while( this->_p.imag() > this->_opts->min_im ) {
      while(this->_ppy < this->_opts->height){
        this->_preRow(); // __PRE_ROW__
//        while( this->_p.real() < this->_opts->max_re ) {
          while(this->_ppx < this->_opts->width ) {
            this->_preColumn(); // __PRE_COLUMN__
            this->_preOrbit(); // __PRE_ORBIT__
            for(this->_idx = 0; this->_idx < this->_opts->max_iterations; this->_idx++){
                this->_preIterate(); // __PRE_ITERATE__
                this->_iterate(); // __ITERATE__
                this->_postIterate(); // __POST_ITERATE__
                if(this->_bailoutTest()){
                    this->_bailout(); // __BAILOUT__
                    break;
                }
            }
            this->_postOrbit(); // __POST_ORBIT__
            this->_postColumn(); // __POST_COLUMN__
        }
        this->_postRow(); // __POST_ROW__
    }
    this->_postLoop(); // __POST_LOOP__

    return true;
}

void Generator::_preLoop(){
} // Empty

void Generator::_preColumn(){
} // Empty

void Generator::_preRow(){
} // Empty

void Generator::_preOrbit(){
    if(!this->_opts->skiporbits){
        this->orbits[this->_row][this->_col].push_back(this->_z);
    }
}

void Generator::_preIterate(){
} // Empty

void Generator::_iterate(){
    this->_z = std::complex<float>(0, 0);
}

void Generator::_postIterate(){
	if(!isfinite(abs(this->_z))){
		this->_zabs = 10000;
	} else {
		this->_zabs = abs(this->_z);
	}
    if(!this->_opts->skiporbits){
        this->orbits[this->_row][this->_col].push_back(this->_z);
    }
}

bool Generator::_bailoutTest(){
    return this->_zabs > this->_opts->cutoff;
}

void Generator::_bailout(){
    this->_uniques[this->_idx + 1]++;
}

void Generator::_postOrbit(){
    if(this->_idx == this->_opts->max_iterations){
        this->_uniques[0]++;
    }
    if(this->_bailoutTest()){
        this->_iterate_fraction = log(this->_ln_cutoff / log(this->_zabs)) / this->_ln_2;
//        this->_iterate_fraction = ( log( this->_zabs ) - this->_ln_cutoff ) / ( this->_opts->cutoff - this->_ln_cutoff );
        this->_argument = this->_idx + 1 + this->_iterate_fraction;
        this->results[this->_row].push_back(static_cast<float>(this->_argument));
    } else {
        this->results[this->_row].push_back(-1);
    }
}

void Generator::_postRow(){
    this->_p -= this->_dp_im;
    this->_p = complex<float>(static_cast<float>(this->_opts->min_re), this->_p.imag());
    this->_temp = floor(this->_current_point / this->_progress_diff);

	this->_ppy++;
	this->_ppx = 0;

    if(this->_temp > this->_progress){
        while(this->_progress < this->_temp){
            this->_progress++;
            if(!this->_opts->quiet){
				cout << ".";
			}
        }
        if(!this->_opts->quiet){
			cout.flush();
		}
    }

    this->_row++;
    this->_col = 0;
}

void Generator::_postColumn(){
    this->_current_iteration += static_cast<float>(this->_opts->max_iterations);
    this->_acc_iterations += static_cast<float>(this->_idx) + 1;

    this->_p += this->_dp_re;
    this->_current_point++;

    this->_col++;
	this->_ppx++;
}

void Generator::_postLoop(){
    if(!this->_opts->quiet){
		cout << endl << endl << "Completed " << this->_acc_iterations << " of " << this->_current_iteration << " allocated iterations" << endl;
	}

    if(this->_opts->showuniques){
        cout << "Unique escape times with number of occurrences:" << endl;

        float pc = 0;
        for(this->_uniq_itr = this->_uniques.begin(); this->_uniq_itr != this->_uniques.end(); ++this->_uniq_itr){
            pc = static_cast<float>(floor(((*this->_uniq_itr).second / this->_total_points) * 100000)) / 1000;
            cout << endl << "  " << (*this->_uniq_itr).first << ":\t" << (*this->_uniq_itr).second << "\t" << pc << "%" << endl;
        }
    }

    if(this->_opts->showorbits){
        cout << endl << "Orbits of each point:" << endl;
        for(vector<vector<vector<complex<float> > > >::iterator row_itr = this->orbits.begin(); row_itr != this->orbits.end(); ++row_itr){
            for(vector<vector<complex<float> > >::iterator col_itr = (*row_itr).begin(); col_itr != (*row_itr).end(); ++col_itr){
                for(vector<complex<float> >::iterator orbit_itr = (*col_itr).begin(); orbit_itr != (*col_itr).end(); ++orbit_itr){
                    cout << "[" << row_itr - this->orbits.begin() << "," << col_itr - (*row_itr).begin() << "]-orbit[" << orbit_itr - (*col_itr).begin() << "]: " << (*orbit_itr) << endl;
                }
                cout << endl;
            }
        }
    }

    if(this->_opts->showresults){
        cout << endl << "Results for each point:" << endl << endl;
        for(vector<vector<float> >::iterator row_itr = this->results.begin(); row_itr != this->results.end(); ++row_itr){
            for(vector<float>::iterator col_itr = (*row_itr).begin(); col_itr != (*row_itr).end(); ++col_itr){
                    cout << "[" << row_itr - this->results.begin() << "," << col_itr - (*row_itr).begin() << "]: " << (*col_itr) << endl;
            }
        }
        cout << endl;
    }
}
