
#include "gtest/gtest.h"

#include "../cutecat.h"

//using namespace cutecat;

namespace {

static const char* lorem_ipsum = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
	"sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, "
	"sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet "
	"clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. "
	"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod "
	"tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At "
	"vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, "
	"no sea takimata sanctus est Lorem ipsum dolor sit amet.";

}

//----------------------------------------------------------------------------------------
TEST (StringTest, TestEmptyString) { 
	cutecat::BaseString<char> str;


	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get_array());

	char* d = str.get_writable_array();

	ASSERT_EQ(0, str.length());
	ASSERT_NE(nullptr, str.get_array());

	ASSERT_EQ(cutecat::BaseString<char>(), str);
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

	const char* c = lorem_ipsum;

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

	cutecat::BaseString<char> str = cutecat::FromRaw(c);
	ASSERT_EQ(3, str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ(0, ::strcmp(str.get_array(),c));

	cutecat::BaseString<char> str2(std::move(str));

	ASSERT_EQ(3, str2.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ(0, ::strcmp(str2.get_array(),c));
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestMoveConstruction) { 
	const char* c = lorem_ipsum;

	// create a string from a single static string
	cutecat::BaseString<char> str = cutecat::FromRaw(c);
	ASSERT_EQ(::strlen(c), str.length());
	ASSERT_NE(nullptr, str.get_array());
	ASSERT_EQ(0, ::strcmp(str.get_array(),c));

	cutecat::BaseString<char> str2(std::move(str));

	ASSERT_EQ(::strlen(c), str2.length());
	ASSERT_NE(nullptr, str2.get_array());
	ASSERT_EQ(0, ::strcmp(str2.get_array(),c));
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStringComparison) { 
	const char* c = lorem_ipsum;

	// create a string from a single static string
	cutecat::BaseString<char> str = cutecat::FromRaw(c);
	cutecat::BaseString<char> str2 = cutecat::FromRaw(c);
	cutecat::BaseString<char> str3 = cutecat::FromRaw(c);
	str3.set(0) = 'a';

	// string == string
	ASSERT_EQ(str, str2);
	ASSERT_NE(str, str3);

	cutecat::BaseStringSlice<const char> slice = str.get(0, 15);
	cutecat::BaseStringSlice<const char> slice2 = str2.get(0, 15);
	// const slice == const slice
	ASSERT_EQ(slice, slice2);

	slice2 = str2.get(0, 16);
	ASSERT_NE(slice, slice2);

	slice = str.get(1, 16);
	ASSERT_NE(slice, slice2);

	// check comparison against prefixes to make sure the slice boundary is honored
	cutecat::BaseString<char> strPrefix = cutecat::FromStatic("Lorem");
	slice = str.get(0, 5);
	ASSERT_EQ(slice, strPrefix);
	ASSERT_EQ(strPrefix, slice);

	slice = str.get(0, 6);
	ASSERT_NE(slice, strPrefix);
	ASSERT_NE(strPrefix, slice);

	cutecat::BaseStringSlice<const char> sliceFull = str.get(0, str.length());
	cutecat::BaseStringSlice<const char> sliceFull2 = str2.get(0, str.length());

	// (const) slice == string and vice versa
	ASSERT_EQ(str, sliceFull);
	ASSERT_EQ(sliceFull2, str2);
	ASSERT_EQ(str2, sliceFull);
	ASSERT_EQ(sliceFull, str);
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStringEmptySlicing) { 
	const char* c = lorem_ipsum;

	cutecat::BaseString<char> str1 = cutecat::FromRaw(c);
	cutecat::BaseString<char> str2 = cutecat::FromStatic("foo");
	cutecat::BaseString<char> str3 = cutecat::FromStatic("foobar");

	const std::size_t len = str1.length();

	ASSERT_EQ(str1.get(0,0), str2.get(0,0));
	ASSERT_EQ(str1.set(0,0), str2.get(0,0));

	// assign empty slice of different string
	str1.set(0,0) = str2.get(0,0);
	str1.set(0,0) = str2.get(2,2);

	// assign empty slice of same string
	str1.set(0,0) = str1.get(0,0);
	str1.set(0,0) = str1.get(15,15);

	ASSERT_EQ(str1.get(0,0), str2.get(0,0));
	ASSERT_EQ(str1.set(0,0), str2.get(0,0));
	ASSERT_EQ(len, str1.length());
	ASSERT_TRUE(!::strcmp(str1.get_array(),c));
	ASSERT_TRUE(!::strcmp(str1.get(0,0).begin(),c));
	ASSERT_TRUE(!::strcmp(str1.set(0,0).begin(),c));
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStringSelfAssignment) { 
	// internalized
	cutecat::BaseString<char> str1 = cutecat::FromRaw("foo");
	str1 = str1;

	// static
	cutecat::BaseString<char> str2 = cutecat::FromStatic(lorem_ipsum);
	str2 = str2;

	// heap
	cutecat::BaseString<char> str3 = cutecat::FromRaw(lorem_ipsum);
	str3 = str3;

	ASSERT_EQ(::strlen(lorem_ipsum),str2.length());
	ASSERT_EQ(::strlen(lorem_ipsum),str3.length());
	ASSERT_EQ(3,str1.length());
}

//----------------------------------------------------------------------------------------
#define CompareAndCheckLengthConsistency(str,expected) \
{ \
	ASSERT_EQ(str, cutecat::BaseString<char>(cutecat::FromRaw(expected))); \
	ASSERT_TRUE(!strcmp((str).get_array(),expected)); \
	ASSERT_TRUE(::strlen((str).get_array()) == (str).length()); \
}


//----------------------------------------------------------------------------------------
TEST (StringTest, TestStringSlicing) { 
	const char* c = lorem_ipsum;

	cutecat::BaseString<char> str1 = cutecat::FromRaw(c);

	// test basic slice access
	str1.set(1,5)[0] = 'a';
	ASSERT_EQ(str1.get(1), 'a');

	cutecat::BaseStringSlice<char> s = str1.set(1,5);
	for( char* c = s.begin(); c != s.end(); ++c) {
		*c = 'a';
	} 

	// equality of different slices
	ASSERT_EQ(str1.get(1,5), s);
	ASSERT_EQ(s, str1.get(1,5));
	ASSERT_EQ(str1.set(1,5), str1.get(1,5));

	//
	cutecat::BaseString<char> str2 = cutecat::FromStatic("abcdef");
	cutecat::BaseString<char> str3 = cutecat::FromStatic("aaaaaa");

	// insert str3 at the beginning of str2 
	str2.set(0,0) = str3;
	CompareAndCheckLengthConsistency(str3,"aaaaaa");
	CompareAndCheckLengthConsistency(str2,"aaaaaaabcdef");

	// insert str2 at the beginning of str3
	str3.set(0,0) = str2;
	CompareAndCheckLengthConsistency(str2,"aaaaaaabcdef");
	CompareAndCheckLengthConsistency(str3,"aaaaaaabcdefaaaaaa");

	// extract middle part of str3 and assign to str2 using normal assignment
	str2.set(0,str2.length()) = str3.get(6,12);
	//str2 = str3.get(6,12);
	CompareAndCheckLengthConsistency(str2,"abcdef");
	CompareAndCheckLengthConsistency(str3,"aaaaaaabcdefaaaaaa");

	// duplicate str2 by appending to the end
	str2.set(str2.length(),str2.length()) = str2;
	CompareAndCheckLengthConsistency(str2,"abcdefabcdef");

	// do that again, but this time insert at the second last position
	str2.set(str2.length() - 1,str2.length() - 1) = str2;
	CompareAndCheckLengthConsistency(str2,"abcdefabcdeabcdefabcdeff");

	// do that again, but this time replace everything between the first and last character
	str2.set(1,str2.length() - 1) = str2;
	CompareAndCheckLengthConsistency(str2,"aabcdefabcdeabcdefabcdefff");

	// do that again, but this time set everything between the first and last character nil
	str2.set(1,str2.length() - 1) = cutecat::FromStatic("");
	CompareAndCheckLengthConsistency(str2,"af");

	// extract middle part of str3 and assign to str2 using slice assignment
	str2.set(0,str2.length()) = str3.get(6,12);
	CompareAndCheckLengthConsistency(str2,"abcdef");
	CompareAndCheckLengthConsistency(str3,"aaaaaaabcdefaaaaaa");
}

/* vi: set shiftwidth=4 tabstop=4: */ 

