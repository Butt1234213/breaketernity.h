#include "breaketernity.h"
#include <cmath>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <string>
#include <deque>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <limits>

const double EXP_LIMIT = 9e15;
//log10 9e15
const double LAYER_LIMIT = 15.95424250943;
const int MAX_SIGNIFICANT_DIGITS = 17;

//only use this shorthand if you are creating 0
static Decimal N() {
	return Decimal();
}
//only use this shorthand when you are sure a is and will not be a massive number
static Decimal I(Decimal::Mag a) {
	return Decimal(a);
}
//use this shorthand if you are returning an existing Decimal object
static Decimal D(Decimal a) {
	Decimal::Sign sign = a.sign;
	Decimal::Layer layer = a.layer;
	Decimal::Mag mag = a.mag;
	return Decimal(sign, layer, mag);
}
//use this shorthand if you are creating a new Decimal object
static Decimal FC(Decimal::Sign sign, Decimal::Layer layer, Decimal::Mag mag) {
	return Decimal(sign, layer, mag);
}

template<typename T>
bool Decimal::isZero(T a) {
	if (a == 0) {
		return true;
	} else {
		return false;
	}
}

template <typename T>
Decimal::Sign Decimal::toSign(T a) {
	if (Decimal::isZero(a)) {
		return 0;
	}
	if (std::signbit(a)) {
		return -1;
	} else {
		return 1;
	}
}

Decimal::Sign Decimal::signFromStr(std::string a) {
	char front = a.front();
	std::string zeroPoint = a.substr(0, 2);
	if (front == '-') {
		return -1;
	}
	if (front == '0' && zeroPoint != "0.") {
		return 0;
	}
	return 1;
}
Decimal::Layer Decimal::layerFromStr(std::string a) {
	std::string::difference_type e = std::count(a.begin(), a.end(), 'e');
	return static_cast<Layer>(e);
}
Decimal::Mag Decimal::magFromStr(std::string a) {
	//if the layer is 0
	char exp = 'e';
	if (a.find(exp) == std::string::npos) {
		return std::stod(a);
	}

	//splits up the string by the e's
	std::stringstream ss(a);
	std::string str;
	std::vector<std::string> strs;
	while (std::getline(ss, str, exp)) {
		strs.push_back(str);
	}
	
	//I am only gonna support a layer of 1 right now, but I think this code works for layers higher than 1
	//xey => new Decimal(1, 1, (y + std::log10(x)));
	//I don't know the math for xeyez, it might be new Decimal(1, 1, (z + std::log10(y) + std::log10(std::log10(x))));
	std::deque<double> values;
	for (int i = 0; i < strs.size(); i++) {
		std::string temp = strs[i];
		if (i == 0) {
			if (temp.front() == '-') {
				temp.erase(0, 1);
			}
		}
		if (!temp.empty()) {
			values.push_front(std::stod(temp));
		}
	}
	int queSize = static_cast<int>(values.size());
	for (int i = queSize; i > 0; i--) {
		double val = values[i - 1];
		for (int j = 0; j < (i - 1); j++) {
			if (j < 2) {
				val = std::log10(val);
			} else {
				val = 0;
			}
		}
		values[i - 1] = val;
	}
	Mag totalMag = 0;
	for (int i = 0; i < queSize; i++) {
		totalMag = totalMag + values[i];
	}
	return totalMag;
}


template<typename T>
Decimal::Layer Decimal::layerChecker(T a, Layer b) {
	if (isZero(std::abs(a))) {
		return 0;
	}
	double x = std::abs(a);
	if (!std::is_integral_v<double>) {
		double x = static_cast<double>(std::abs(a));
	}
	double y = std::log10(x);

	if (std::abs(y) > LAYER_LIMIT) {
		return b + 1;
	} else {
		return b;
	}
}

template<typename T>
Decimal::Mag Decimal::setMagBasedOnLayer(T a, Layer b, Sign c) {
	if (b > 0) {
		if (c == 0) {
			return std::nan("");
		}
		return std::log10(a);
	} else {
		return a;
	}
}

Decimal* Decimal::toPtr(Decimal a) {
	return &a;
}
Decimal Decimal::fromPtr(Decimal* a) {
	return *a;
}

