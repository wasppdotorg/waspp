/**
* @file msgpack.hpp
* @author  Arturo Blas Jimenez <arturoblas@gmail.com>
* @version 0.1
*
* @section LICENSE
*
* \GPLv3
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* \Apache 2.0
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
* an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
* specific language governing permissions and limitations under the License.
*
* @section DESCRIPTION
*
*  This header file provides a C++ implementation of the MessagePack
*  binary serialization protocol <http://http://msgpack.org/> relying on
*  an extensive usage of STL containers and stream classes.
*
*/

#ifndef _MSGPACK_LITE_HPP_
#define _MSGPACK_LITE_HPP_

#include <cstdint>

#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <bitset>
#include <cmath>
#include <typeinfo>
#include <locale>
#include <iostream>

#ifdef GLOBAL_NAMESPACE__
#define NAMESPACE_HEADER__ namespace GLOBAL_NAMESPACE__
#define NAMESPACE_PREFIX__ GLOBAL_NAMESPACE__::
#endif

#ifndef MSGPACK_NAMESPACE__
#define MSGPACK_NAMESPACE__ msgpack_lite
#endif

namespace MSGPACK_NAMESPACE__
{
	namespace bm
	{
		/*****************************************************
		* Type identifiers as specified by:
		* {@link http://wiki.msgpack.org/display/MSGPACK/Format+specification }
		* ***************************************************/

		/*****************************************************
		* Fixed length types
		*****************************************************/

		//! Integers
		static const uint8_t MP_INT8 = (uint8_t) 0xd0;
		static const uint8_t MP_INT16 = (uint8_t) 0xd1;
		static const uint8_t MP_INT32 = (uint8_t) 0xd2;
		static const uint8_t MP_INT64 = (uint8_t) 0xd3;
		static const uint8_t MP_UINT8 = (uint8_t) 0xcc;
		static const uint8_t MP_UINT16 = (uint8_t) 0xcd;
		static const uint8_t MP_UINT32 = (uint8_t) 0xce;
		static const uint8_t MP_UINT64 = (uint8_t) 0xcf;
		static const uint8_t MP_FIXNUM = (uint8_t) 0x00; //!< Last 7 bits is value
		static const uint8_t MP_NEGATIVE_FIXNUM = (uint8_t) 0xe0; //!< Last 5 bits is value

		//! nil
		static const uint8_t MP_NULL = (uint8_t) 0xc0;

		//! boolean
		static const uint8_t MP_FALSE = (uint8_t) 0xc2;
		static const uint8_t MP_TRUE = (uint8_t) 0xc3;

		//! Floating point
		static const uint8_t MP_FLOAT = (uint8_t) 0xca;
		static const uint8_t MP_DOUBLE = (uint8_t) 0xcb;

		/*****************************************************
		* Variable length types
		*****************************************************/

		//! Raw bytes
		static const uint8_t MP_RAW16 = (uint8_t) 0xda;
		static const uint8_t MP_RAW32 = (uint8_t) 0xdb;
		static const uint8_t MP_FIXRAW = (uint8_t) 0xa0; //!< Last 5 bits is size

		/*****************************************************
		* Container types
		*****************************************************/

		//! Arrays
		static const uint8_t MP_ARRAY16 = (uint8_t) 0xdc;
		static const uint8_t MP_ARRAY32 = (uint8_t) 0xdd;
		static const uint8_t MP_FIXARRAY = (uint8_t) 0x90; //<! Lst 4 bits is size

		//! Maps
		static const uint8_t MP_MAP16 = (uint8_t) 0xde;
		static const uint8_t MP_MAP32 = (uint8_t) 0xdf;
		static const uint8_t MP_FIXMAP = (uint8_t) 0x80; //<! Last 4 bits is size

