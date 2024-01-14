/***********************************************************************                                                                         *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
***************************************************************************/

#define BOOST_TEST_MODULE project_test
//#include <boost/test/included/unit_test.hpp> // single-header
#include <boost/test/unit_test.hpp>
#include <project/project.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE( tada )

BOOST_AUTO_TEST_CASE (tada_case)
{
  int v = 4;
  int w = 8
  int expected = 8/2;
  BOOST_TEST(v == expected);
}

BOOST_AUTO_TEST_SUITE_END()
