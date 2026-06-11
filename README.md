# breaketernity.h
breaketernity.h is a port of breaketernity.js by Patashu to C++

# How to install
+ Download the latest release in the releases section, and add the files to your C++ project
+ MAKE SURE YOU PUT THESE AT THE TOP OF YOUR FILE
- #include "breaketernity.h"
- #include "breaketernity.cpp"
+ All done!

# Four primary constructors

+ Decimal x = Decimal(), returns {sign: 0, layer: 0, mag: 0}
+ Decimal x = Decimal(any numeric type a), returns {sign: a's sign, layer: a's layer, mag: a's mag}
+ Decimal x = Decimal(int a, unsigned long b, double c), returns {sign: a, layer: b, mag: c}
+ Decimal x = Decimal(std::string a), returns {sign: a's sign, layer: a's layer, mag: a's mag}

# How to call functions

+ In your .cpp file, just put Decimal::whatever(args), and it will call the function! ✅✅✅✅✅✅✅

If you want to see some test breaketernity.h calls, compile examples.cpp and look at my awesome math 🥰

I'm just gonna copy-paste the methods
# Utilities
+ pointer manipulation
  
static Decimal fromPtr(Decimal* a);

static Decimal* toPtr(Decimal a);

+ export to type

static int toInt(Decimal a);

static long long toLongLong(Decimal a);

static double toDouble(Decimal a);

static float toFloat(Decimal a);

+ spooky secret thing

static double d_maglog10(double a);

# Comparisons
+ -a

static Decimal neg(Decimal a);

+ some comparison crap, I know what it does, but it's so simple to the point where I don't know how to word it

static int cmp(Decimal a, Decimal b);

static int compare(Decimal a, Decimal b);

+ for some reason this is a different function that just calling cmp(|a|, |b|) lol but that's essentially what it does

static int cmpabs(Decimal a, Decimal b);

static int compareAbs(Decimal a, Decimal b);

+ a > b

static bool gt(Decimal a, Decimal b);

static bool greaterThan(Decimal a, Decimal b);

+ a >= b

static bool gte(Decimal a, Decimal b);

static bool greaterThanOrEqualTo(Decimal a, Decimal b);

//a < b

static bool lt(Decimal a, Decimal b);

static bool lessThan(Decimal a, Decimal b);

+ a <= b

static bool lte(Decimal a, Decimal b);

static bool lessThanOrEqualTo(Decimal a, Decimal b);

+ returns which Decimal is bigger

static Decimal max(Decimal a, Decimal b);

static Decimal clampMin(Decimal a, Decimal b);

+ max of a and b's magnitudes

static Decimal maxabs(Decimal a, Decimal b);

+ returns which Decimal is smaller

static Decimal min(Decimal a, Decimal b);

static Decimal clampMax(Decimal a, Decimal b);

+ min of a and b's magnitudes

static Decimal minabs(Decimal a, Decimal b);

+ if a < b || a > c, return a Decimal in that range

static Decimal clamp(Decimal a, Decimal b, Decimal c);

+ a = b
+ 
static bool eq(Decimal a, Decimal b);

static bool equal(Decimal a, Decimal b);

static bool equals(Decimal a, Decimal b);

static bool equality(Decimal a, Decimal b);

- I recommend using tolEq over eq due to Decimal.mag being a double

+ a = (b ± c), if c is not given, the tolerance is 1e-7

static bool tolEq(Decimal a, Decimal b, double c);

static bool tolEq(Decimal a, Decimal b);

static bool tolerantEqual(Decimal a, Decimal b, double c);

static bool tolerantEqual(Decimal a, Decimal b);

static bool tolerantEquals(Decimal a, Decimal b, double c);

static bool tolerantEquals(Decimal a, Decimal b);

static bool tolerantEquality(Decimal a, Decimal b, double c);

static bool tolerantEquality(Decimal a, Decimal b);

+ 1/a

static Decimal recip(Decimal a);

static Decimal reciprocal(Decimal a);

static Decimal reciprocate(Decimal a);

static Decimal inv(Decimal a);

static Decimal inverse(Decimal a);

static Decimal invert(Decimal a);

# Operations
+ a + b

static Decimal add(Decimal a, Decimal b);

static Decimal plus(Decimal a, Decimal b);

+ a - b

static Decimal sub(Decimal a, Decimal b);

static Decimal subtract(Decimal a, Decimal b);

static Decimal minus(Decimal a, Decimal b);

+ a * b

static Decimal mul(Decimal a, Decimal b);

static Decimal mult(Decimal a, Decimal b);

static Decimal times(Decimal a, Decimal b);

static Decimal multiply(Decimal a, Decimal b);

+ a / b

static Decimal div(Decimal a, Decimal b);

static Decimal divide(Decimal a, Decimal b);

static Decimal divideBy(Decimal a, Decimal b);

static Decimal dividedBy(Decimal a, Decimal b);

- WARNING, only bitshifts when values are lower than the 64 bit integer limit, values over the 64 bit integer limit will try to emulate this behavior

- this is (probably) faster than the div functions

+ roughly a / 2^(b) or a >> b

static Decimal sr(Decimal a, Decimal b);

static Decimal rs(Decimal a, Decimal b);

+ loga(b)

static Decimal log(Decimal a, Decimal b);

+ log10(|a|)

static Decimal absLog10(Decimal a);

+ log10(a)

static Decimal log10(Decimal a);

+ log2(a)

static Decimal lg(Decimal a);

static Decimal log2(Decimal a);

+ ln(a)

static Decimal ln(Decimal a);

+ a^b

static Decimal pow(Decimal a, Decimal b);

static Decimal power(Decimal a, Decimal b);

static Decimal raise(Decimal a, Decimal b);

+ 10^a

static Decimal pow10(Decimal a);

static Decimal power10(Decimal a);

static Decimal raise10(Decimal a);

static Decimal exp10(Decimal a);

+ 2^a

static Decimal pow2(Decimal a);

static Decimal power2(Decimal a);

static Decimal raise2(Decimal a);

static Decimal exp2(Decimal a);

- WARNING, only bitshifts when values are lower than the 64 bit integer limit, values over the 64 bit integer limit will try to emulate this behavior

- this is (probably) faster than the pow2 functions

+ a * 2^(|b|) or a << b

static Decimal sl(Decimal a, Decimal b);

static Decimal ls(Decimal a, Decimal b);

+ e^a

static Decimal exp(Decimal a);

static Decimal powE(Decimal a);

static Decimal powerE(Decimal a);

static Decimal raiseE(Decimal a);

+ sqrt(a)

static Decimal sqrt(Decimal a);

static Decimal root(Decimal a);

+ cbrt(a)

static Decimal cbrt(Decimal a);
