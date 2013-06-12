#pragma once

/** 
cutecat - Highly efficient C++ string library
meowza! ^_^

Copyright (c) 2013, Alexander Christoph Gessler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

******************************************************************************

For more information, see https://github.com/acgessler/cutecat meow


Usage: TODO

*/

#ifndef INCLUDED_CUTECAT_H
#define INCLUDED_CUTECAT_H

namespace cutecat {

#if 0
	//----------------------------------------------------------------------------------------
	/** Gets the length of a string.
	 *
	 *  Length is in characters, not counting the terminating zero sentinel value.
	 *  @note TODO unicode
	 *  @param s[in] String instance */
	//----------------------------------------------------------------------------------------
	template <typename T>
	inline size_t Length(const BaseString<T>& s)
	{
		return s.length();
	}


	//----------------------------------------------------------------------------------------
	/** Checks if a string is empty.
	 *
	 *  A string s is empty iff Length(s) == 0.
	 *  @param s[in] String instance */
	//----------------------------------------------------------------------------------------
	template <typename T>
	inline bool Empty(const BaseString<T>& s)
	{
		return s.length() == 0;
	}
	

	//----------------------------------------------------------------------------------------
	/** 
	 *
	 *
	 *  @code
	 *
	 *  constexpr String f = "{0} and {1} go well together and 15+1 is {2:x}";
	 *  constexpr String input = "foo and bar go   well together and 15+1 is 0x10";
	 *
	 *  String s0, s1;
	 *  int i2;
	 *
	 *  const bool b = Parse(f, cutecat::ignoreSpace, s0, s1, 2);
	 *  // value of b: true
	 *
	 *  @endcode
	 */ 
	//----------------------------------------------------------------------------------------
	template <typename T, typename TFormatArgumentStaticType>
 	inline bool Parse(const BaseString<T>& base, const TFormatArgumentStaticType& args)
	{
		// TODO meow
		return false;
	}

	

