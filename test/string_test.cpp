
#include "gtest/gtest.h"

#include "../cutecat.h"

//using namespace cutecat;


//----------------------------------------------------------------------------------------
TEST (StringTest, TestEmptyString) { 
	cutecat::BaseString<char> str;

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get_array());

	char* d = str.get_writable_array();

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get_array());
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStaticStringLengthDeduction) { 

	cutecat::detail::StaticBaseStringWithKnownLength<char,5> s = cutecat::FromStatic("blorb");
	cutecat::BaseString<char> str(s);
	ASSERT_EQ(5, str.length());
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStaticString_Internalized) { 
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

	ASSERT_EQ(3, str2.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ('f', str2[0]);
	ASSERT_EQ('o', str2[1]);
	ASSERT_EQ('o', str2[2]);

	// and the data pointers should be identical
	ASSERT_EQ(str.get_array(), str2.get_array());
	ASSERT_EQ(str.get_array(), c);
	//ASSERT_EQ(str,str2);

	// now modify one of them - this should trigger COW, 
	// but the resulting string should be internalized
	char* d = str.get_writable_array();
	ASSERT_EQ(0, ::strcmp(d,c));
	d[2] = 'r';

	// and str2 still should have its old value
	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ('f', str[0]);
	ASSERT_EQ('o', str[1]);
	ASSERT_EQ('r', str[2]);

	ASSERT_EQ(3, str2.length());
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


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStaticString_NotInternalized) { 

	const char* c = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
		"sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, "
		"sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet "
		"clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. "
		"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod "
		"tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At "
		"vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, "
		"no sea takimata sanctus est Lorem ipsum dolor sit amet.";

	// create two strings from a single static string
	cutecat::BaseString<char> str = cutecat::FromStatic(c);
	cutecat::BaseString<char> str2 = cutecat::FromStatic(c);

	// they should be equal
	ASSERT_EQ(::strlen(c), str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ('L', str[0]);

	ASSERT_EQ(::strlen(c), str.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ('L', str2[0]);

	// and the data pointers should be identical
	ASSERT_EQ(str.get_array(), str2.get_array());
	ASSERT_EQ(str.get_array(), c);
	//ASSERT_EQ(str,str2);

	// now modify one of them - this should trigger COW, 
	// but the resulting string is too long to be internalized
	char* d = str.get_writable_array();
	ASSERT_EQ(0, ::strcmp(d,c));
	d[0] = 'r';

	// and str2 still should have its old value
	ASSERT_EQ(::strlen(c), str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ('r', str[0]);

	ASSERT_EQ(::strlen(c), str2.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ('L', str2[0]);

	ASSERT_NE(str2.get_array(), str.get_array());
	//ASSERT_NE(str,str2);

	// now undo the change to str - now they should compare equal again
	d[0] = 'L';
	//ASSERT_EQ(str,str2);
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestMoveConstructionFromStatic) { 
	const char* c = "foobar";

	// create a string from a single static string
	cutecat::BaseString<char> str = cutecat::FromStatic(c);
	ASSERT_EQ(6, str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ(0, ::strcmp(str.get_array(),c));

	cutecat::BaseString<char> str2(std::move(str));

	ASSERT_EQ(6, str2.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ(0, ::strcmp(str2.get_array(),c));
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestMoveConstructionFromInternalized) { 
	const char* c = "foo";

	// create a string from a single static string
	cutecat::BaseString<char> str = cutecat::FromRaw(c);
	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ(0, ::strcmp(str.get_array(),c));

	cutecat::BaseString<char> str2(std::move(str));

	ASSERT_EQ(3, str2.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ(0, ::strcmp(str2.get_array(),c));
}

/* vi: set shiftwidth=4 tabstop=4: */ 

