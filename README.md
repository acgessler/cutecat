cutecat
=======


The C++ string library that all the cute kittens use. It is highly efficient, uses nice syntax, 
is `std::string`-compatible and has all the string tools you've always missed in C++.

Slices
-------
_(done)_

With `std::string`, inserting, replacing or adding to strings is as with all `std::` containers
a bit tedious. _cutecat_ uses slices to enable insertion, erasure and substring operation. To quickly refer
to positions relative to the end of the string, negative indices (think Python) are used. 

Slices can be used almost interchangeable with strings.
````c++
using cutecat::String;
using cutecat::FromRaw;
using cutecat::Back;

String st = FromRaw("apple banana"); 

// replace the space by a colon
st(3,4) <= ":";
// st is now "apple:banana"

// prepend "cherry:"
st(0,0) <= "cherry:";


// append ":peach"
st(Back(0)) <= ":peach";
// without slices: st = st <cat> ":peach";

// remove "apple:" by substituting the empty string
st(8,14) <= "";

// or lets just make a new string by putting together some (overlapping) slices
String result = st(1,6) <cat> st(2,7) <cat> st(0, Back(1));

````

Lazy Concatenation
-------

_(not done)_

String concatenation with _cutecat_ uses expression templates to optimize concatenations. String operations
can thus be expressed  in the simplest possible way without inducing unnecessary copies. 

Slices can be used almost interchangeable with strings.
````c++
using cutecat::String;
using cutecat::FromRaw;

// unnecessary copies are elided and all concatenations done in one batch
String ar = GetSomeString() <cat> ", Price: " <cat> 24 <cat> " USD"; // TODO

String st = FromRaw("peach:cherry");

// the following induces no extra copies at all
st = "apple:" <cat> st;
st = st <cat> ":banana";

````

Operations: Trim, Split, Join, Find
-------

_(partly done)_

_cutecat_ contains a library of useful string operations, almost all of which are unnecessarily hard to do
with the STL. Many of these algorithms operate much faster than their STL counterparts as they enable the
use of string slices where the STL would have required full substrings. cutecat operations are generally 
free functions and found via ADL.
````c++
using cutecat::String;

String st = GetSomeString();

// split at commas
std::vector<StringSlice> parts;
Split(st, ',', parts); 

// for all parts: remove whitespace from both sides 
// (this is done by simply changing the slice window)
for(auto& slice : parts) {
   slice = TrimSlice(slice); 
}

// now join them again, using a semicolon as separator
String result = Join("; ", parts);
````

Operations: Format, Parse
-------

_(not done)_

Type-safe `printf` and `scanf` replacements. A bit like `boost.format`,  yet more lightweight.


````
using cutecat::String;
using cutecat::Format;


String s2 = Format(" {0} -- {1}, Age: {2}" 
  &= "Some Name" 
  &= "my-email@my-hoster.net"
  &= 23);
// s2 == "Some Name -- my-email@my-hoster.net, Age: 23"
 

String name, email;
int age;
if(Extract("{0} -- {1}, Age: {2}" &=name &=email &=age)) {
 
}
````


Copy-On-Write for static strings
-------

_(done)_

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

std::string adapter
-------

TODO
