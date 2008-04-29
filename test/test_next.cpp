//  (C) Copyright John Maddock 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/concepts/real_concept.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/next.hpp>

template <class T>
void test_value(const T& val, const char* name)
{
   using namespace boost::math;
   T upper = tools::max_value<T>();
   T lower = -upper;

   std::cout << "Testing type " << name << " with initial value " << val << std::endl;

   BOOST_CHECK_EQUAL(edit_distance(next_greater(val), val), 1);
   BOOST_CHECK(next_greater(val) > val);
   BOOST_CHECK_EQUAL(edit_distance(next_less(val), val), 1);
   BOOST_CHECK(next_less(val) < val);
   BOOST_CHECK_EQUAL(edit_distance(nextafter(val, upper), val), 1);
   BOOST_CHECK(nextafter(val, upper) > val);
   BOOST_CHECK_EQUAL(edit_distance(nextafter(val, lower), val), 1);
   BOOST_CHECK(nextafter(val, lower) < val);
   BOOST_CHECK_EQUAL(edit_distance(next_greater(next_greater(val)), val), 2);
   BOOST_CHECK_EQUAL(edit_distance(next_less(next_less(val)), val), 2);
   BOOST_CHECK_EQUAL(edit_distance(next_less(next_greater(val)), val), 0);
   BOOST_CHECK_EQUAL(edit_distance(next_greater(next_less(val)), val), 0);
   BOOST_CHECK_EQUAL(next_less(next_greater(val)), val);
   BOOST_CHECK_EQUAL(next_greater(next_less(val)), val);
}

template <class T>
void test_values(const T& val, const char* name)
{
   static const T a = static_cast<T>(1.3456724e22);
   static const T b = static_cast<T>(1.3456724e-22);
   static const T z = 0;
   static const T one = 1;
   static const T two = 2;

   test_value(a, name);
   test_value(-a, name);
   test_value(b, name);
   test_value(-b, name);
   test_value(boost::math::tools::epsilon<T>(), name);
   test_value(-boost::math::tools::epsilon<T>(), name);
   test_value(boost::math::tools::min_value<T>(), name);
   test_value(-boost::math::tools::min_value<T>(), name);
   test_value(z, name);
   test_value(-z, name);
   test_value(one, name);
   test_value(-one, name);
   test_value(two, name);
   test_value(-two, name);
   if(std::numeric_limits<T>::is_specialized && std::numeric_limits<T>::has_denorm)
   {
      test_value(std::numeric_limits<T>::denorm_min(), name);
      test_value(-std::numeric_limits<T>::denorm_min(), name);
      test_value(2 * std::numeric_limits<T>::denorm_min(), name);
      test_value(-2 * std::numeric_limits<T>::denorm_min(), name);
   }

   static const unsigned primes[] = {
      11,     13,     17,     19,     23,     29, 
      31,     37,     41,     43,     47,     53,     59,     61,     67,     71, 
      73,     79,     83,     89,     97,    101,    103,    107,    109,    113, 
      127,    131,    137,    139,    149,    151,    157,    163,    167,    173, 
      179,    181,    191,    193,    197,    199,    211,    223,    227,    229, 
      233,    239,    241,    251,    257,    263,    269,    271,    277,    281, 
      283,    293,    307,    311,    313,    317,    331,    337,    347,    349, 
      353,    359,    367,    373,    379,    383,    389,    397,    401,    409, 
      419,    421,    431,    433,    439,    443,    449,    457,    461,    463, 
   };

   for(unsigned i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i)
   {
      T v1 = val;
      T v2 = val;
      for(unsigned j = 0; j < primes[i]; ++j)
      {
         v1 = boost::math::next_greater(v1);
         v2 = boost::math::next_less(v2);
      }
      BOOST_CHECK_EQUAL(boost::math::edit_distance(v1, val), primes[i]);
      BOOST_CHECK_EQUAL(boost::math::edit_distance(v2, val), primes[i]);
   }
}

int test_main(int, char* [])
{
   std::cout << boost::math::edit_distance(1.0, 0.0) << std::endl;
   test_values(1.0f, "float");
   test_values(1.0, "double");
   test_values(1.0L, "long double");
   test_values(boost::math::concepts::real_concept(0), "real_concept");
   return 0;
}

