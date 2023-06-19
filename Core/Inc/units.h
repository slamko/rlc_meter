/*
 * units.h
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#ifndef INC_UNITS_H_
#define INC_UNITS_H_

#define E 4096

template <class Data>
struct Unit {
	Unit() : val{} {}

protected:
	Data val;
	explicit Unit(Data val);
};

struct Res : public Unit<unsigned long long> {
	using Unit::Unit;

	unsigned long long get_kohm() {
		return this->val / 1000;
	}

	unsigned long long get_ohm() {
		return this->val;
	}

	static Res kohm(unsigned long long);
};

struct Capa : public Unit<long double>{
	using Unit::Unit;

	Capa operator +(Capa unit) {
		return Capa(this->val + unit.val);
	}

	Capa operator /(int unit) {
		return Capa(this->val / unit);
	}

	Capa& operator =(const Capa& unit) {
		this->val = unit.val;
		return *this;
	}

	long double get_uf() {
		return val / (1000 * 1000);
	}

	long double get_nf() {
		return val / (1000);
	}

	long double get_pf() {
		return val;
	}

	long double get_auto();
	const char *get_auto_unit();

	static Capa f(long double);
	static Capa uf(long double);
	static Capa nf(long double);
	static Capa pf(long double);
};

struct Self : public Unit<long double>{
	using Unit::Unit;

	long double get_mh() {
		return val / 1000;
	}

	long double get_uh() {
		return val;
	}

	long double get_nh() {
		return val * 1000;
	}

	static Self h(long double);
	static Self mh(long double);
	static Self uh(long double);
	static Self nh(long double);
};

Res operator ""_Ohm(unsigned long long val);
Res operator""_kOhm(unsigned long long int val);

Capa operator ""_uF(long double val);
Capa operator ""_nF(long double val);
Capa operator ""_pF(long double val);

Self operator ""_mH(long double val);
Self operator ""_uH(long double val);
Self operator ""_nH(long double val);

#endif /* INC_UNITS_H_ */
