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

	// compiler-specific configuration
#ifdef _MSC_VER

#	pragma intrinsic(memcpy)
#	define CUTECAT_HAS_DECLTYPE

#else

#error "Compiler or platform not supported"

#endif



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


	namespace detail {
		template<typename TLeft, typename TRight, typename TResult> struct enable_if_same_or_const
		{
			typedef typename std::enable_if<
				std::is_same<
					typename std::remove_const<TLeft>::type,
					typename std::remove_const<TRight>::type
				>::value, 
			TResult>::type type;
		};
	}

	template<typename T>
	class BaseMutableSlice;


	//----------------------------------------------------------------------------------------
	/** Represents a string or slice index that starts from the back. Use Back() to construct
	 *  instances.
	 */
	//----------------------------------------------------------------------------------------
	class FromBack
	{
	public:

		FromBack(const std::size_t index)
			: index(index)
		{
		}

	public:

		operator std::size_t() const {
			return index;
		}

	private:

		const std::size_t index;
	};


	//----------------------------------------------------------------------------------------
	/** Represents a string or slice index that starts from the back.
	 *
	 *  @code
	 *  Back(0) // represents the character that is just behind the end of the slice
	 *  Back(1) // represents the last character
	 *  @endcode
	 */
	//----------------------------------------------------------------------------------------
	inline FromBack Back(const std::size_t index) {
		return FromBack(index);
	}


	//----------------------------------------------------------------------------------------
	/** Represents a slice of a string that cannot be modified at all.
	 *
	 *  String slices are for their lifetime bound to the string they pertain to. Their
	 *  behavior becomes undefined when the source string dies.
	 *
	 *  The <= operator is not available on mutable slices. operator[] is available,
	 *  but gives only by-value access. The (start, end)-slice operator is available on
	 *  immutable slices, but only returns equally immutable slices.
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	class BaseImmutableSlice
	{
		friend class BaseString<const T>;
		friend class BaseString<T>;

	public:

		BaseMutableSlice(const T* data, const T* data_end)
			: data		(const_cast<T*>(data))
			, data_end	(const_cast<T*>(data_end))
		{

		}

	public:

		// ---------------------------------------------------------------------
		/** Access an element of the slice by zero-based index. 
		 *  @param index Index in [0,length()) */
		// ---------------------------------------------------------------------
		T operator[] (size_t index) const {
			assert(data + index < data_end);
			return data[index];
		}

	public:

		// ---------------------------------------------------------------------
		/** Obtain an iterator to the begin of the data area for use in foreach loops.
		 *  @note Currently the 'iterator' is a raw pointer, but this should
		 *        not be relied upon. To obtain a raw pointer to the slice data,
		 *        use @code &*begin() @endcode. Note that the slice data is
		 *        usually not zero-terminated. */
		// ---------------------------------------------------------------------
		const T* begin() const {
			return data;
		}


		// ---------------------------------------------------------------------
		/** Obtain an iterator pointing right after the last element of the slice. */
		// ---------------------------------------------------------------------
		const T* end() const {
			return data_end;
		}


		// ---------------------------------------------------------------------
		/** Equivalent to #begin() */
		// ---------------------------------------------------------------------
		const T* cbegin() const {
			return data;
		}


		// ---------------------------------------------------------------------
		/** Equivalent to #end() */
		// ---------------------------------------------------------------------
		const T* cend() const {
			return data_end;
		}

	public:

		// ---------------------------------------------------------------------
		/** Checks whether this is a slice of another string or slice.
		 *  @param[in] base String or string slice 
		 *  @return true iff *this* was obtained from a slice operation on the
		 *          given slice. */
		// ---------------------------------------------------------------------
		template <template<typename> TStringType>
		bool is_slice_of(const TStringType<T>& base) const {
			return data >= base.cbegin() && data_end <= base.cend(); 
		}

	public:

		// ---------------------------------------------------------------------
		/** Gets the length of the slice, in characters. */
		// ---------------------------------------------------------------------
		std::size_t length() const {
			return static_cast<std::size_t>(data_end - data);
		}

	protected:
		/*const*/ T* data;
		/*const*/ T* data_end;
	};



	//----------------------------------------------------------------------------------------
	/** Represents a slice of a string that can either be treated as immutable, or a
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	class BaseMaybeMutableSlice : public BaseImmutableSlice<T>
	{
	public:	

		BaseMaybeMutableSlice(T* data, T* data_end, BaseString<T>& src)
			: BaseImmutableSlice<T>(data, data_end)
			, src(src)
		{

		}

	protected:

		BaseString<T>& src;
	};


	//----------------------------------------------------------------------------------------
	/** Represents a slice (subset) of a string.
	 *
	 *  A slice is created TODO
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	class BaseMutableSlice : public BaseMaybeMutableSlice
	{
		
	public:

		BaseMutableSlice(T* data, T* data_end, BaseString<T>& src)
			: BaseMaybeMutableSlice(data, data_end, src)
		{

		}

	public:

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseMutableSlice(const BaseMaybeMutableSlice<T>&& other)
			: src(other.src)
			, data(other.data)
			, data_end(other.data_end)
		{
			if(src.flags & BaseString<T>::FLAG_STATIC) {
				src._make_nonstatic();
			}
			data		= src.data + other.starti;
			data_end	= src.data + other.endi;
		}

	public:

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseMutableSlice& operator <= (const BaseMutableSlice<T>& other)
		{
			if(other.src == src) {
				src._sub<true>(data, data_end, other.begin(), other.end());
			}
			else {
				src._sub<false>(data, data_end, other.begin(), other.end());
			}
			return *this;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseMutableSlice& operator <=(const BaseImmutableSlice& other)
		{
			const T* obegin = other.cbegin(), *oend = other.cend();
			if(obegin >= data && oend <= data_end) {
				src._sub<true>(data, data_end, obegin, oend);
			}
			else {
				src._sub<false>(data, data_end, obegin, oend);
			}
			return *this;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseMutableSlice& operator <=(const BaseString<T>& other)
		{
			if(other == src) {
				src._sub<true>(data, data_end, other.cbegin(), other.cend());
			}
			else {
				src._sub<false>(data, data_end, other.cbegin(), other.cend());
			}
			return *this;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseMutableSlice& operator <= (T fill)
		{
			// handle the most common case when assigning a character
			// a slice of size 1 (i.e. character substitution)
			if(data+1 == data_end) {
				*data = fill;
				return *this;
			}
			src._sub<false>(data, data_end, &fill, &fill+1);
			return *this;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseMutableSlice& operator <= (const T* fill)
		{
			assert(fill != nullptr);

			const T* fill_end = fill;
			while(*fill_end) ++fill_end;

			if(fill >= data && fill_end <= data_end) {
				src._sub<true>(data, data_end, fill, fill_end);
			}
			else {
				src._sub<false>(data, data_end, fill, fill_end);
			}

			return *this;
		}

	public:

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T operator[] (size_t index) const {
			assert(data + index < data_end);
			return data[index];
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T& operator[] (size_t index) {
			assert(data + index < data_end);
			return data[index];
		}


	public:

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		bool is_slice_of(const BaseString<T>& other) const {
			return src == other; // TODO: check for stale data etc
		}

	public:


		using BaseImmutableSlice<T>::begin;
		using BaseImmutableSlice<T>::end;


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T* begin() {
			return data;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T* end() {
			return data_end;
		}
	};



	template <typename T>
	BaseMutableSlice<T>& operator <=(BaseMaybeMutableSlice<T>&& self, const BaseMutableSlice<T>& other)
	{
		return BaseMutableSlice<T>(std::move(self)) <= other;
	}


	template <typename T>
	BaseMutableSlice<T>& operator <=(BaseMaybeMutableSlice<T>&& self, const BaseImmutableSlice& other)
	{
		return BaseMutableSlice<T>(std::move(self)) <= other;
	}


	template <typename T>
	BaseMutableSlice<T>& operator <=(BaseMaybeMutableSlice<T>&& self, BaseMaybeMutableSlice<T>&& other)
	{
		return BaseMutableSlice<T>(std::move(self)) <= BaseImmutableSlice(std::move(other));
	}


	template <typename T>
	BaseMutableSlice<T>& operator <=(BaseMaybeMutableSlice<T>&& self, const BaseString<T>& other)
	{
		return BaseMutableSlice<T>(std::move(self)) <= other;
	}


	template <typename T>
	BaseMutableSlice<T>& operator <=(BaseMutableSlice<T>& self, BaseMaybeMutableSlice<T>&& other)
	{
		return self <= BaseImmutableSlice(std::move(other));
	}


	template <typename T>
	BaseMutableSlice<T>& operator <= (BaseMaybeMutableSlice<T>&& self, T fill)
	{
		return BaseMutableSlice<T>(std::move(self)) <= fill;
	}


	template <typename T>
	BaseMutableSlice<T>& operator <= (BaseMaybeMutableSlice<T>&& self, const T* fill)
	{
		return BaseMutableSlice<T>(std::move(self)) <= fill;
	}



	

	namespace detail {

		template<bool expect_overlapping>
		__forceinline void fast_copy(void* d, const void* s, std::size_t l)
		{
			::memcpy(d,s,l);
		}


		template<>
		__forceinline void fast_copy<true>(void* d, const void* s, std::size_t l)
		{
			::memmove(d,s,l);
		}
	}



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
	template<typename T /*, planned
		typename TAllocationHandler	= detail::StandardAllocationHandler,
		typename TGrowPolicy		= detail::ExponentialGrowthPolicy,
	*/ > 
	class BaseString 
	{
	private:

		friend class BaseMutableSlice<T>;

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
		mutable unsigned int flags;

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
				data = intern.buff;
				return;
			}
			else if (flags & FLAG_STATIC) {
				return;
			}

			data = new T[other.ext.capacity];
			::strcpy(data, other.data); 
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
		BaseString(const detail::StaticBaseString<T>& other)
			: data(const_cast<T*>(other.data))
			, flags(FLAG_STATIC)
		{
			ext.len = ::strlen(data);
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		template<std::size_t n>
		BaseString(const detail::StaticBaseStringWithKnownLength<T,n>& other)
			: data(const_cast<T*>(other.data))
			, flags(FLAG_STATIC)
		{
			ext.len = n;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		BaseString(const detail::DynamicBaseString<T>& other)
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
		explicit BaseString(const BaseImmutableSlice& slice)
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


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
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
		BaseString& operator <= (const BaseImmutableSlice& other)
		{
			const bool self_slice = other.is_slice_of(*this);

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

			if(self_slice) {
				detail::fast_copy<true>(data, other.cbegin(), len);
			}
			else {
				detail::fast_copy<false>(data, other.cbegin(), len);
			}

			// ensure zero-termination
			data[len] = 0;
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
				return *this;
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
				return *this;
			}

			delete[] data;
			ext = other.ext;

			data = other.data;
			other.data = nullptr;
			return *this;
		}


	public:
	
		// ---------------------------------------------------------------------
		BaseMaybeMutableSlice<T> operator()(std::size_t begini, std::size_t endi) {
			assert(begini <= endi && endi <= length());
			return BaseMaybeMutableSlice<T>(begini, endi, *this);
		}


		BaseImmutableSlice operator()(std::size_t begini, std::size_t endi) const {
			assert(begini <= endi && endi <= length());
			return BaseImmutableSlice(data + begini, data + endi);
		} 


		// ---------------------------------------------------------------------
		/** Obtains a slice of the string using backwards notation.
		 *
		 *  Usage:
		 *  @code
		 *  using cutecat::FromBack;
		 *
		 *  String st = ...;
		 *  st(FromBack(4), FromBack(1)) <= "abc";
		 *  @endcode
		 *
		 *  @param[in] begini	#cutecat::FromBack instance that specifies the first 
		 *                      character of the slice (inclusive).
		 *  @param[in] endi		#cutecat::FromBack instance that specifies the last 
		 *                      character of the slice (exclusive).
		 *  @return Slice (begini,endi]
		 */
		// ------------------------------- --------------------------------------
		BaseMaybeMutableSlice<T> operator()(const FromBack& begini, const FromBack& endi) {
			assert(_is_valid_slice(begini, endi));
			const std::size_t len = length();
			return BaseMaybeMutableSlice<T>(
				data + len - begini,
				data + len - endi,
				*this
			);
		}


		BaseImmutableSlice operator()(const FromBack& begini, const FromBack& endi) const {
			assert(_is_valid_slice(begini, endi));

			// TODO: keep data end pointer?
			const std::size_t len = length();
			return BaseImmutableSlice(
				data + len - begini,
				data + len - endi
			);
		} 


		// ------------------------------- --------------------------------------
		BaseMaybeMutableSlice<T> operator()(const FromBack& begini, std::size_t endi) {
			assert(_is_valid_slice(begini, endi));
			return BaseMaybeMutableSlice<T>(data + length() - begini, data + endi, *this);
		}


		BaseImmutableSlice operator()(const FromBack& begini, std::size_t endi) const {
			assert(_is_valid_slice(begini, endi));
			return BaseImmutableSlice(cend() - begini, data + endi);
		} 


		// ------------------------------- --------------------------------------
		BaseMaybeMutableSlice<T> operator()(std::size_t begini, const FromBack& endi) {
			assert(_is_valid_slice(begini, endi));
			return BaseMaybeMutableSlice<T>(data + begini, data +  length() - endi, *this);
		}


		BaseImmutableSlice operator()(std::size_t begini, const FromBack& endi) const {
			assert(_is_valid_slice(begini, endi));
			return BaseImmutableSlice(data + begini, cend() - endi);
		} 


		// ---------------------------------------------------------------------
		/** TODO */
		// ------------------------------- --------------------------------------
		BaseMaybeMutableSlice<T> operator()(std::size_t index) {
			assert(_is_valid_slice(index,index));
			return BaseMaybeMutableSlice<T>(data + index,  data + index, *this);
		}


		BaseImmutableSlice operator()(std::size_t index) const {
			assert(_is_valid_slice(index,index));
			return BaseImmutableSlice(data + index, data + index);
		} 


		BaseMaybeMutableSlice<T> operator()(const FromBack& index) {
			assert(_is_valid_slice(index,index));
			const std::size_t i = length() - index;
			return BaseMaybeMutableSlice<T>(data + i,data + i);
		}


		BaseImmutableSlice operator()(const FromBack& index) const {
			assert(_is_valid_slice(index,index));
			return BaseImmutableSlice(cend() - index, cend() - index);
		} 


	public:

		
		// ---------------------------------------------------------------------
		/** TODO */
		// ------------------------------- --------------------------------------
		template<typename TFirst, typename TSecond>
		BaseMutableSlice<T> set(TFirst begini, TSecond endi) {
			return BaseMutableSlice<T>((*this)(begini, endi));
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		template<typename TFirst, typename TSecond>
		BaseImmutableSlice get(TFirst begini, TSecond endi) const {
			return BaseImmutableSlice((*this)(begini, endi));
		} 


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		template<typename TFirst>
		BaseMutableSlice<T> set(TFirst index) {
			return BaseMutableSlice<T>((*this)(index));
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		template<typename TFirst>
		BaseImmutableSlice get(TFirst index) const {
			return BaseImmutableSlice((*this)(index));
		}


	public:

		// std::iterator_traits<T> supplies the necessary iterator meta info for
		// raw pointers automatically

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T* begin() {
			if(flags & FLAG_STATIC) {
				_make_nonstatic();
			}
			return data;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T* end() {
			if(flags & FLAG_STATIC) {
				_make_nonstatic();
			}
			return data + length();
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		const T* begin() const {
			return data;
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		const T* end() const {
			return data + length();
		}


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		const T* cbegin() const {
			return data;
		}

		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		const T* cend() const {
			return data + length();
		}

	public:


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		T operator[] (std::size_t index) const {
			assert(index < length());
			return data[index];
		}


		T operator[] (const FromBack& index) const {
			assert(index > 0 && index <= length());
			return cend()[-index];
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
			if (flags & FLAG_STATIC) { // copy-on-write
				_make_nonstatic();
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


#ifdef _DEBUG

		// ---------------------------------------------------------------------
		/** Debug checks */
		// ---------------------------------------------------------------------
		bool _is_valid_slice(std::size_t begini, std::size_t endi) const {
			return begini <= endi && endi <= length();
		}


		bool _is_valid_slice(const FromBack& begini_tag, std::size_t endi) const {
			const std::size_t begini = length() - begini_tag;
			return begini <= endi && endi <= length();
		}

		bool _is_valid_slice(std::size_t begini, const FromBack& endi_tag) const {
			const std::size_t endi = length() - endi_tag;
			return begini <= endi && endi <= length();
		}

		bool _is_valid_slice(const FromBack& begini_tag, const FromBack& endi_tag) const {
			const std::size_t begini = length() - begini_tag;
			const std::size_t endi = length() - endi_tag;
			return begini <= endi && endi <= length();
		}

#endif


		// ---------------------------------------------------------------------
		/** TODO */
		// ---------------------------------------------------------------------
		void _make_nonstatic() {
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

			flags = (flags & ~FLAG_STATIC);

			ext.capacity = len + 1;
			data = new char[ext.capacity];
			::strcpy(data, src);
		}



		// ---------------------------------------------------------------------
		/** Substitute a given slice of the string by another.
		 *  @param[in,out] dest_begin_ref Begin of destination slice. Receives
		 *				the updated beginning in case the string is
		 *				being reallocated.
		 *  @param[in,out] dest_end_ref End of destination slice. Receives
		 *				the updated end pointer in case the string is
		 *				being reallocated.
		 *  @param[in]	src_begin Begin of source slice.
		 *  @param[in]	src_end End of source slice.
		 */
		// ---------------------------------------------------------------------
		template<bool expect_overlapping>
		void _sub(T*& dest_begin_ref, T*& dest_end_ref, const T* src_begin, const T* src_end)	{
			// dereference parameters to keep the compiler from doing that on every access
			T* dest_begin = dest_begin_ref;
			T* dest_end = dest_end_ref;

			// static strings should have been made non-static upon obtaining a slice
			assert(!(flags & FLAG_STATIC));

			// ranges must be well-defined 
			assert(dest_begin <= dest_end && data <= dest_begin && dest_end <= data + length());
			assert(src_begin <= src_end);

			// TODO: handle overlapping slices
			const std::size_t my_len = static_cast<std::size_t>(dest_end - dest_begin);
			const std::size_t other_len = static_cast<std::size_t>(src_end - src_begin);

			if(my_len == other_len) {
				detail::fast_copy<expect_overlapping>(dest_begin, src_begin, my_len);
				return;
			}

			const std::size_t full_len = length();
			const std::ptrdiff_t diff = other_len - my_len;

			const std::size_t dest_ofs_end = full_len - static_cast<std::size_t>(dest_end - data);

			if(my_len < other_len) {
				// the slice to be assigned is longer than the current piece
				const std::size_t dest_ofs = static_cast<std::size_t>(dest_begin - data);
				const std::size_t new_len = full_len + diff;

				if (((flags & FLAG_INTERN) && new_len > MAX_INTERNAL_LEN) || new_len >= ext.capacity) {

					// note: until all data copying is complete, we cannot access ext
					// since it may overlap with the internalized string.

					// allocate heap
					// TODO: try a realloc if possible!
					const std::size_t cap = new_len + 1;  // todo: more capacity
					T* const new_data = new char[cap];

					T* cursor = new_data;
					detail::fast_copy<false>(cursor, data, dest_ofs);

					cursor += dest_ofs;
					detail::fast_copy<false>(cursor, src_begin, other_len);

					cursor += other_len;
					detail::fast_copy<false>(cursor, dest_end, dest_ofs_end);

					// ensure zero-termination
					cursor[dest_ofs_end] = 0;

					if (flags & FLAG_INTERN) {
						flags &= ~FLAG_INTERN;
					}
					else {
						delete[] data;
					}

					ext.capacity = cap; 
					ext.len = new_len;

					data = new_data;
					dest_begin = data + dest_ofs;

					// update caller's refs
					dest_begin_ref = dest_begin;
					dest_end_ref = dest_begin + other_len;
					return;
				}
				// else: fall through	
			}

			// the slice to be assigned is shorter than the current piece OR
			// it is longer, but there is enough storage. Therefore, we
			// can leave the first part of the string as is and move the
			// last part to its final position and then finally insert the
			// new slice.
			detail::fast_copy<expect_overlapping>(dest_begin + other_len, dest_end, dest_ofs_end);
			detail::fast_copy<expect_overlapping>(dest_begin, src_begin, other_len);

			// ensure zero-termination
			dest_begin[other_len + dest_ofs_end] = 0;

			// update caller's refs
			dest_end_ref = dest_begin + other_len;
			// dest_begin remains untouched

			if (flags & FLAG_INTERN) {
				intern.len += diff;
			}
			else {
				ext.len += diff;
			}
		}
	};

	// pop pack state
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop )
#endif
#undef PACK_STRUCT


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
		return !::strcmp(a.get_array(), b.get_array());
	}


	template<typename TLeft, typename TRight> 
	inline bool operator!=(const TLeft& a, const TRight& b)
	{
		// ADL should find this
		return !(a == b);
	}


	//----------------------------------------------------------------------------------------
	/** String comparison
	 *
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	
	template<template<typename> TLeft, template<typename> TRight, typename T>
	inline bool operator==(const TLeft<T>& b, const TRight<T>& a) 
	{
		const std::size_t len = a.length();
		if(len != b.length()) {
			return false;
		}
		return !::memcmp(a.cbegin(), b.cbegin(), len);
	}


	template<typename T, template<typename> TStringOrSliceType>
	inline bool operator==(const T* cstr, const TStringOrSliceType<T>& a)
	{
		const std::size_t len = a.length();
		if(len != ::strlen(cstr)) {
			return false;
		}
		return !::memcmp(a.cbegin(), cstr, len);
	}


	template<typename T, template<typename> TStringOrSliceType>
	inline bool operator==(const TStringOrSliceType<T>& a, const T* cstr)
	{
		const std::size_t len = a.length();
		if(len != ::strlen(cstr)) {
			return false;
		}
		return !::memcmp(a.cbegin(), cstr, len);
	}


	//----------------------------------------------------------------------------------------
	/** Gets the length of a string.
	 *
	 *  Length is in characters, not counting the terminating zero sentinel value.
	 *  @note TODO unicode
	 *  @param s[in] String instance */
	//----------------------------------------------------------------------------------------
	template<typename T, template<typename> TStringOrSliceType>
	inline size_t Length(const TStringOrSliceType<T>& s)
	{
		return s.length();
	}


	//----------------------------------------------------------------------------------------
	/** Checks if a string or slice is empty.
	 *
	 *  A string s is empty iff Length(s) == 0.
	 *  @param s[in] String or slice instance */
	//----------------------------------------------------------------------------------------
	template<typename T, template<typename> TStringOrSliceType>
	inline bool Empty(const TStringOrSliceType<T>& s)
	{
		return s.length() == 0;
	}


#if 0
	//----------------------------------------------------------------------------------------
	/** Checks if a string is empty.
	 *
	 *  A string s is empty iff Length(s) == 0.
	 *  @param s[in] String instance */
	//----------------------------------------------------------------------------------------
	template <typename T>
	inline void Fill(T& s, typename T::value_type fill)
	{
		for(const char* c = s.begin(), const * const e = s.end(); c != e; *c++ = fill);
	}
#endif	
	

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
 	inline bool Parse(const BaseString<T>& base, const TFormatArgumentStaticType& args);

#if 0

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
#endif


	//----------------------------------------------------------------------------------------
	/** CharacterTraits encapsulates traits of the raw characters stored in a BaseString.
	 *
	 *  It does not know about encodings and is therefore for example not able to tell
	 *  whether a character is printable or not - for this, code points traits are used,
	 *  which then require an encoding to be specified.
	 */
	//----------------------------------------------------------------------------------------
	template<typename T>
	struct CharacterTraits {

	};

	template<>
	struct CharacterTraits<char> : std::numeric_limits<char> {
		
		// These character traits do not depend on the character set being used
		// as the C++ compiler automatically encodes the escapes accordingly.
		static const char tab = '\t';
		static const char linefeed = '\r';
		static const char newline = '\n';
		static const char space = ' ';
	};

	
	//----------------------------------------------------------------------------------------
	/** Trim a string or slice from both sides by removing whitespace. #cutecat::CharacterTraits's 
	 *  tab, linefeed, newline and space characters are all considered whitespace.
	 *
	 *  Use the predicated version to specify a different character set.
	 *  @return 
	 */
	//----------------------------------------------------------------------------------------
	template<typename T, template<typename> TStringOrSliceType>
	inline BaseImmutableSlice Trim(const TStringOrSliceType<T>& d)
	{
		const char* const sz = d.cbegin(), *cur = sz;

		const std::size_t len = d.length();
		const char* const end = sz + len, *back_cur = end;

		size_t left = 0u;
		while(cur != != end) {
			if (*cur != CharacterTraits<T>::space		&& 
				*cur != CharacterTraits<T>::tab			&&
				*cur != CharacterTraits<T>::newline		&& 
				*cur != CharacterTraits<T>::linefeed) {
				break;
			}
			++cur;
			++left;
		}

		std::size_t right = 0u;
		while(--back_cur > cur) {
			if (*back_cur != CharacterTraits<T>::space		&& 
				*back_cur != CharacterTraits<T>::tab		&&
				*back_cur != CharacterTraits<T>::newline	&& 
				*back_cur != CharacterTraits<T>::linefeed) {
				break;
			}
			++right;
		}

		return d(left,len-right); 
	}


	//----------------------------------------------------------------------------------------
	/** Trim a string from both sides by removing whitespace.
	 *  By default, this means only spaces are removed.
	 *  TODO
	 */
	//----------------------------------------------------------------------------------------
	template <typename T, template<typename> TStringOrSliceType, typename TLambda>
	inline BaseImmutableSlice Trim(const TStringOrSliceType<T>& d, TLambda predicate) 
	{
		const char* const sz = d.cbegin(), *cur = sz;

		const std::size_t len = d.length();
		const char* const end = sz + len, *back_cur = end;

		size_t left = 0u;
		while(cur != end) {
			if (!predicate(*cur)) {
				break;
			}
			++cur;
			++left;
		}

		size_t right = 0u;
		while(--back_cur > cur) {
			if (!predicate(*back_cur)) {
				break;
			}
			++right;
		}
		
		return d(left,len-right); 
	}


	//void TrimLeft(String& d);
	//void TrimRight(String& d);

#if 0
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
#endif // 0
	
	// String splitting


	//----------------------------------------------------------------------------------------
	/** Split a given string at each occurrence of a split character.
	 *
	 *  The output is a sequence of slices into the input string.
	 *
	 *  @param split			Character to split input string at.
	 *  @param outp				std::back_inserter-like output iterator to receive output
	 *  @param merge_adjacent	If set to true, adjacent occurrences of the split characters
	 *							are treated as a single occurrence. If false, adjacent
	 *                          occurrences cause empty string slices to be generated.
	 *                          The same applies to split characters at the beginning or end of 
	 *                          the string, which then cause an empty slice to be generated.
	 *
	 *  @return The output iterator after the split operation.
	 */
	//----------------------------------------------------------------------------------------
	template <typename T, template<typename> TStringOrSliceType, typename TOutputIterator>
	TOutputIterator Split(T split, const TStringOrSliceType<T>& src, TOutputIterator outp, 
		bool merge_adjacent = true
	)
	{
		const T* data = src.cbegin(), const T* end = src.end();
		std::size_t idx = 0u, last = 0u, last_non_split = 0u;

		// merging of adjacent split characters
		if(merge_adjacent) {
			while(*data == split) {
				++last;
				++idx;
				++data;
			}

			while(data != end) {
				++idx;
				if(*data == split) {
					if(data[1] != split) {
						*outp++ = src.get(last, last_non_split);
						last = idx;
					}
				}
				else {
					last_non_split = idx;
				}
				++data;
			}

			if(last_non_split == idx && last < last_non_split) {
				*outp++ = src.get(last, FromBack(0));
			}

			return outp;
		}

		// no merging of adjacent split characters
		while(data != end) {
			++idx;
			if(*data == split) {
				*outp++ = src.get(last, last_non_split);
				last = idx;
			}
			last_non_split = idx;
			++data;
		}

		if(last_non_split == idx) {
			*outp++ = src.get(last, FromBack(0));
		}
		return outp;
	}

	struct PatternNotFound {};

	//----------------------------------------------------------------------------------------
	/** 
	 * Usage
	 *
	 * @code
	 * String s = ...
	 * // replace "Apple" by "Peach"
	 * try {
	 *		Find(s, "Apple") <= "Peach";
	 * } catch(PatternNotFound& ex) {
	 *
	 * }
	 *
	 * @endcode
	 */
	//----------------------------------------------------------------------------------------
	template<template<typename> THaystackType, template<typename> TNeedleType, typename T>
	typename SelectSliceType< THaystackType<T> >::type FindOrThrow(THaystackType<T>& haystack, const TNeedleType<T>& needle)
	{
		const std::size_t len = needle.length();

		const T* const haystack_begin = haystack.begin(), *const end = haystack.end() - len + 1;
		const T* const needle_begin = needle.begin();

		const std::size_t d = 0;
		for(const T* it = haystack_begin; it != end; ++it, ++d) {
			if(!::strncmp(needle_begin, it, len)) {
				return haystack(d, d + len);
			}
		} 
		throw PatternNotFound();
	}


	//----------------------------------------------------------------------------------------
	template<template<typename> THaystackType, template<typename> TNeedleType, typename T>
	typename SelectSliceType< THaystackType<T> >::type FindOrDefault(THaystackType<T>& haystack, const TNeedleType<T>& needle)
	{
		const std::size_t len = needle.length();

		const T* const haystack_begin = haystack.begin(), *const end = haystack.end() - len + 1;
		const T* const needle_begin = needle.begin();

		const std::size_t d = 0;
		for(const T* it = haystack_begin; it != end; ++it, ++d) {
			if(!::strncmp(needle_begin, it, len)) {
				return haystack(d, d + len);
			}
		} 
		// TODO
	}


#if 0
	void Split(const String& d, const String& d, TContainer<String>& outp);
	void Split(char split, const String& d, TContainer<StringSlice>& outp);
	void Split(const String& d, const String& d, TContainer<StringSlice>& outp);

	// String join
	StringExpression Join(char character, TContainer<ConvertibleToString>& inp);
	StringExpression Join(const String& d, TContainer<ConvertibleToString>& inp);
#endif // 0

	
	typedef BaseString<char> String;
	 
} // namespace cutecat


#endif // INCLUDED_CUTECAT_H