int Decimal::toInt(Decimal a) {
	constexpr int lowerLimit = std::numeric_limits<int>::lowest();
	constexpr int upperLimit = std::numeric_limits<int>::max();
	double val;

	if (a.layer == 0) {
		val = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? upperLimit : lowerLimit : 0;
	}
	if (val > upperLimit) {
		return upperLimit;
	} else if (val < lowerLimit) {
		return lowerLimit;
	} else {
		return static_cast<long long>(val);
	}
}
long long Decimal::toLongLong(Decimal a) {
	constexpr long long lowerLimit = std::numeric_limits<long long>::lowest();
	constexpr long long upperLimit = std::numeric_limits<long long>::max();
	double val;

	if (a.layer == 0) {
		val = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? upperLimit : lowerLimit : 0;
	}
	if (val > upperLimit) {
		return upperLimit;
	} else if (val < lowerLimit) {
		return lowerLimit;
	} else {
		return static_cast<long long>(val);
	}
}
double Decimal::toDouble(Decimal a) {
	constexpr double lowerLimit = std::numeric_limits<double>::lowest();
	constexpr double lowerMin = -std::numeric_limits<double>::min();
	constexpr double upperLimit = std::numeric_limits<double>::max();
	constexpr double upperMin = std::numeric_limits<double>::min();
	//I really don't like using long double due to it taking up a massive amount of memory but luckily the compiler cleans it up automatically so yay
	long double val;
	if (a.layer == 0) {
		val = a.sign * a.mag;
	} else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? upperLimit : lowerLimit : 0;
	}
	if (val > upperLimit) {
		return upperLimit;
	} else if (val < lowerLimit) {
		return lowerLimit;
	} else if (val < upperMin && val > 0) {
		return upperMin;
	} else if (val > lowerMin && val < 0) {
		return lowerMin;
	} else {
		return static_cast<double>(val);
	}
}
float Decimal::toFloat(Decimal a) {
	constexpr float lowerLimit = std::numeric_limits<float>::lowest();
	constexpr float lowerMin = -std::numeric_limits<float>::min();
	constexpr float upperLimit = std::numeric_limits<float>::max();
	constexpr float upperMin = std::numeric_limits<float>::min();
	//I really don't like using long double due to it taking up a massive amount of memory but luckily the compiler cleans it up automatically so yay
	long double val;
	if (a.layer == 0) {
		val = a.sign * a.mag;
	} else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? upperLimit : lowerLimit : 0;
	}
	if (val > upperLimit) {
		return upperLimit;
	} else if (val < lowerLimit) {
		return lowerLimit;
	} else if (val < upperMin && val > 0) {
		return upperMin;
	} else if (val > lowerMin && val < 0) {
		return lowerMin;
	} else {
		return static_cast<double>(val);
	}
}

double Decimal::d_maglog10(double a) {
	return std::log10(std::abs(a));
}

Decimal Decimal::neg(Decimal a) {
	return FC(-a.sign, a.layer, a.mag);
}

int Decimal::cmp(Decimal a, Decimal b) {
	if (a.sign > b.sign) {
		return 1;
	} else if (b.sign > a.sign) {
		return -1;
	}
	return a.sign * cmpabs(a, b);
}
int Decimal::compare(Decimal a, Decimal b) {
	return cmp(a, b);
}
int Decimal::cmpabs(Decimal a, Decimal b) {
	double layera = a.mag > 0 ? a.layer : -static_cast<long>(a.layer);
	double layerb = b.mag > 0 ? b.layer : -static_cast<long>(a.layer);

	if (layera > layerb) {
		return 1;
	}
	if (layerb > layera) {
		return -1;
	}
	if (a.mag > b.mag) {
		return 1;
	}
	if (b.mag > a.mag) {
		return -1;
	}
	return 1;
}
int Decimal::compareAbs(Decimal a, Decimal b) {
	return cmpabs(a, b);
}

bool Decimal::gt(Decimal a, Decimal b) {
	return cmp(a, b) == 1;
}
bool Decimal::greaterThan(Decimal a, Decimal b) {
	return gt(a, b);
}
bool Decimal::gte(Decimal a, Decimal b) {
	return !lt(a, b);
}
bool Decimal::greaterThanOrEqualTo(Decimal a, Decimal b) {
	return gte(a, b);
}