		//! Some helper bitmasks
		static const uint32_t MAX_4BIT = 0xf;
		static const uint32_t MAX_5BIT = 0x1f;
		static const uint32_t MAX_7BIT = 0x7f;
		static const uint32_t MAX_8BIT = 0xff;
		static const uint32_t MAX_15BIT = 0x7fff;
		static const uint32_t MAX_16BIT = 0xffff;
		static const uint32_t MAX_31BIT = 0x7fffffff;
		static const uint32_t MAX_32BIT = 0xffffffff;

	} //namespace bm

	// Forward declarations
	class Packer;
	class Unpacker;

	/**
	* This interface allows to extend any type to
	* make it serializable using the MsgPack protocol
	*/
	class Parcelable
	{
	protected:

		friend class Packer;
		friend class Unpacker;

		/**
		* Packs the current object using the provided
		* Packer reference
		*/
		virtual void pack(Packer&) const = 0;

		/**
		* Unpacks the current object using the provided
		* Unpacker reference
		*/
		virtual void unpack(Unpacker& ) = 0;

	}; // Parcelable

	/**
	* Packer class. Provides the functionality to serialize data
	* into a stream using the MessagePack binary data.
	*/
	class Packer
	{
	public:

		/**
		* Constructor
		* @param out Packer output stream where the binary data will be put
		*/
		explicit Packer(std::ostream& out) : out_(out) {}

		/**
		* This method allows to pack an object of any type
		* which implements the Parcelable interface.
		* @param p Reference to an object to be packed
		*/
		Packer& pack(const Parcelable& p)
		{
			p.pack(*this);
			return *this;
		}

		/**
		* Packs any data type that can be casted into an int64_t
		* for which there's not any overloaded pack method.
		* @param item Data element to be packed.
		*/
		template<typename T> inline
			Packer& pack(T item)
		{
			return pack((int64_t) item);
		}

		/**
		* Packs using a pointer as the data source. If the pointer is null
		* a null object will be serialized into the stream, if not,
		* the contents of the pointer will be serialized.
		* @param item Pointer to the data to be serialized.
		*/
		template<typename T> inline
			Packer& pack(const T* item)
		{
			return (item == 0) ? write(bm::MP_NULL) : pack(*item);
		}


		/**
		* Packs using a C string as a source of data. If the pointer is null
		* a null object will be serialized into the stream, if not,
		* the contents of the array will be serialized using the lenght of
		* the string provided by strlen(). Note that this is not suitable for
		* serializing raw regions of memory, in order to do so,
		* {@see pack(const char*, std::size_t) } should be used instead.
		* @param item Pointer to the C string to be serialized.
		*/
		Packer& pack(const char* item)
		{
			return (item == 0) ? write(bm::MP_NULL) : pack(item, strlen(item));
		}

		/**
		* Packs using a wide char C string as a source of data. If the pointer is null
		* a null object will be serialized into the stream, if not,
		* the contents of the array will be serialized using the lenght of
		* the string provided by wcslen().
		* @param item Pointer to the wide char C string to be serialized.
		*/
		Packer& pack(const wchar_t* item)
		{
			return (item == 0) ? write(bm::MP_NULL) :
				pack((char*)item, wcslen(item)*sizeof(wchar_t));
		}

		/**
		* Pack a boolean value
		* @param item Value to be serialized
		*/
		Packer& pack(bool item)
		{
			return (item ? write(bm::MP_TRUE) : write(bm::MP_FALSE));
		}

