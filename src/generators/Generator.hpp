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

#ifndef GENERATOR_HPP
#define	GENERATOR_HPP

#include <boost/shared_ptr.hpp>

#include <complex>
#include <vector>
#include <map>
#include <iostream>

#include "../ProgramOptions.hpp"

namespace JS{

class Generator{

public:
    Generator(const boost::shared_ptr<ProgramOptions> &opts);
    virtual ~Generator();
    virtual bool run();
    std::vector<std::vector<float> > results;
    std::vector<std::vector<std::vector<std::complex<float> > > > orbits;

private:

protected:
    virtual void _iterate();
    virtual void _preLoop();
    virtual void _postLoop();
    virtual void _preColumn();
    virtual void _postColumn();
    virtual void _preRow();
    virtual void _postRow();
    virtual void _preOrbit();
    virtual void _postOrbit();
    virtual void _preIterate();
    virtual void _postIterate();
    virtual bool _bailoutTest();
    virtual void _bailout();

    std::complex<float> _c, _z, _p;
    std::complex<float> _dp_re, _dp_im;
    std::map<int, double> _uniques;
    std::map<int, double>::iterator _uniq_itr;

    boost::shared_ptr<ProgramOptions> _opts;

    int _px, _py, _ppx, _ppy, _total_points, _idx, _temp, _row, _col, _progress;
    float _acc_iterations, _current_iteration, _current_point, _progress_diff, _zabs;
    double _ln_2, _ln_cutoff, _iterate_fraction, _argument;

};


}

#endif