bool Decimal::lt(Decimal a, Decimal b) {
	return cmp(a, b) == -1;
}
bool Decimal::lessThan(Decimal a, Decimal b) {
	return lt(a, b);
}
bool Decimal::lte(Decimal a, Decimal b) {
	return !gt(a, b);
}
bool Decimal::lessThanOrEqualTo(Decimal a, Decimal b) {
	return lte(a, b);
}

Decimal Decimal::max(Decimal a, Decimal b) {
	return gt(a, b) ? a : b;
}
Decimal Decimal::clampMin(Decimal a, Decimal b) {
	return max(a, b);
}

Decimal Decimal::maxabs(Decimal a, Decimal b) {
	return cmpabs(a, b) > 0 ? D(b) : D(a);
}

Decimal Decimal::min(Decimal a, Decimal b) {
	return lt(a, b) ? a : b;
}
Decimal Decimal::clampMax(Decimal a, Decimal b) {
	return min(a, b);
}

Decimal Decimal::minabs(Decimal a, Decimal b) {
	return cmpabs(a, b) < 0 ? D(b) : D(a);
}

Decimal Decimal::clamp(Decimal a, Decimal b, Decimal c) {
	return min(max(a, b), c);
}


bool Decimal::eq(Decimal a, Decimal b) {
	return a.sign == b.sign && a.layer == b.layer && a.mag == b.mag;
}
bool Decimal::equal(Decimal a, Decimal b) {
	return eq(a, b);
}
bool Decimal::equals(Decimal a, Decimal b) {
	return eq(a, b);
}
bool Decimal::equality(Decimal a, Decimal b) {
	return eq(a, b);
}

bool Decimal::tolEq(Decimal a, Decimal b, double c) {
	if (a.sign != b.sign || std::abs(static_cast <double>(a.layer - b.layer)) > 0) {
		return false;
	}
	double magA = a.mag;
	double magB = b.mag;
	if (a.layer > b.layer) {
		magB = toSign(magB) * d_maglog10(magB);
	} else if (a.layer < b.layer) {
		magA = toSign(magA) * d_maglog10(magA);
	}
	return std::abs(magA - magB) <= c * std::max(std::abs(magA), std::abs(magB));
}
bool Decimal::tolEq(Decimal a, Decimal b) {
	return tolEq(a, b, 1e-7);
}
bool Decimal::tolerantEqual(Decimal a, Decimal b, double c) {
	return tolEq(a, b, c);
}
bool Decimal::tolerantEqual(Decimal a, Decimal b) {
	return tolEq(a, b);
}
bool Decimal::tolerantEquals(Decimal a, Decimal b, double c) {
	return tolEq(a, b, c);
}
bool Decimal::tolerantEquals(Decimal a, Decimal b) {
	return tolEq(a, b);
}
bool Decimal::tolerantEquality(Decimal a, Decimal b, double c) {
	return tolEq(a, b, c);
}
bool Decimal::tolerantEquality(Decimal a, Decimal b) {
	return tolEq(a, b);
}


Decimal Decimal::recip(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("Division by 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "Domain error: " << e.what() << std::endl;
	}
	if (a.layer == 0) {
		return FC(a.sign, 0, 1 / a.mag);
	} else {
		return FC(a.sign, a.layer, -a.mag);
	}
}
Decimal Decimal::reciprocal(Decimal a) {
	return recip(a);
}
Decimal Decimal::reciprocate(Decimal a) {
	return recip(a);
}
Decimal Decimal::inv(Decimal a) {
	return recip(a);
}
Decimal Decimal::inverse(Decimal a) {
	return recip(a);
}
Decimal Decimal::invert(Decimal a) {
	return recip(a);
}