		/**
		* Pack a long integer value
		* @param value Value to be serialized
		*/
		Packer& pack(const int64_t& value)
		{
			if (value >= 0)
			{
				if (value <= bm::MAX_7BIT)
				{
					write<int8_t>((int8_t(value) | bm::MP_FIXNUM));
				}
				else if (value <= bm::MAX_8BIT)
				{
					write(bm::MP_UINT8).write<int8_t>(value);
				}
				else if (value <= bm::MAX_16BIT)
				{
					write(bm::MP_UINT16).write<uint16_t>(value);
				}
				else if (value <= bm::MAX_32BIT)
				{
					write(bm::MP_UINT32).write<uint32_t>(value);
				}
				else
				{
					write(bm::MP_UINT64).write<uint64_t>(value);
				}
			}
			else
			{
				if (value >= -(bm::MAX_5BIT + 1))
				{
					write<int8_t>(int8_t(value) | bm::MP_NEGATIVE_FIXNUM);
				}
				else if (value >= -(bm::MAX_7BIT + 1))
				{
					write(bm::MP_INT8).write<int8_t>(value);
				}
				else if (value >= -(bm::MAX_15BIT + 1))
				{
					write(bm::MP_INT16).write<int16_t>(value);
				}
				else if (value >= -(int64_t(bm::MAX_31BIT) + 1))
				{
					write(bm::MP_INT32).write<int32_t>(value);
				}
				else
				{
					write(bm::MP_INT64).write<int64_t>(value);
				}
			}
			return *this;
		}

		/**
		* Pack a single precision floating point value
		* @param item Value to be serialized
		*/
		Packer& pack(float item)
		{
			return write(bm::MP_FLOAT).write(item);
		}

		/**
		* Pack a double precision floating point value
		* @param item Value to be serialized
		*/
		Packer& pack(double item)
		{
			return write(bm::MP_DOUBLE).write(item);
		}

		/**
		* Pack a generic C++ string object.
		* @param item Reference to the string to be packed
		*/
		template<class T>
		Packer& pack(const std::basic_string<T>& value)
		{
			return pack(value.c_str());
		}

		/**
		* Pack a raw memory area given a pointer to it and
		* the size in bytes. Can be also used to serialize
		* C strings with a known size.
		* @param data Pointer to the memory area.
		* @param length Length of the memory area in bytes.
		*/
		Packer& pack(const char* data, std::size_t length)
		{
			if (length <= bm::MAX_5BIT)
			{
				write((int8_t) (((int8_t) length) | bm::MP_FIXRAW));
			}
			else if (length <= bm::MAX_16BIT)
			{
				write(bm::MP_RAW16).write<int16_t>(length);
			}
			else
			{
				write(bm::MP_RAW32).write<int32_t>(length);
			}
			out_.write(data, length);
			return *this;
		}

		/**
		* Pack the elements in a range.
		* @param first Iterator to the initial position of the sequence.
		* @param last Iterator to the final position of the sequence
		*/
		template<typename IteratorT>
		Packer& pack(IteratorT first, IteratorT last)
		{
			typename std::iterator_traits<IteratorT>::value_type type;

			std::size_t length = std::distance(first, last);
			initContainer(length, type);

			// perform function for each element
			for (; first != last; ++first)
				pack(*first);

			return *this;
		}

		/**
		* Pack a std::pair
		* @param item reference to the object to be packed.
		*/
		template<typename keyT, typename valT>
		Packer& pack(const std::pair<const keyT, valT>& item)
		{
			return pack(item.first).pack(item.second);
		}

