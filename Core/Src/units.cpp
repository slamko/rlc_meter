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

Res Res::ohm(unsigned long long val) {
	return Res(val);
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
	return Res::ohm(val);
}

Res operator ""_kOhm(unsigned long long val) {
	return Res::kohm(val);
}

unsigned long long Res::get_auto() {
	if (val > 1000.0) {
		return get_kohm();
	} else if (val > 1.0) {
		return get_ohm();
	}
	return val;
}

const char *Res::get_auto_unit() {
	if (val > 1000000.0) {
		return "MOhm";
	} else if (val > 1000.0) {
		return "KOhm";
	} else if (val > 1.0) {
		return "Ohm";
	}
	return "nil";
}


long double Self::get_auto() {
	if (val > 1000000000.0) {
		return 0;
	} else if (val > 1000000.0) {
		return get_h();
	} else if (val > 1000.0) {
		return get_mh();
	} else if (val > 1.0) {
		return get_uh();
	}
	return val;
}

const char *Self::get_auto_unit() {
	if (val > 1000000.0) {
		return "H";
	} else if (val > 1000.0) {
		return "mH";
	} else if (val > 1.0) {
		return "uH";
	}
	return "nil";
}

long double Capa::get_auto() {
	if (val > 1000000.0) {
		return get_uf();
	} else if (val > 1000.0) {
		return get_nf();
	} else if (val > 1.0) {
		return get_pf();
	}
	return val;
}

const char *Capa::get_auto_unit() {
	if (val > 1000000.0) {
		return "uF";
	} else if (val > 1000.0) {
		return "nF";
	} else if (val > 1.0) {
		return "pF";
	}
	return "nil";
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
