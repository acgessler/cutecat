
#include "gtest/gtest.h"

#include "../cutecat.h"

using namespace cutecat;

TEST (StringTest, TestEmptyString) { 
	BaseString<char> str;

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get());

	char* d = str.get_writable();

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get());
}

/* vi: set shiftwidth=4 tabstop=4: */ 

