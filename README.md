cutecat
=======

The C++ string library that all the cute kittens use. It is highly efficient, uses nice syntax, is std::string-compatible and has all the string tools you've always missed in C++.

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
