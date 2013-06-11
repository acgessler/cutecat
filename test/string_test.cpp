
#include "gtest/gtest.h"

#include "../cutecat.h"

//using namespace cutecat;

TEST (StringTest, TestEmptyString) { 
	cutecat::BaseString<char> str;

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get_array());

	char* d = str.get_writable_array();

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get_array());
}


TEST (StringTest, TestStaticStringLengthDeduction) { 

	cutecat::StaticBaseStringWithKnownLength<char,5> s = cutecat::FromStatic("blorb");
}


TEST (StringTest, TestStaticString) { 
	const char* c = "foo";

	// create two strings from a single static string
	cutecat::BaseString<char> str = cutecat::FromStatic(c);
	cutecat::BaseString<char> str2 = cutecat::FromStatic(c);

	// they should be equal
	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ('f', str[0]);
	ASSERT_EQ('o', str[1]);
	ASSERT_EQ('o', str[2]);

	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ('f', str2[0]);
	ASSERT_EQ('o', str2[1]);
	ASSERT_EQ('o', str2[2]);

	ASSERT_EQ(str.get_array(), str2.get_array());
	//ASSERT_EQ(str,str2);

	// now modify one of them - this should trigger COW
	char* d = str.get_writable_array();
	d[2] = 'r';

	// and str2 still should have its old value
	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ('f', str[0]);
	ASSERT_EQ('o', str[1]);
	ASSERT_EQ('r', str[2]);

	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ('f', str2[0]);
	ASSERT_EQ('o', str2[1]);
	ASSERT_EQ('o', str2[2]);

	ASSERT_NE(str2.get_array(), str.get_array());
	//ASSERT_NE(str,str2);

	// now undo the change to str - now they should compare equal again
	d[2] = 'o';
	//ASSERT_EQ(str,str2);
}

/* vi: set shiftwidth=4 tabstop=4: */ 