Decimal Decimal::add(Decimal a, Decimal b) {
	Mag x = 0.0;
	if (a.sign == 0) {
		return D(b);
	}
	if (b.sign == 0) {
		return D(a);
	}
	if ((a.sign == -b.sign) && (a.layer == b.layer) && (a.mag == b.mag)) {
		return N();
	}

	if (a.layer >= 2 || b.layer >= 2) {
		return maxabs(a, b);
	}

	//this is a cool way to save time on writing code
	Decimal A;
	Decimal B;
	if (cmpabs(a, b) > 0) {
		A = a;
		B = b;
	} else {
		B = a;
		A = b;
	}

	//typical case of addition
	if (A.layer == 0 && B.layer == 0) {
		x = (A.mag * A.sign) + (B.mag * B.sign);
		return I(x);
	}
	int layera = A.layer * toSign(A.mag);
	int layerb = B.layer * toSign(B.mag);

	if (layera - layerb >= 2) {
		return D(A);
	}

	if (layera == 0 && layerb == -1) {
		x = B.mag - std::log10(A.mag);
		if (std::abs(x) > MAX_SIGNIFICANT_DIGITS) {
			return D(A);
		} else {
			double _magdiff = std::pow(10, -x);
			double _mantissa = B.sign + (A.sign * _magdiff);
			return FC(toSign(_mantissa), 1, B.mag + d_maglog10(_mantissa));
		}
	}

	if (layera == 1 && layerb == 0) {
		x = A.mag - std::log10(B.mag);
		if (std::abs(x) > MAX_SIGNIFICANT_DIGITS) {
			return D(A);
		} else {
			double _magdiff = std::pow(10, x);
			double _mantissa = B.sign + (A.sign * _magdiff);
			return FC(toSign(_mantissa), 1, std::log10(B.mag) + d_maglog10(_mantissa));
		}
	}

	x = A.mag - B.mag;
	if (std::abs(x) > MAX_SIGNIFICANT_DIGITS) {
		return D(A);
	} else {
		double _magdiff = std::pow(10, x);
		double _mantissa = B.sign + (A.sign * _magdiff);
		return FC(toSign(_mantissa), 1, B.mag + d_maglog10(_mantissa));
	}

	try {
		throw std::invalid_argument("Invalid arguments for addition");
	}
	catch (const std::invalid_argument e) {
		std::cerr << "Invalid argument error: " << e.what() << std::endl;
	}
}
Decimal Decimal::plus(Decimal a, Decimal b) {
	return add(a, b);
}
Decimal Decimal::sub(Decimal a, Decimal b) {
	return add(a, neg(b));
}
Decimal Decimal::subtract(Decimal a, Decimal b) {
	return sub(a, b);
}
Decimal Decimal::minus(Decimal a, Decimal b) {
	return sub(a, b);
}

Decimal Decimal::mul(Decimal a, Decimal b) {
	if (a.sign == 0 || b.sign == 0) {
		return N();
	}
	if (a.layer == b.layer && a.mag == -b.mag) {
		return FC(a.sign * b.sign, 0, 1);
	}

	Decimal A;
	Decimal B;
	if (a.layer > b.layer || (a.layer == b.layer && std::abs(a.mag) > std::abs(b.mag))) {
		A = a;
		B = b;
	} else {
		A = b;
		B = a;
	}
	
	//typical case of multiplication
	if (A.layer == 0 && B.layer == 0) {
		return I(A.sign * B.sign * A.mag * B.mag);
	}
	if (A.layer >= 3 || A.layer - B.layer == 2) {
		return FC(A.sign * B.sign, A.layer, A.mag);
	}
	if (A.layer == 1 && B.layer == 0) {
		return FC(A.sign * B.sign, 1, A.mag + std::log10(B.mag));
	}
	if (A.layer == 1 && B.layer == 1) {
		return FC(A.sign * B.sign, 1, A.mag + B.mag);
	}
	if ((A.layer == 2 && B.layer == 1) || (A.layer == 2 && B.layer == 2)) {
		Decimal _newDecimal = add(FC(toSign(A.mag), A.layer - 1, std::abs(A.mag)), (FC(toSign(B.mag), B.layer - 1, std::abs(B.mag))));
		return FC(A.sign * B.sign, _newDecimal.layer + 1, _newDecimal.sign * _newDecimal.mag);
	}
	try {
		throw std::invalid_argument("Invalid arguments for multiplication");
	} catch(const std::invalid_argument e) {
		std::cerr << "Invalid argument error: " << e.what() << std::endl;
		return N();
	}
}
Decimal Decimal::mult(Decimal a, Decimal b) {
	return mul(a, b);
}
Decimal Decimal::times(Decimal a, Decimal b) {
	return mul(a, b);
}
Decimal Decimal::multiply(Decimal a, Decimal b) {
	return mul(a, b);
}
Decimal Decimal::div(Decimal a, Decimal b) {
	return mul(a, recip(b));
}
Decimal Decimal::divide(Decimal a, Decimal b) {
	return div(a, b);
}
Decimal Decimal::divideBy(Decimal a, Decimal b) {
	return div(a, b);
}
Decimal Decimal::dividedBy(Decimal a, Decimal b) {
	return div(a, b);
}
Decimal Decimal::sr(Decimal a, Decimal b) {
	constexpr long long lowerLimit = std::numeric_limits<long long>::lowest() / 2;
	constexpr long long upperLimit = std::numeric_limits<long long>::max() / 2;
	double temp{};
	bool _outOfRange = false;
	long long _value{};

	if (a.layer == 0) {
		temp = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		temp = a.sign * std::pow(10, a.mag);
	}
	else {
		if (a.sign == 0 && a.mag == 0) {
			_value = 0;
		}
		else {
			_outOfRange = true;
		}
	}
	if (temp > upperLimit) {
		_outOfRange = true;
	}
	else if (temp < lowerLimit) {
		_outOfRange = true;
	}
	else {
		_value = static_cast<long long>(temp);
	}

	int _ops = toInt(b);
	if (_ops >= std::log2(_value)) {
		_outOfRange = true;
	}

	if (!_outOfRange) {
		return FC(a.sign, 0, static_cast<double>(_value >> _ops));
	}
	return div(a, pow2(b));
}
Decimal Decimal::rs(Decimal a, Decimal b) {
	return sr(a, b);
}

