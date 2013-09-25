
#include <vector>

#include "gtest/gtest.h"

#include "../cutecat.h"

using cutecat::String;
using cutecat::BaseString;
using cutecat::FromRaw;
using cutecat::FromStatic;

//----------------------------------------------------------------------------------------
TEST (SplitTest, TestSplitToSlice) { 

	// TODO: typedefs for slices
	String sbase = FromStatic("foo bar fob");

	typedef std::vector< BaseString<char>::ImmutableSliceType > SliceType;

	// split normal
	SliceType slices;
	Split(' ', sbase, std::back_inserter(slices));
	ASSERT_EQ(3,slices.size());
	ASSERT_EQ("foo",slices[0]);
	ASSERT_EQ("bar",slices[1]);
	ASSERT_EQ("fob",slices[2]);

	slices.clear();

	// split empty string
	std::back_insert_iterator<SliceType> s = std::back_inserter(slices);
	std::back_insert_iterator<SliceType> e = Split(' ', String(), s);
	ASSERT_EQ(0,slices.size());
	*e++ = sbase(0,1);
	ASSERT_EQ(1,slices.size());

	// split string with multiple split characters
	slices.clear();
	sbase = FromStatic(" foo  bar  fob ");
	Split(' ', sbase, std::back_inserter(slices));
	ASSERT_EQ(3,slices.size());
	ASSERT_EQ("foo",slices[0]);
	ASSERT_EQ("bar",slices[1]);
	ASSERT_EQ("fob",slices[2]);

	// split string with multiple split characters, but do not merge adjacent characters
	slices.clear();
	sbase = FromStatic(" foo  bar  fob ");
	Split(' ', sbase, std::back_inserter(slices), false);
	ASSERT_EQ(7,slices.size());
	ASSERT_EQ("",slices[0]);
	ASSERT_EQ("foo",slices[1]);
	ASSERT_EQ("",slices[2]);
	ASSERT_EQ("bar",slices[3]);
	ASSERT_EQ("",slices[4]);
	ASSERT_EQ("fob",slices[5]);
	ASSERT_EQ("",slices[6]);

	// same, but using a vector to store it
	slices.clear();
	Split(' ', sbase, slices, false);
	ASSERT_EQ(7,slices.size());
	ASSERT_EQ(7,slices.capacity());
	ASSERT_EQ("foo",slices[1]);
	ASSERT_EQ("",slices[2]);
	ASSERT_EQ("bar",slices[3]);
	ASSERT_EQ("",slices[4]);
	ASSERT_EQ("fob",slices[5]);
	ASSERT_EQ("",slices[6]);
}
