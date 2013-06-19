cutecat
=======


The C++ string library that all the cute kittens use. It is highly efficient, uses nice syntax, 
is `std::string`-compatible and has all the string tools you've always missed in C++.

Slices
-------

With `std::string`, inserting, replacing or adding to strings is as with all `std::` containers
a bit tedious. _cutecat_ uses slices to enable insertion, erasure and substring operation. To quickly refer
to positions relative to the end of the string, negative indices (think Python) are used. 

Slices can be used almost interchangeable with strings.
````c++
using cutecat::String;
using cutecat::FromRaw;

String st = FromRaw("apple banana"); 

// replace the space by a colon
st.set(3,4) = ":";
// st is now "apple:banana"

// prepend "cherry:"
st.set(0,0) = "cherry:";

// append ":peach"
st.set(-1,-1) = ":peach";
// alt: st &= ":peach";

// remove apply by substituting the empty string
st.set(8,14) = "";

// or lets just make a new string by putting together some (overlapping) slices
String result = st.get(1,6) & st.get(2,7) & st.get(0, -1);

````

Operations: Trim, Split, Join, Find, Format, Parse
-------

_cutecat_ contains a library of useful string operations, almost all of which are unnecessarily hard to do
with the STL. Many of these algorithms operate much faster than their STL counterparts as they enable the
use of string slices where the STL would have required full substrings. cutecat operations are generally 
free functions and found via ADL.
````c++
using cutecat::String;

String st = GetSomeString();

// split at commas
std::vector<MutableStringSlice> parts;
Split(st, ',', parts); 

// for all parts: remove whitespace from both sides 
for(auto& slice : parts) {
   Trim(slice); 
}

// now join them again, using a semicolon as separator
String result = Join("; ", parts);


````


Copy-On-Write for static strings
-------

With `std::string`, it is oftentimes a hurdle that string literals, which are in static storage and 
assuming sane programmers will not be mutated for the entire duration of the program, are needlessly
copied as to please APIs demanding such thing. With _cutecat_, static strings are explicitly annotated and 
copied the first time they are being written to.
````c++
using cutecat::String;
using cutecat::FromStatic;

String st = FromStatic("lorem ipsum ..."); // no copy yet
st.set(0) = 'L'; // this does a copy 

````


````c++
using cutecat::String;

 
// 1) efficient (lazy) string concatenation using expression templates
String s1  = "3535" <cat> ' ' <cat> 12; // type StringExpression
 
 
// 2) lightweight printf
int someInt = 2;
String s2 = s1 <cat> Fill(" {0} -- {1}, {2}*" 
  &0= "Alexander Gessler" 
  &1= "my-email@my-hoster.net"
	&2= someInt);
 
// 3) lightweight scanf
String name, email;
if(Extract("{0} -- {1}" &0=name &1=email)) {
}
 
 
// 4) slice indexing and deletion
s1[1 <to> 3] = "blubb"; // type StringSlice
s2[1 <to> -2] = "blubb";
s1[0 <to> 3] = ""; // no re-allocation for this
 
 
// 5) zero-overhead for static string literals
constexpr String stack = "sss";
String s = stack[1 <to> 2]; // still no copy
s[0] = 2; // now a copy
 
// 6) std::string-compatible interface (complete source compatibility)
String::size_type s = stack.find_first_of(0);
 
// 7) much cooler library functions:
Trim()
TrimLeft()
TrimRight()
ForEach(s1, "blubb", [] (String sl) {
})
 
std::vector<String> s;
Split(',', s, Trim) // automatically reserves
Join(', ', s);
 
// 8) can be converted to std::string (this has overhead, though)
std::string ss = s;