Decimal Decimal::log(Decimal a, Decimal b) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("log base 0");
		} else if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
			throw std::domain_error("log base 1");
		} else if (b.sign == 0) {
			throw std::domain_error("log 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "Domain error: " << e.what() << std::endl;
	}
	//change of base formula :)
	return div(log10(b), log10(a));
}
Decimal Decimal::absLog10(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("log 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "Domain error: " << e.what() << std::endl;
	}
	if (a.layer > 0) {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	} else {
		return FC(1, 0, std::log10(a.mag));
	}
}
Decimal Decimal::log10(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("log 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "Domain error: " << e.what() << std::endl;
	}
	if (a.layer > 0) {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	}
	else {
		return FC(a.sign, 0, std::log10(a.mag));
	}
}
Decimal Decimal::lg(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("lg 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "Domain error: " << e.what() << std::endl;
	}
	if (a.layer == 0) {
		return FC(a.sign, 0, std::log2(a.mag));
	} else if (a.layer == 1) {
		double _lg10 = 3.321928094887362;
		return FC(toSign(a.mag), 0, std::abs(a.mag) + _lg10);
	} else if (a.layer == 2) {
		double _minusloglog2 = 0.5213902276543247;
		return FC(toSign(a.mag), 1, std::abs(a.mag) + _minusloglog2);
	} else {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	}
}
Decimal Decimal::log2(Decimal a) {
	return lg(a);
}
Decimal Decimal::ln(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("ln 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "Domain error: " << e.what() << std::endl;
	}
	if (a.layer == 0) {
		return FC(a.sign, 0, std::log(a.mag));
	} else if (a.layer == 1) {
		double _ln10 = 2.302585092994046;
		return FC(toSign(a.mag), 0, std::abs(a.mag) * _ln10);
	} else if (a.layer == 2) {
		double _logloge = 0.36221568869946325;
		return FC(toSign(a.mag), 1, std::abs(a.mag) + _logloge);
	} else {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	}
}

