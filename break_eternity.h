#ifndef BREAKETERNITY_H
#define BREAKETERNITY_H

#include <concepts>
#include <string>

class Decimal {
public:
	//uses int operations to set value to 0
	typedef int Sign;
	typedef unsigned long Layer;
	typedef double Mag;
	Sign sign;
	Layer layer;
	Mag mag;

	template <typename T>
	static Sign toSign(T a);
	template <typename T>
	static bool isZero(T a);

	template <typename T>
	Layer layerChecker(T a, Layer b);
	template <typename T>
	Mag setMagBasedOnLayer(T a, Layer b, Sign c);

	Sign signFromStr(std::string a);
	Layer layerFromStr(std::string a);
	Mag magFromStr(std::string a);

	//constructors
	//Decimal.normalize() constructor
	Decimal() : sign(0), layer(0), mag(0) {}
	//primary constructor, Decimal test(x), similar to BE's test = new Decimal(x)
	template <typename T>
		requires (std::integral<T> || std::floating_point<T>)
	Decimal(const T _value) : sign(toSign(_value)), layer(layerChecker(_value, 0)), mag(setMagBasedOnLayer(_value* sign, layer, sign)) {}
	//secondary constructor, Decimal test(sign, layer, mag), similar to BE's test = new Decimal.fromComponents(sign, layer, mag)
	Decimal(const Sign _sign, const Layer _layer, const Mag _mag) : sign(_sign), layer(_layer), mag(_mag) {}
	//tertiary constructor, Decimal test("x"), similar to BE's test = new Decimal("x")
	Decimal(const std::string _string) : sign(signFromStr(_string)), layer(layerFromStr(_string)), mag(magFromStr(_string)) {}


	//utilities
	static Decimal fromPtr(Decimal* a);
	static Decimal* toPtr(Decimal a);

	static int toInt(Decimal a);
	static long long toLongLong(Decimal a);
	static double toDouble(Decimal a);
	static float toFloat(Decimal a);

	static double d_maglog10(double a);

	//comparisons
	//-a
	static Decimal neg(Decimal a);
	//some comparison crap, I know what it does, but it's so simple to the point where I don't know how to word it
	static int cmp(Decimal a, Decimal b);
	static int compare(Decimal a, Decimal b);
	//for some reason this is a different function that just calling cmp(|a|, |b|) lol but that's essentially what it does
	static int cmpabs(Decimal a, Decimal b);
	static int compareAbs(Decimal a, Decimal b);
	//a > b
	static bool gt(Decimal a, Decimal b);
	static bool greaterThan(Decimal a, Decimal b);
	//a >= b
	static bool gte(Decimal a, Decimal b);
	static bool greaterThanOrEqualTo(Decimal a, Decimal b);
	//a < b
	static bool lt(Decimal a, Decimal b);
	static bool lessThan(Decimal a, Decimal b);
	//a <= b
	static bool lte(Decimal a, Decimal b);
	static bool lessThanOrEqualTo(Decimal a, Decimal b);

	//returns which Decimal is bigger
	static Decimal max(Decimal a, Decimal b);
	static Decimal clampMin(Decimal a, Decimal b);
	//max of a and b's magnitudes
	static Decimal maxabs(Decimal a, Decimal b);
	//returns which Decimal is smaller
	static Decimal min(Decimal a, Decimal b);
	static Decimal clampMax(Decimal a, Decimal b);
	//min of a and b's magnitudes
	static Decimal minabs(Decimal a, Decimal b);
	//if a < b || a > c, return a Decimal in that range
	static Decimal clamp(Decimal a, Decimal b, Decimal c);

	//a = b
	static bool eq(Decimal a, Decimal b);
	static bool equal(Decimal a, Decimal b);
	static bool equals(Decimal a, Decimal b);
	static bool equality(Decimal a, Decimal b);
	//I recommend using tolEq over eq due to Decimal.mag being a double
	//a = (b ± c), if c is not given, the tolerance is 1e-7
	static bool tolEq(Decimal a, Decimal b, double c);
	static bool tolEq(Decimal a, Decimal b);
	static bool tolerantEqual(Decimal a, Decimal b, double c);
	static bool tolerantEqual(Decimal a, Decimal b);
	static bool tolerantEquals(Decimal a, Decimal b, double c);
	static bool tolerantEquals(Decimal a, Decimal b);
	static bool tolerantEquality(Decimal a, Decimal b, double c);
	static bool tolerantEquality(Decimal a, Decimal b);

	//1/a
	static Decimal recip(Decimal a);
	static Decimal reciprocal(Decimal a);
	static Decimal reciprocate(Decimal a);
	static Decimal inv(Decimal a);
	static Decimal inverse(Decimal a);
	static Decimal invert(Decimal a);

	//operations
	//a + b
	static Decimal add(Decimal a, Decimal b);
	static Decimal plus(Decimal a, Decimal b);
	//a - b
	static Decimal sub(Decimal a, Decimal b);
	static Decimal subtract(Decimal a, Decimal b);
	static Decimal minus(Decimal a, Decimal b);

	//a * b
	static Decimal mul(Decimal a, Decimal b);
	static Decimal mult(Decimal a, Decimal b);
	static Decimal times(Decimal a, Decimal b);
	static Decimal multiply(Decimal a, Decimal b);
	//a / b
	static Decimal div(Decimal a, Decimal b);
	static Decimal divide(Decimal a, Decimal b);
	static Decimal divideBy(Decimal a, Decimal b);
	static Decimal dividedBy(Decimal a, Decimal b);
	//WARNING, only bitshifts when values are lower than the 64 bit integer limit, values over the 64 bit integer limit will try to emulate this behavior
	//this is (probably) faster than the div functions
	//roughly a / 2^(b) or a >> b
	static Decimal sr(Decimal a, Decimal b);
	static Decimal rs(Decimal a, Decimal b);

	//loga(b)
	static Decimal log(Decimal a, Decimal b);
	//log10(|a|)
	static Decimal absLog10(Decimal a);
	//log10(a)
	static Decimal log10(Decimal a);
	//log2(a)
	static Decimal lg(Decimal a);
	static Decimal log2(Decimal a);
	//ln(a)
	static Decimal ln(Decimal a);

	//a^b
	static Decimal pow(Decimal a, Decimal b);
	static Decimal power(Decimal a, Decimal b);
	static Decimal raise(Decimal a, Decimal b);
	//10^a
	static Decimal pow10(Decimal a);
	static Decimal power10(Decimal a);
	static Decimal raise10(Decimal a);
	static Decimal exp10(Decimal a);
	//2^a
	static Decimal pow2(Decimal a);
	static Decimal power2(Decimal a);
	static Decimal raise2(Decimal a);
	static Decimal exp2(Decimal a);
	//WARNING, only bitshifts when values are lower than the 64 bit integer limit, values over the 64 bit integer limit will try to emulate this behavior
	//this is (probably) faster than the pow2 functions
	//a * 2^(|b|) or a << b
	static Decimal sl(Decimal a, Decimal b);
	static Decimal ls(Decimal a, Decimal b);
	//e^a
	static Decimal exp(Decimal a);
	static Decimal powE(Decimal a);
	static Decimal powerE(Decimal a);
	static Decimal raiseE(Decimal a);

	//sqrt(a)
	static Decimal sqrt(Decimal a);
	static Decimal root(Decimal a);
	//cbrt(a)
	static Decimal cbrt(Decimal a);
};

#endif