	//----------------------------------------------------------------------------------------
	/** 
	 *
	 *
	 *  @code
	 *
	 *  constexpr String f = "{0} and {1} go well together and 15+1 is 0x{2:x}";
	 *
	 *  String filled = Format(f, "foo", "bar", 16);
	 *  // value of filled: "foo and bar go well together and 15+1 is 0x10"
	 *
	 *  @endcode
	 */ 
	//----------------------------------------------------------------------------------------
	template <typename T, typename TFormatArgumentStaticType>
 	inline StringExpression<T> Format(const BaseString<T>& base, const TFormatArgumentStaticType& args);

	
	//----------------------------------------------------------------------------------------
	/** Concatenates two string expressions. 
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template <typename TLeft, typename TRight>
	inline StaticStringExpression Cat(const TLeft& a, const TRight& b, 
		typename std::enable_if<
			detail::traits :: IsStringExpression<TLeft >::value &&
			detail::traits :: IsStringExpression<TRight>::value
		>::type* p = nullptr);

	template <typename T Args...>
	inline StaticStringExpression Cat();


	//----------------------------------------------------------------------------------------
	/** Trim a string from both sides by removing whitespace.
	 *
	 *  By default, only space characters, tab characters, backspaces and newlines (ASCII) are removed
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template <typename T>
	inline bool Trim(BaseString<T>& d)
	{
		const char* const sz = d.c_str(), *cur = sz;

		size_t left = 0u;
		while(*cur != 0) {
			if (*cur != ' ' && *cur != '\t' && *cur != '\n' && *cur != '\r') {
				break;
			}
			++cur;
			++left;
		}

		const std::size_t len = d.length();
		const char* const end = sz + len, *back_cur = end;

		size_t right = 0u;
		while(--back_cur > cur) {
			if (*cur != ' ' && *cur != '\t' && *cur != '\n' && *cur != '\r') {
				break;
			}
			++right;
		}

		assert(back_cur >= cur);
		if(left > 0 || right > 0) {
			d = d[Slice(left,len-right)]; 
			return true;
		}
		return false;
	}


	//----------------------------------------------------------------------------------------
	/** Trim a string from both sides by removing whitespace.
	 *  By default, this means only spaces are removed.
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template <typename T, TLambda>
	inline bool Trim(BaseString<T>& d, const TLambda& predicate, 
		// avoid use of std::function because it may be slower
#if CUTECAT_HAS_DECLTYPE
		typename std::enable_if<
			typename std::is_convertible<
				detail::traits :: function_traits<decltype(lambda)>::result_type, 
				bool
			>::value
		>::type* p = nullptr
#endif 
	) 
	{
		const char* const sz = d.c_str(), *cur = sz;

		size_t left = 0u;
		while(*cur != 0) {
			if (!predicate(*cur)) {
				break;
			}
			++cur;
			++left;
		}

		const std::size_t len = d.length();
		const char* const end = sz + len, *back_cur = end;

		size_t right = 0u;
		while(--back_cur > cur) {
			if (!predicate(*back_cur)) {
				break;
			}
			++right;
		}
		
		assert(back_cur >= cur);
		if(left > 0 || right > 0) {
			d = d[Slice(left,len-right)]; 
			return true;
		}
		return false;
	}


	void TrimLeft(String& d);
	void TrimRight(String& d);


	//----------------------------------------------------------------------------------------
	/** Substring search
	 *
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	void ForEachOccurence(String& d, const String& needle, std::function<void ()(StringSlice&)>)
	{
		// IDEA: use KMP algorithm for large values of |d| * |needle|
	}


	void ForEachOccurence(const String& d, const String& needle, std::function<void ()(const StringSlice&)>);

	// String splitting
	void Split(char split, const String& d, TContainer<String>& outp);
	void Split(const String& d, const String& d, TContainer<String>& outp);
	void Split(char split, const String& d, TContainer<StringSlice>& outp);
	void Split(const String& d, const String& d, TContainer<StringSlice>& outp);

	// String join
	StringExpression Join(char character, TContainer<ConvertibleToString>& inp);
	StringExpression Join(const String& d, TContainer<ConvertibleToString>& inp);
#endif // 0


	template<typename T> class BaseString;


	namespace detail {

		//----------------------------------------------------------------------------------------
		/** Static string base class
		 *
		 *  TODO
		 */
		//----------------------------------------------------------------------------------------
		template<typename T>
		class StaticBaseString 
		{
			friend class BaseString<T>;

		public:

			// TODO: get length from compiler
			StaticBaseString(const T* data)
				: data(data)
			{
				assert(data != nullptr);
			}

		private:
			const T* data;
		};


		//----------------------------------------------------------------------------------------
		/** Static string base class
		 *
		 *  TODO
		 */
		//----------------------------------------------------------------------------------------
		template<typename T, std::size_t n>
		class StaticBaseStringWithKnownLength
		{
			friend class BaseString<T>;

		public:

			// TODO: get length from compiler
			StaticBaseStringWithKnownLength(const T* data)
				: data(data)
			{
				assert(data != nullptr);
			}

		private:
			const T* data;
		};

		//----------------------------------------------------------------------------------------
		/** Dynamic string base class
		 *
		 *  TODO
		 */
		//----------------------------------------------------------------------------------------
		template<typename T>
		class DynamicBaseString 
		{
			friend class BaseString<T>;

		public:

			DynamicBaseString(const T* data)
				: data(data)
				, len(::strlen(data))
			{
				assert(data != nullptr);
			}


			DynamicBaseString(const T* data, size_t len)
				: data(data)
				, len(len)
			{
				assert(data != nullptr);
			}

