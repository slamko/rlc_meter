/*
 * outils.cpp
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#include "units.h"

template <class Data>
Unit<Data>::Unit(Data val) : val(val) {}

Res Res::kohm(unsigned long long val) {
	return Res(val * 1000);
}

Capa Capa::f(long double val) {
	return Capa(val * 1000 * 1000 * 1000 * 1000);
}

Capa Capa::uf(long double val) {
	return Capa(val * 1000 * 1000);
}

Capa Capa::nf(long double val) {
	return Capa(val * 1000);
}

Capa Capa::pf(long double val) {
	return Capa(val);
}

Res operator ""_Ohm(unsigned long long val) {
	return Res::kohm(val);
}

Res operator ""_kOhm(unsigned long long val) {
	return Res::kohm(val * 1000);
}

Capa operator ""_uF(long double val) {
	return Capa::uf(val);
}

Capa operator ""_nF(long double val) {
	return Capa::nf(val);
}

Capa operator ""_pF(long double val) {
	return Capa::pf(val);
}

Self Self::h(long double val) {
	return Self(val * 1000 * 1000);
}

Self Self::mh(long double val) {
	return Self(val * 1000);
}

Self Self::uh(long double val) {
	return Self(val);
}

Self operator ""_mH(long double val) {
	return Self::mh(val);
}

Self operator ""_uH(long double val) {
	return Self::uh(val);
}