		/**
		* Pack a std::vector
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::vector<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::deque
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::deque<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::list
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::list<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::stack
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::stack<T>& item)
		{
			return pack(item.begin(), item.end());
		}


		/**
		* Pack a std::queue
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::queue<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::priority_queue
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::priority_queue<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::set
		* @param item reference to the container to be packed.
		*/
		template<typename T> inline
			Packer& pack(const std::set<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::multiset
		* @param item reference to the container to be packed.
		*/
		template<typename T>
		Packer& pack(const std::multiset<T>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::map
		* @param item reference to the container to be packed.
		*/
		template<typename T, typename U>
		Packer& pack(const std::map<T, U>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::multimap
		* @param item reference to the container to be packed.
		*/
		template<typename T, typename U>
		Packer& pack(const std::multimap<T, U>& item)
		{
			return pack(item.begin(), item.end());
		}

		/**
		* Pack a std::bitset
		* @param item reference to the object to be packed.
		*/
		template<size_t N>
		Packer& pack(const std::bitset<N>& item)
		{
			static size_t size = N;
			char p[(int) std::ceil(float(size)/float(8))]={0};
			for (int i=0; i<32; i++)
			{
				p[i / 8] |= (item[i] == '1') << 7 - i % 8;
			}
			return pack(&p[0], size);
		}

		/**
		* Serialize the given instance of the template type T into
		* a Packer object.
		* @param val Reference to the value to be packed
		*/
		template<typename T> inline
			Packer& operator<<(T& val)
		{
			return this->pack(val);
		}

		/**
		* Serialize the given reference to a Parcelable object
		* a Packer object.
		* @parcel Reference to the instance of the parcel
		*/
		Packer& operator<<(Parcelable& parcel)
		{
			return this->pack(parcel);
		}

	private:

		//! Write data into the underlying stream
		template<typename T> inline
			Packer& write(T data)
		{
			out_.write((char*) &data, sizeof(T));
			return *this;
		}

		//! Initialize the header for a given container
		template<typename T> inline
			void initContainer(std::size_t& length, T&)
		{
			if (length <= bm::MAX_4BIT)
			{
				write<int8_t>(((int8_t) length) | bm::MP_FIXARRAY);
			}
			else if (length <= bm::MAX_16BIT)
			{
				write(bm::MP_ARRAY16).write<int16_t>(length);
			}
			else
			{
				write(bm::MP_ARRAY32).write<int32_t>(length);
			}
		}

		//! Inintialize the header for a pair
		template<typename key, typename val> inline
			void initContainer(std::size_t& length, std::pair<const val, key>&)
		{
			if (length <= bm::MAX_4BIT)
			{
				write<int8_t>(((int8_t) length) | bm::MP_FIXMAP);
			}
			else if (length <= bm::MAX_16BIT)
			{
				write(bm::MP_MAP16).write<int16_t>(length);
			}
			else
			{
				write(bm::MP_MAP32).write<int32_t>(length);
			}
		}

		std::ostream& out_; //!< The output stream where the data is packed in

	}; // Packer

	/**
	* Type enum type defines all the possible output types
	* for the {@see #Object} instances generated by
	* the {@see #Unpacker} class
	*/
	enum object_type
	{
		NIL,      //<! nulll
		BOOL,     //<! boolean [true, false]
		INT8,     //<! int8 [positive, negative fixnum]
		INT16,    //<! int16
		INT32,    //<! int32
		INT64,    //<! int64
		UINT8,    //<! uint8
		UINT16,   //<! uint16
		UINT32,   //<! uint32
		UINT64,   //<! uint64
		FLOAT,    //<! float
		DOUBLE,   //<! double
		RAW,      //<! Raw bytes [fix raw, raw 16, raw 32]
		ARRAY,    //<! Array [fix array, array 16, array 32]
		MAP       //<! Map [fix map, map 16, map 32]

	}; // object_type

	namespace detail
	{

		/**
		* This defines the primary relationship
		* between an object_type and the data
		* type that is actually handled by it
		*/
		template<object_type t>
		struct type_traits {
		};

		/**
		* This defines the possible casts
		* from a given type T to an object type.
		*/
		template<typename T>
		struct type_cast {
		};

		// Forward definition
		template<typename type>
		class ObjectImpl;

	} // namespace detail

	/**
	* Object class. Represents a piece of deserialized
	* data, and provides access to the data itself.
	*/
	class Object
	{
	public:

		/**
		* Destructor
		*/
		virtual ~Object() {}

		/**
		* Retrieve the object type for the
		* current Object.
		*/
		object_type getType() const
		{
			return type_;
		}

		/**
		* Returns true if this object represents
		* a nil instance.
		*/
		virtual bool isNil() const
		{
			return false;
		}

		template<object_type object_t>
		detail::ObjectImpl<typename detail::type_traits<object_t>::type>& getImpl() throw(std::bad_cast)
		{
			return getImpl<typename detail::type_traits<object_t>::type>();
		}

		template<typename T>
		detail::ObjectImpl<T>& getImpl() throw(std::bad_cast)
		{
			if(detail::type_cast<T>::id!=type_)
				throw std::bad_cast();

			return *((detail::ObjectImpl<T>*) this);
		}

	protected:

		/**
		* Protected constructor. This is a virtual class.
		* @param type Type handled by the current object instance.
		*/
		template<typename type>
		Object(type) : type_(detail::type_cast<type>::id) {}

	private:
		const object_type type_; //!< Type of the current object

	}; // Object

	namespace detail
	{

		/**
		* Class ObjectImpl. Internal implementation for the speific Object types
		* that handle fixed length types, except for nil.
		*/
		template<typename T>
		class ObjectImpl: public Object
		{
		public:
			static const object_type obj_type = detail::type_cast<T>::id; //<! My type

			/**
			* Constructor.
			* @param value Value contained by the Object. There's a implicit
			* copy performed here.
			*/
			ObjectImpl(const T& value) : Object(T()), value_(value)
			{
			}

			/**
			* Retrieve value
			*/
			const T& getValue() const
			{
				return value_;
			}

			virtual ~ObjectImpl() {}

		protected:

			ObjectImpl() : Object(T()) {}

			T value_; //< Instance of the data handled by the Object

		}; // ObjectImpl

		struct nil_type {};

		/**
		* Nil class. Represents a nil object.
		*/
		class NilObject : public ObjectImpl<nil_type>
		{
		public:

			NilObject() {} //<! Constructor

			bool isNil() const
			{
				// I'm the nil guy!
				return true;
			}
		};


		/**
		* Declaration of the types correspondences with
		* their specific data types
		*/

#define TYPE_CAST(__ID__, __TYPE__) \
	template<> \
		struct type_cast<__TYPE__> \
		{ \
		static const object_type id = __ID__;\
		};

#define TYPE_TRAITS(__ID__, __TYPE__) \
	template<> \
		struct type_traits<__ID__> \
		{ \
		typedef __TYPE__ type; \
		}; \
		TYPE_CAST(__ID__, __TYPE__)

		TYPE_TRAITS(NIL,    nil_type)
			TYPE_TRAITS(BOOL,   bool)
			TYPE_TRAITS(INT8,   int8_t)
			TYPE_TRAITS(INT16,  int16_t)
			TYPE_TRAITS(INT32,  int32_t)
			TYPE_TRAITS(INT64,  int64_t)
			TYPE_TRAITS(UINT8,  uint8_t)
			TYPE_TRAITS(UINT16, uint16_t)
			TYPE_TRAITS(UINT32, uint32_t)
			TYPE_TRAITS(UINT64, uint64_t)
			TYPE_TRAITS(FLOAT,  float)
			TYPE_TRAITS(DOUBLE, double)

			typedef uint8_t* raw_type;
		TYPE_TRAITS(RAW,    raw_type)
			TYPE_CAST  (RAW,    std::string) // Type cast is implemented for raw type
			TYPE_CAST  (RAW,    std::wstring)

			typedef std::list<Object*> array_type;
		TYPE_TRAITS(ARRAY,    std::list<Object*>)

			typedef std::multimap<Object*,Object*> map_type;
		TYPE_TRAITS(MAP,    detail::map_type)

			/**
			* ObjectImpl class specilization for the ARRAY type.
			*/
		class ArrayObject : public ObjectImpl<array_type>
		{
		public:

			/**
			* Destructor. Removes and deletes all the Objects
			* contained in the array.
			*/
			virtual ~ArrayObject()
			{
				array_type::iterator it = value_.begin();;
				while(it!=value_.end())
				{
					delete *it;
					*it = 0;
					it++;
				}
				value_.clear();
			}

			/**
			* Add a new object to the array, which
			* will be owned and deleted by it on destruction.
			*/
			void add(Object* o)
			{
				value_.push_back(o);
			}
		};

		/**
		* ObjectImpl class specilization for the MAP type.
		*/
		class MapObject : public ObjectImpl<map_type>
		{
		public:

			/**
			* Destructor. Removes and deletes all the Object keys
			* and values contained in the map.
			*/
			virtual ~MapObject()
			{
				map_type::iterator it = value_.begin();;
				while(it!=value_.end())
				{
					delete it->first;
					delete it->second;
					it++;
				}
				value_.clear();
			}

			/**
			* Insert a new pair key-value into the map, which
			* will be owned and deleted by it on destruction.
			*/
			void insert(Object* key, Object* val)
			{
				value_.insert(map_type::value_type(key, val));
			}
		};

		/**
		* ObjectImpl class specilization for the RAW type.
		*/
		class RawObject : public ObjectImpl<raw_type>
		{
		public:

			/**
			* Constructor
			* @param value Reference to the data region handled
			* by the Object, which ownership is now from
			* the current Object which will destroy it on
			* deletion.
			* @param size Number of items held by this object
			*/
			RawObject(raw_type value, std::size_t size) : ObjectImpl<raw_type>(value), size_(size)
			{
			}

			/**
			* Destructor
			*/
			virtual ~RawObject()
			{
				delete[] value_;
			}

			template<typename char_t>
			operator std::basic_string<char_t>() const
			{
				std::basic_string<char_t> str;
				std::copy((char_t*)value_, (char_t*)(value_+size_), std::back_inserter(str));
				return str;
			}

		private:

			RawObject() {}

			std::size_t size_;
		};

		//! Utility struct that allows to remove the pointer part of a given type
		template<typename T>
		struct remove_pointer { typedef T type; };
		template<typename T>
		struct remove_pointer<T*> { typedef typename remove_pointer<T>::type type; };

	} // namespace detail

	typedef detail::NilObject                                        Nil;
	typedef detail::ObjectImpl<detail::type_traits<BOOL  >::type>   Bool;
	typedef detail::ObjectImpl<detail::type_traits<INT8  >::type>   Int8;
	typedef detail::ObjectImpl<detail::type_traits<INT16 >::type>  Int16;
	typedef detail::ObjectImpl<detail::type_traits<INT32 >::type>  Int32;
	typedef detail::ObjectImpl<detail::type_traits<INT64 >::type>  Int64;
	typedef detail::ObjectImpl<detail::type_traits<UINT8 >::type>  UInt8;
	typedef detail::ObjectImpl<detail::type_traits<UINT16>::type> UInt16;
	typedef detail::ObjectImpl<detail::type_traits<UINT32>::type> UInt32;
	typedef detail::ObjectImpl<detail::type_traits<UINT64>::type> UInt64;
	typedef detail::ObjectImpl<detail::type_traits<FLOAT >::type>  Float;
	typedef detail::ObjectImpl<detail::type_traits<DOUBLE>::type> Double;
	typedef detail::ArrayObject                                    Array;
	typedef detail::MapObject                                        Map;
	typedef detail::RawObject                                        Raw;

	/**
	* Exception likely to be thrown during the
	* data deserialization.
	*/
	typedef std::ios_base::failure unpack_exception;

	/**
	* Unpacker class. Allows to deserialize MessagePack binary data
	* from a stream
	*/
	class Unpacker
	{
	public:

		/**
		* Constructor
		* @param in Unpacker input stream from where the binary data is taken
		*/
		Unpacker(std::istream& in) : in_(in) {}

		/**
		* This method allows to unpack an object of any type
		* which implements the Parcelable interface.
		* @param p Reference to an object to be unpacked
		*/
		Unpacker& unpack(Parcelable& p)
		{
			p.unpack(*this);
			return *this;
		}

		/**
		* This method allows to unpack an object of the given data
		* type T. Should the data of the next element of the stream
		* be from a different type or the stream be empty, and
		* unpack_exception will be thrown.
		* @param v Reference to the object to be populated with the
		* unpacked data.
		*/
		template<typename T>
		Unpacker& unpack(T& v) throw(unpack_exception)
		{
			using namespace detail;

			Object* obj = 0;
			if((obj = this->unpack())!=0)
			{
				if(obj->isNil())
				{
					delete obj;
					throw unpack_exception("Null retrieved from the input stream");
				}

				switch(obj->getType())
				{
				case BOOL:
					v = (T) obj->getImpl<BOOL>().getValue();
					break;
				case INT8:
					v = (T) obj->getImpl<INT8>().getValue();
					break;
				case INT16:
					v = (T) obj->getImpl<INT16>().getValue();
					break;
				case INT32:
					v = (T) obj->getImpl<INT32>().getValue();
					break;
				case INT64:
					v = (T) obj->getImpl<INT64>().getValue();
					break;
				case UINT8:
					v = (T) obj->getImpl<UINT8>().getValue();
					break;
				case UINT16:
					v = (T) obj->getImpl<UINT16>().getValue();
					break;
				case UINT32:
					v = (T) obj->getImpl<UINT32>().getValue();
					break;
				case UINT64:
					v = (T) obj->getImpl<UINT64>().getValue();
					break;
				case FLOAT:
					v = (T) obj->getImpl<FLOAT>().getValue();
					break;
				case DOUBLE:
					v = (T) obj->getImpl<DOUBLE>().getValue();
					break;
				default:
					break;
				}
				delete obj;
			}
			else
			{
				throw unpack_exception("Unable to get next object from stream");
			}

			return *this;
		}

		/**
		* This method allows unpacking a generic C++ string.
		* Should the next element of the stream not be a string of the provided
		* type or a different object, or the stream be null, an unpack_exception
		* will be thrown.
		* @param v Reference to the string to be populated.
		*/
		template<typename char_t>
		Unpacker& unpack(std::basic_string<char_t>& v) throw(unpack_exception)
		{
			using namespace detail;

			Object* obj = 0;
			if((obj = this->unpack())!=0)
			{
				if(obj->isNil())
				{
					delete obj;
					throw unpack_exception("Null retrieved from the input stream");
				}

				if(obj->getType()!=RAW)
				{
					delete obj;
					throw unpack_exception("Unable to get next object from stream");
				}

				Raw& raw = (Raw&) obj->getImpl<RAW>();
				v = (std::basic_string<char_t>) raw;

				delete obj;
			}
			else
			{
				throw unpack_exception("Unable to get next object from stream");
			}

			return *this;
		}

		/**
		* Equivalent to unpack(T&)
		*/
		template<typename T>
		Unpacker& operator>>(T& v) throw(unpack_exception)
		{
			return unpack(v);
		}

		/**
		* Equivalent to unpack(std::basic_string<char_t>&)
		*/
		template<typename char_t>
		Unpacker& operator>>(std::basic_string<char_t>& v) throw(unpack_exception)
		{
			return unpack(v);
		}

		/**
		* Equivalent to unpack(Parcelable&)
		*/
		Unpacker& operator>>(Parcelable& v) throw (unpack_exception)
		{
			return unpack(v);
		}

		/**
		* Builds an Object from the available data in the input
		* stream. The caller is responsible for deleting the returned
		* instance.
		* This method may throw an {@see unpack_exception} in case the
		* buffer runs out of data while trying to deserialize.
		*/
		Object* unpack() throw(unpack_exception)
		{
			using namespace detail;

			if(in_.eof())
				throw unpack_exception("Reached end of stream");

			uint8_t value;
			read(value); // Read the header

			type_traits<FLOAT>::type  fVal;
			type_traits<DOUBLE>::type dVal;
			type_traits<INT8>::type   int8Val;
			type_traits<INT16>::type  int16Val;
			type_traits<INT32>::type int32Val;
			type_traits<INT64>::type int64Val;
			type_traits<UINT8>::type uint8Val;
			type_traits<UINT16>::type uint16Val;
			type_traits<UINT32>::type uint32Val;
			type_traits<UINT64>::type uint64Val;

			switch (value)
			{
			case bm::MP_NULL:
				return new Nil();
			case bm::MP_FALSE:
				return new Bool(false);
			case bm::MP_TRUE:
				return new Bool(true);
			case bm::MP_FLOAT:
				read(fVal);
				return new Float(fVal);
			case bm::MP_DOUBLE:
				read(dVal);
				return new Double(dVal);
			case bm::MP_UINT8:
				read(uint8Val);
				return new UInt8(uint8Val);
			case bm::MP_UINT16:
				read(uint16Val);
				return new UInt16(uint16Val);
			case bm::MP_UINT32:
				read(uint32Val);
				return new UInt32(uint32Val);
			case bm::MP_UINT64:
				read(uint64Val);
				return new UInt64(uint64Val);
			case bm::MP_INT8:
				read(int8Val);
				return new Int8(int8Val);
			case bm::MP_INT16:
				read(int16Val);
				return new Int16(int16Val);
			case bm::MP_INT32:
				read(int32Val);
				return new Int32(int32Val);
			case bm::MP_INT64:
				read(int64Val);
				return new Int64(int64Val);
			case bm::MP_ARRAY16:
				read(int16Val);
				return unpackArray(int16Val);
			case bm::MP_ARRAY32:
				read(int32Val);
				return unpackArray(int32Val);
			case bm::MP_MAP16:
				read(int16Val);
				return unpackMap(int16Val);
			case bm::MP_MAP32:
				read(int16Val);
				return unpackMap(int16Val);
			case bm::MP_RAW16:
				read(int16Val);
				return unpackRaw(int16Val);
			case bm::MP_RAW32:
				read(int32Val);
				return unpackRaw(int32Val);
			}

			if (((uint8_t)(value & 0xE0)) == bm::MP_FIXRAW)
			{
				return unpackRaw(value - bm::MP_FIXRAW);
			}

			if (((uint8_t)(value & 0xE0)) == bm::MP_NEGATIVE_FIXNUM)
			{
				return new Int32((value & 0x1F) - 32);
			}

			if (((uint8_t)(value & 0xF0)) == bm::MP_FIXARRAY)
			{
				return unpackArray(value - bm::MP_FIXARRAY);
			}

			if (((uint8_t)(value & 0xF0)) == bm::MP_FIXMAP)
			{
				return unpackMap(value - bm::MP_FIXMAP);
			}

			if (value <= 127) //MP_FIXNUM
			{
				return new Int8(value);
			}
			else
			{
				return 0;
			}
		}

	private:

		//! Unpack an array
		Array* unpackArray(int size)
		{
			if (size < 0)
				return 0;

			Array* ret = new Array();
			for (int i = 0; i < size; ++i)
			{
				ret->add(unpack());
			}
			return ret;
		}

		//! Unpack a map
		Map* unpackMap(int size)
		{
			if (size < 0)
				return 0;

			Map* ret = new Map();

			for (int i = 0; i < size; ++i)
			{
				Object* key = unpack();
				Object* val = unpack();
				ret->insert(key, val);
			}
			return ret;
		}


		//! Unpack raw data
		Raw* unpackRaw(int size)
		{
			if (size < 0)
				return 0;

			detail::type_traits<RAW>::type data =
				new detail::remove_pointer<detail::type_traits<RAW>::type>::type[size];

			for(int i = 0;i<size;i++)
				read(data[i]);

			return new Raw(data, size);
		}

		//! Read an object of the given type from the underlying stream
		template<typename T>
		Unpacker& read(T& ret) throw(unpack_exception)
		{
			if(in_.eof())
				throw unpack_exception("Reached end of stream while reading");

			in_.read((char*) &ret, sizeof(T));

			return *this;
		}

		std::istream& in_; //!< The stream we are unpacking the data from

	}; // Unpacker

} // namespace MSGPACK_NAMESPACE__

#endif // _MSGPACK_LITE_HPP_