		private:
			const T* data;
			const size_t len;
		};


	} // namespace detail


	//----------------------------------------------------------------------------------------
	/** Create a string source from a string pointer in static storage (i.e. a string
	 *  literal). String source objects can be passed to the #cutecat::BaseString constructor
	 *  to wrap them in a real string object.
	 *
	 *  @param data[in] Pointer to the first character of the string. The string must be
	 *                  zero-terminated.
	 *  @return A string source that can then be passed to a #cutecat::BaseString constructor
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
		detail::StaticBaseString< typename std::remove_const< typename std::remove_pointer<T>::type >::type > 
			FromStatic(const T data, 

		// this is necessary to explicitly disambiguate calls to the array versions -
		// if this was not here, arrays would implicitly decay to pointers and both
		// FromStatic() methods would then candidates of same rank.
		typename std::enable_if<
			std::is_pointer<T>::value
		>::type* = nullptr
	)
	{
		return detail::StaticBaseString<typename std::remove_const< typename std::remove_pointer<T>::type >::type>(data);
	}


	//----------------------------------------------------------------------------------------
	/** Create a string source from a string array in static storage (i.e. a string
	 *  literal). This overload evaluates the length of the argument at compile-time and
	 *  thus does not require a runtime call to get the length of the string.
	 *
	 *  @param data[in] Compile-time array containing a zero-terminated string.
	 *  @return A string source that can then be passed to a #cutecat::BaseString constructor
	 */
	//----------------------------------------------------------------------------------------
	template<typename T, std::size_t n>
	detail::StaticBaseStringWithKnownLength<T,n-1> FromStatic(const T (&data)[n])
	{
		assert(::strlen(data) == n-1);
		return detail::StaticBaseStringWithKnownLength<T,n-1>(data);
	}

	///{

	//----------------------------------------------------------------------------------------
	/** TODO
	 *
	 *  @param data[in] 
	 *  @return A string source that can then be passed to a #cutecat::BaseString constructor
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	detail::DynamicBaseString<T> FromRaw(const T* src)
	{
		return detail::DynamicBaseString<T>(src);
	}

	//----------------------------------------------------------------------------------------
	template<typename T>
	detail::DynamicBaseString<T> FromRaw(const T* src, std::size_t len)
	{
		return detail::DynamicBaseString<T>(src, len);
	}

	///}

	//----------------------------------------------------------------------------------------
	/** TODO
	 *
	 *  @param data[in] 
	 *  @return A string source that can then be passed to a #cutecat::BaseString constructor
	 */
	//----------------------------------------------------------------------------------------
	template<typename T, typename TTraits, typename TAlloc>
	detail::DynamicBaseString<T> FromStd(const std::basic_string<T, TTraits, TAlloc>& src)
	{
		return detail::DynamicBaseString<T>(src.c_str(), src.length());
	}




	//----------------------------------------------------------------------------------------
	/** Represents a slice (subset) of a string.
	 *
	 *  A slice is created TODO
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	class BaseStringSlice
	{
		friend class BaseString<T>;
		BaseStringSlice(T* data, T* data_end)
			: data(data)
			, data_end(end)
		{

		}

	public:

		T& operator[] (size_t index) {
			assert(data + index < data_end);
			return data[index];
		}

		const typename std::enable_if< !std::is_const<T>::value, T& 
			>::type operator[] (size_t index) const 
		{
			assert(data + index < data_end);
			return data[index];
		}

	public:

		// std::iterator_traits supplies the necessary iterator meta info for
		// raw pointers automatically
		T* begin() {
			return data;
		}

		T* end() {
			return data_end;
		}

	private:
		T* data;
		T* data_end;
	};



	// disable any padding
#if defined(_MSC_VER) ||  defined(__BORLANDC__) ||	defined (__BCPLUSPLUS__) 
#	pragma pack(push,1)
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error Compiler not supported
#endif


	//----------------------------------------------------------------------------------------
	/** String base class
	 *
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	class BaseString 
	{
	private:

		friend bool operator==(const BaseString& a, const BaseString& b);
		friend bool operator!=(const BaseString& a, const BaseString& b);

	private:

		
		enum {
				// string data lives in static storage and is assumed to be immutable
			  FLAG_STATIC   = 0x1
			, FLAG_UNUSED0	= 0x2
			, FLAG_UNUSED1	= 0x4
				// string data is internalized, intern.len gives the string length
			, FLAG_INTERN   = (sizeof(std::size_t) - sizeof(T)) * 8
		};


		T* data;

		union {
			struct external_storage_block {
				// raw storage capacity, does include terminal
				std::size_t capacity;		
				// does not include terminal
				std::size_t len;			
			} PACK_STRUCT ext;

			enum {
				// size of buffer for internalized strings, includes terminal 0
				  INTERNAL_BUFF_SIZE = (sizeof(external_storage_block) / sizeof(T)) - 1

				// maximum length of internalized strings
				, MAX_INTERNAL_LEN   = INTERNAL_BUFF_SIZE - 1
			};


			struct internalized_storage_block { 
				T buff[INTERNAL_BUFF_SIZE];
				T len; // TODO: make unsg and move to front
			} PACK_STRUCT intern;



			static_assert(sizeof(external_storage_block) == sizeof(internalized_storage_block), "TODO");
			static_assert(sizeof(external_storage_block) % sizeof(T) == 0, "TODO");
			static_assert( (1 << (sizeof(T)*8)) > MAX_INTERNAL_LEN, "TODO");

		} PACK_STRUCT;


		char flags;
	
	public:
		
		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString()
			: flags(FLAG_INTERN)
		{
			// the 0-string is internalized
			intern.len = 0;
			intern.buff[0] = 0;
			data = intern.buff;
		}
		

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString(const BaseString& other)
			: data(other.data)
			, flags(other.flags)
			, ext(other.ext)
		{
			if (flags & FLAG_INTERN) {
				data = &intern.buff;
			}
			else if ((flags & FLAG_STATIC) == 0) {
				return;
			}

			data = new char[other.ext.capacity];
			::strcpy(data, other.data); // TODO
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString(BaseString&& other)
			: data(other.data)
			, flags(other.flags)
			, ext(other.ext)
		{
			other.data = nullptr;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString(detail::StaticBaseString<T>& other)
			: data(const_cast<T*>(other.data))
			, flags(FLAG_STATIC)
		{
			ext.len = ::strlen(data);
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		template<std::size_t n>
		BaseString(detail::StaticBaseStringWithKnownLength<T,n>& other)
			: data(const_cast<T*>(other.data))
			, flags(FLAG_STATIC)
		{
			ext.len = n;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString(detail::DynamicBaseString<T>& other)
			: flags()
		{
			const size_t len = other.len;
			if(len <= MAX_INTERNAL_LEN) {
				intern.len = len;
				data = intern.buff;
				flags = FLAG_INTERN;
			}
			else {
				ext.len = len;
				ext.capacity = len + 1;
				data = new T[ext.capacity];
			}
			::strcpy(data, other.data);
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		explicit BaseString(const BaseStringSlice<T>& slice)
			: flags()
		{
			const size_t len = slice.length();
			if(len <= MAX_INTERNAL_LEN) {
				intern.len = len;
				data = intern.buff;
				flags = FLAG_INTERN;
				
			}
			else {
				// TODO: handle static here as well?
				ext.len = len;
				ext.capacity = len + 1;
				data = new T[ext.capacity];
			}
			::strcpy(data, slice.begin());
		}

	public:

		~BaseString()
		{
			if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
				delete[] data;
			}
		}

	public:


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString& operator= (const BaseStringSlice<T>& other)
		{
			const size_t len = other.length();
			if(len <= MAX_INTERNAL_LEN) {
				if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
					delete[] data;
				}

				data = intern.buff;
				flags = FLAG_INTERN;
				intern.len = len; // TODO proper cast
			}
			else {
				if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
					if (len > ext.len) {
						delete[] data;

						ext.capacity = len + 1;
						data = new T[ext.capacity];
					}
					assert(ext.capacity >= len + 1);
				}
				else {
					ext.capacity = len + 1;
					data = new T[ext.capacity];
				}
				ext.len = len;
				flags = 0;
			}
			::strcpy(data, other.begin());
			return *this;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString& operator= (const BaseString& other)
		{
			if(this == &other) {
				return *this;
			}
			if(other.flags & FLAG_STATIC) {

				if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
					if(other.ext.len <= ext.len) {
						// other is a static string, but we have enough 
						// space to store it, so we do a full copy.
						flags = 0;
						::strcpy(data, other.data);
						ext.len = other.ext.len;
						return *this;
					}
					delete[] data;
				}

				flags = FLAG_STATIC;
				data = other.data;
				ext.len = other.ext.len;
				return *this;
			}

			if(other.flags & FLAG_INTERN) {
				if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
					delete[] data;
				}
				data = intern.buff;
				intern = other.intern;
				flags = FLAG_INTERN;
				return;
			}

			ext = other.ext;
			data = new char[ext.capacity];
			::strcpy(data, other.data);	

			return *this;
		}

		// TODO: debug checks on moved strings

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString& operator= (BaseString&& other)
		{
			if(this == &other) {
				return *this;
			}
			if(other.flags & FLAG_STATIC) {

				if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
					if(other.ext.len <= ext.len) {
						// other is a static string, but we have enough 
						// space to store it, so we do a full copy.
						flags = 0;
						::strcpy(data, other.data);
						ext.len = other.ext.len;
						return *this;
					}
					delete[] data;
				}

				flags = FLAG_STATIC;
				data = other.data;
				ext.len = other.ext.len;
				return *this;
			}

			if(other.flags & FLAG_INTERN) {
				if ((flags & (FLAG_STATIC | FLAG_INTERN)) == 0) {
					delete[] data;
				}
				data = intern.buff;
				intern = other.intern;
				flags = FLAG_INTERN;
				return;
			}

			delete[] data;
			ext = other.ext;

			data = other.data;
			other.data = nullptr;
			return *this;
		}


	public:

		
		// ---------------------------------------------------------------------
		/** TODO */
		// ------------------------------- --------------------------------------
		BaseStringSlice<T> set(std::size_t begin, std::size_t end) {
			assert(begin < end && end <= length());

			if (flags & FLAG_STATIC) {
				// copy-on-write
				make_nonstatic();
			}
			return BaseStringSlice<T>(data + begin, data + end);
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseStringSlice<const T> get(std::size_t begin, std::size_t end) const {
			return BaseStringSlice<const T>(data + begin, data + end);
		} 


		BaseStringSlice<const T> operator()(std::size_t begin, std::size_t end) const {
			return BaseStringSlice<const T>(data + begin, data + end);
		} 

	public:


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T operator[] (std::size_t index) const {
			assert(index < length());
			return data[index];
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T& set(std::size_t index) {
			assert(index < length());

			if (flags & FLAG_STATIC) {
				 // copy-on-write
				make_nonstatic();
			}
			return data[index];
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T get(std::size_t index) const {
			return data[index];
		}


	public:


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		const T* get_array() const {
			return data;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T* get_writable_array() {
			if (flags & FLAG_STATIC) {
				// copy-on-write
				make_nonstatic();
			}
			return data;
		}

	public:

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		std::size_t length() const {
			// TODO: BigEndian
			return ext.len >> (flags & FLAG_INTERN);
		}

	private:

		void make_nonstatic() {
			assert(flags & FLAG_STATIC);

			const T* const src = data;
			const size_t len = ext.len;
			if(len <= MAX_INTERNAL_LEN) {
				data = intern.buff;
				::strcpy(data, src); // TODO

				flags = (flags & ~FLAG_STATIC) | FLAG_INTERN;
				intern.len = static_cast<T>(len);
				return;
			}

			ext.capacity = len + 1;
			data = new char[ext.capacity];
			::strcpy(data, src);
		}
	};

	// pop pack state
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop )
#endif
#undef PACK_STRUCT


	//----------------------------------------------------------------------------------------
	/** String base class
	 *
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	class BaseStringAdapter
	{
		 
	};


	//----------------------------------------------------------------------------------------
	/** String comparison
	 *
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	inline bool operator==(const BaseString<T>&  a, const BaseString<T>& b)
	{
		// do data comparison since getting the length takes extra bit fiddling
		return !::strcmp(a.data, b.data);
	}


	template<typename T>
	inline bool operator!=(const BaseString<T>& a, const BaseString<T>& b)
	{
		// do data comparison since getting the length takes extra bit fiddling
		return !!::strcmp(a.data, b.data);
	}


	/*
	bool operator==(const String& a, const StringSlice& b);
	bool operator!=(const String& a, const StringSlice& b);

	bool operator==(const StringSlice& a, const StringSlice& b);
	bool operator!=(const StringSlice& a, const StringSlice& b);
	*/

} // namespace cutecat


#endif // INCLUDED_CUTECAT_H