Decimal Decimal::pow(Decimal a, Decimal b) {
	if (a.sign == 0) {
		return b.sign == 0 ? FC(1, 0, 1) : N();
	}
	if (b.sign == 0) {
		return FC(1, 0, 1);
	}
	if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
		return a;
	}
	if (b.sign == 1 && b.layer == 0 && b.mag == 1) {
		return a;
	}

	//to be honest, I have no clue what patashu is doing here, but I will add it
	Decimal _result = pow10(mul(absLog10(a), b));
	if (a.sign < 0) {
		long long _modulo = std::abs(toLongLong(b) % 2) % 2;
		if (_modulo == 1) {
			return neg(_result);
		} else if (_modulo == 0) {
			return _result;
		} else {
			return N();
		}
	}
	return _result;
}
Decimal Decimal::power(Decimal a, Decimal b) {
	return pow(a, b);
}
Decimal Decimal::raise(Decimal a, Decimal b) {
	return pow(a, b);
}
Decimal Decimal::pow10(Decimal a) {
	Decimal b = a;
	if (a.layer == 0) {
		double _newmag = std::pow(10, a.sign * a.mag);
		if (std::abs(_newmag) >= 0.1) {
			return FC(1, 0, _newmag);
		} else {
			if (a.sign == 0) {
				return FC(1, 0, 1);
			} else {
				b = FC(a.sign, a.layer + 1, std::log10(a.mag));
			}
		}
	}
	
	if (b.sign > 0 && b.mag >= 0) {
		return FC(b.sign, b.layer + 1, b.mag);
	} else if (b.sign < 0 && b.mag >= 0) {
		return FC(-b.sign, b.layer + 1, -b.mag);
	}
	return FC(1, 0, 1);
};
Decimal Decimal::power10(Decimal a) {
	return pow10(a);
}
Decimal Decimal::raise10(Decimal a) {
	return pow10(a);
}
Decimal Decimal::exp10(Decimal a) {
	return pow10(a);
}
Decimal Decimal::pow2(Decimal a) {
	if (a.sign == 0) {
		return FC(1, 0, 1);
	}
	if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
		return FC(1, 0, 2);
	}
	//this would be really easy if the layers worked in base 2, but they work in base 10 so it's like 20x more annoying
	double _log2 = 0.30102999566;
	if (a.layer == 0) {
		return FC(1, 1, a.sign * _log2 * a.mag);
	} else if (a.layer == 1) {
		return pow10(mul(a.sign, std::log10(_log2) + a.mag));
	} else {
		return FC(1, a.layer + 1, a.sign * a.mag);
	}
};
Decimal Decimal::power2(Decimal a) {
	return pow2(a);
}
Decimal Decimal::raise2(Decimal a) {
	return pow2(a);
}
Decimal Decimal::exp2(Decimal a) {
	return pow2(a);
}
Decimal Decimal::sl(Decimal a, Decimal b) {
	constexpr long long lowerLimit = std::numeric_limits<long long>::lowest() / 2; 
	constexpr long long upperLimit = std::numeric_limits<long long>::max() / 2;
	double temp{};
	bool _outOfRange = false;
	long long _value{};

	if (a.layer == 0) {
		temp = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		temp = a.sign * std::pow(10, a.mag);
	} else {
		if (a.sign == 0 && a.mag == 0) {
			_value = 0;
		} else {
			_outOfRange = true;
		}
	} 
	if (temp > upperLimit) {
		_outOfRange = true;
	} else if (temp < lowerLimit) {
		_outOfRange = true;
	} else {
		_value = static_cast<long long>(temp);
	}

	int _ops = toInt(b);
	if (_ops >= (63 - std::log2(_value))) {
		_outOfRange = true;
	}
	
	if (!_outOfRange) {
		return FC(a.sign, 0, static_cast<double>(_value << _ops));
	}
	return mul(a, pow2(b));
}
Decimal Decimal::ls(Decimal a, Decimal b) {
	return sl(a, b);
}
Decimal Decimal::exp(Decimal a) {
	if (a.sign == 0) {
		return FC(1, 0, 1);
	}
	if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
		return FC(1, 0, std::exp(1));
	}
	//this would be really easy if the layers worked in base e, but they work in base 10 so it's like (10e)x more annoying
	double _logE = 0.4342944819;
	if (a.layer == 0) {
		return FC(1, 1, a.sign * _logE * a.mag);
	} else if (a.layer == 1) {
		return pow10(mul(a.sign, std::log10(_logE) + a.mag));
	} else {
		return FC(1, a.layer + 1, a.sign * a.mag);
	}
};
Decimal Decimal::powE(Decimal a) {
	return exp(a);
}
Decimal Decimal::powerE(Decimal a) {
	return exp(a);
}
Decimal Decimal::raiseE(Decimal a) {
	return exp(a);
}

Decimal Decimal::sqrt(Decimal a) {
	return pow(a, I(0.5));
}
Decimal Decimal::root(Decimal a) {
	return sqrt(a);
}

Decimal Decimal::cbrt(Decimal a) {
	return pow(a, I(1.0 / 3.0));
}