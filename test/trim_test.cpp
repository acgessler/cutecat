
#include "gtest/gtest.h"

#include "../cutecat.h"

using cutecat::String;
using cutecat::FromRaw;
using cutecat::FromStatic;

//----------------------------------------------------------------------------------------
TEST (TrimTest, TestNormalTrim) { 

	String sbase = FromStatic("foo bar"), s = sbase;
	sbase <= Trim(sbase);
	ASSERT_EQ(sbase, s);

	s = FromStatic(" foo bar");
	s <= Trim(s);
	ASSERT_EQ(s, sbase);

	s = FromStatic("foo bar ");
	s <= Trim(s);
	ASSERT_EQ(s, sbase);
	
	s = FromStatic("    foo bar   ");
	s <= Trim(s);
	ASSERT_EQ(s, sbase);

	s = FromStatic(" \t \t  foo bar   \n");
	s <= Trim(s);
	ASSERT_EQ(s, sbase);

	s = String();
	s <= Trim(s);
	ASSERT_EQ(s, String());

	s = FromStatic("  \t\n  ");
	s <= Trim(s);
	ASSERT_EQ(s, String());

	s = FromStatic("  a  ");
	s <= Trim(s);
	ASSERT_EQ(s, String(FromStatic("a")));
}


//----------------------------------------------------------------------------------------
TEST (TrimTest, TestPredicatedNormalTrim) { 

	String s = FromStatic("@\t@\t foo bar@ @*\n");
	s <= Trim(s, [](char c) {
		return c == '@' || c == '\t' || c == '*' || c == '\n';
	});

	ASSERT_EQ(s, String(FromStatic(" foo bar@ ")));
}
