#pragma once
#include <type_traits>

namespace Peano
{
	using std::true_type;
	using std::false_type;

	// Zero
	struct Z { using IS_NATURAL = void; };

	// Positive Integers
	template<class Nat>
	struct S { using IS_NATURAL = typename Nat::IS_NATURAL; using IS_POSITIVE = void; };

	// Define Negative Numbers
	template<class Nat>	struct NEG { using IS_NEGATIVE = typename Nat::IS_POSITIVE; };
	template<class A>	struct Negation;
	template<>			struct Negation<Z> { using result = Z; };
	template<class A>	struct Negation<S<A>> { using result = NEG<S<A>>; };
	template<class A>	struct Negation<NEG<S<A>>> { using result = S<A>; };
	template<class A>	using Negate = typename Negation<A>::result;

	// Types			    // Values (compile time constants)
							constexpr static Z Zero;
	using ONE = S<Z>;		constexpr static ONE One;
	using TWO = S<ONE>;		constexpr static TWO Two;
	using THREE = S<TWO>;	constexpr static THREE Three;
	using FOUR = S<THREE>;	constexpr static FOUR Four;
	using FIVE = S<FOUR>;	constexpr static FIVE Five;
	using SIX = S<FIVE>;	constexpr static SIX Six;
	using SEVEN = S<SIX>;	constexpr static SEVEN Seven;
	using EIGHT = S<SEVEN>; constexpr static EIGHT Eight;
	using NINE = S<EIGHT>;	constexpr static NINE Nine;
	using TEN = S<NINE>;	constexpr static TEN Ten;

	// Convert to int
	struct toInt {
		int i;
		// Construct from Z or S<A>
		constexpr					toInt(Z z) : i(0) {}
		template<class A> constexpr toInt(S<A> s) : i(1 + toInt(A())) {}
		template<class A> constexpr toInt(NEG<A> n) : i(-toInt(A())) {}
		// Implicitly convert to int
		constexpr operator int() const { return i; }
	};

	// Define equality
	template<class A, class B>		struct Equality { using result = false_type; };
	template<class A>				struct Equality<A, A> { using result = true_type; };
	template<class A, class B>		using Equal = typename Equality<A, B>::result;

	// Define comparison
	struct EQUAL { using less = false_type; using leq = true_type; using equal = true_type; using geq = true_type; using greater = false_type; };
	struct LESS { using less = true_type; using leq = true_type; using equal = false_type; using geq = false_type; using greater = false_type; };
	struct GREATER { using less = false_type; using leq = false_type; using equal = false_type; using geq = true_type; using greater = true_type; };
	template<class A, class B>		struct Comparison;
	template<>						struct Comparison<Z, Z>				{ using result = EQUAL; };
	template<class B>				struct Comparison<Z, S<B>>			{ using result = LESS; };
	template<class A>				struct Comparison<S<A>, Z>			{ using result = GREATER; };
	template<class A, class B>		struct Comparison<S<A>, S<B>>		{ using result = typename Comparison<A, B>::result; };
	template<class A, class B>		struct Comparison<NEG<A>, B>		{ using result = LESS; };
	template<class A, class B>		struct Comparison<A, NEG<B>>		{ using result = GREATER; };
	template<class A, class B>		struct Comparison<NEG<A>, NEG<B>>	{ using result = typename Comparison<B, A>::result; };
	template<class A, class B>		using Less		= typename Comparison<A, B>::result::less;
	template<class A, class B>		using Leq		= typename Comparison<A, B>::result::leq;
	template<class A, class B>		using Greater	= typename Comparison<A, B>::result::greater;
	template<class A, class B>		using Geq		= typename Comparison<A, B>::result::geq;

	// Define Addition
	template<class A, class B>		struct Addition;
	template<class B>				struct Addition<Z, B>				{ using result = B; };
	template<class A, class B>		struct Addition<S<A>, B>			{ using result = S<typename Addition<A, B>::result>; };
	template<class A, class B>		struct Addition<NEG<A>, B>			{ using result = Negate<typename Addition<A, Negate<B>>::result>; };
	template<class A, class B>		struct Addition<S<A>, NEG<S<B>>>	{ using result = typename Addition<A, NEG<B>>::result; };
	template<class A, class B>		using Add = typename Addition<A, B>::result;

	// Define Subtraction
	template<class A, class B>		using Subtract = typename Addition<A, Negate<B>>::result;

	// Define Multiplication
	template<class A, class B>		struct Multiplication;
	template<class B>				struct Multiplication<Z, B>			{ using result = Z; };
	template<class A, class B>		struct Multiplication<S<A>, B>		{ using result = Add<B, typename Multiplication<A, B>::result>; };
	template<class A, class B>		struct Multiplication<NEG<A>, B>	{ using result = Negate<typename Multiplication<A, B>::result>; };
	template<class A, class B>		struct Multiplication<S<A>, NEG<B>> { using result = Negate<typename Multiplication<S<A>, B>::result>; };
	template<class A, class B>		using Multiply = typename Multiplication<A, B>::result;

	// Value-level operators (still compile time)
	template<class A, class B> constexpr auto operator+ (A a, B b) -> Add<A, B>			{ return Add<A, B>(); }
	template<class A>		   constexpr auto operator- (A a)      -> Negate<A>			{ return Negate<A>(); }
	template<class A, class B> constexpr auto operator- (A a, B b) -> Subtract<A, B>	{ return Subtract<A, B>(); }
	template<class A, class B> constexpr auto operator* (A a, B b) -> Multiply<A, B>	{ return Multiply<A, B>(); }
	template<class A, class B> constexpr auto operator==(A a, B b) -> Equal<A, B>		{ return Equal<A, B>(); }
	template<class A, class B> constexpr auto operator< (A a, B b) -> Less<A, B>		{ return Less<A, B>(); }
	template<class A, class B> constexpr auto operator<=(A a, B b) -> Leq<A, B>			{ return Leq<A, B>(); }
	template<class A, class B> constexpr auto operator> (A a, B b) -> Greater<A, B>		{ return Greater<A, B>(); }
	template<class A, class B> constexpr auto operator>=(A a, B b) -> Geq<A, B>			{ return Geq<A, B>(); }
}