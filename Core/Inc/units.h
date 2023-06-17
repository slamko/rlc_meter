/*
 * units.h
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#ifndef INC_UNITS_H_
#define INC_UNITS_H_

template <class Data>
struct Unit {
	Data val;
	explicit Unit(Data val);

	operator Data() {
		return val;
	}
};

struct Res : public Unit<unsigned long long> {
	using Unit::Unit;
};

struct Capa : public Unit<long double>{
	using Unit::Unit;
};

struct Self : public Unit<long double>{
	using Unit::Unit;
};

Res operator""_kOhm(unsigned long long int val);

Capa operator ""_uF(long double val);
Capa operator ""_nF(long double val);
Capa operator ""_pF(long double val);

Self operator ""_mH(long double val);
Self operator ""_uH(long double val);
Self operator ""_nH(long double val);

#endif /* INC_UNITS_H_ */
