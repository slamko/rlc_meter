/*
 * outils.cpp
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#include "units.h"

template <class Data>
Unit<Data>::Unit(Data val) : val(val) {}

Res operator ""_kOhm(unsigned long long val) {
	return Res(val * 1000);
}

Capa operator ""_uF(long double val) {
	return Capa(val * 1000000);
}

Capa operator ""_nF(long double val) {
	return Capa(val * 1000);
}

Capa operator ""_pF(long double val) {
	return Capa(val);
}

Self operator ""_mH(long double val) {
	return Self(val * 1000);
}

Self operator ""_uH(long double val) {
	return Self(val);
}
