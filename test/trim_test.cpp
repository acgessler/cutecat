
#include "gtest/gtest.h"

#include "../cutecat.h"

using cutecat::String;
using cutecat::FromRaw;
using cutecat::FromStatic;

//----------------------------------------------------------------------------------------
TEST (TrimTest, TestNormalTrim) { 

	String sbase = FromStatic("foo bar"), s = sbase;
	Trim(sbase);
	ASSERT_EQ(sbase, s);

	s = FromStatic(" foo bar");
	Trim(s);
	ASSERT_EQ(s, sbase);

	s = FromStatic("foo bar ");
	Trim(s);
	ASSERT_EQ(s, sbase);

	s = FromStatic("    foo bar   ");
	Trim(s);
	ASSERT_EQ(s, sbase);

	s = FromStatic(" \t \t  foo bar   \n");
	Trim(s);
	ASSERT_EQ(s, sbase);

	s = String();
	Trim(s);
	ASSERT_EQ(s, String());

	s = FromStatic("  \t\n  ");
	Trim(s);
	ASSERT_EQ(s, String());

	s = FromStatic("  a  ");
	Trim(s);
	ASSERT_EQ(s, String(FromStatic("a")));
}


//----------------------------------------------------------------------------------------
TEST (TrimTest, TestPredicatedNormalTrim) { 

	String s = FromStatic("@\t@\t foo bar@ @*\n");
	Trim(s, [](char c) {
		return c == '@' || c == '\t' || c == '*' || c == '\n';
	});

	ASSERT_EQ(s, String(FromStatic(" foo bar@ ")));
}
