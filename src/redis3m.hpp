// Copyright (c) 2014 Luca Marturana. All rights reserved.
// Licensed under Apache 2.0, see LICENSE for details

#ifndef REDIS3M_HPP
#define REDIS3M_HPP

#ifndef _WIN32
#include <hiredis/hiredis.h>

#include <vector>
#include <string>
#include <stdexcept>

#include <boost/unordered_map.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

// code from redis3m
/*
redis3m::connection::redis3m_ptr rdconn = redis3m::connection::connect();
rdconn->run(redis3m::command("SET") << "foo" << "bar");
redis3m::reply r = rdconn->run(redis3m::command("GET") << "foo");
std::cout << "foo is " << r.str() << std::endl;
*/

namespace redis3m
{

	enum type_t
	{
		STRING_ = 1,
		ARRAY_ = 2,
		INTEGER_ = 3,
		NIL_ = 4,
		STATUS_ = 5,
		ERROR_ = 6
	};

	class reply
	{
	public:
		type_t type() const { return type_; }
		const std::string& str() const { return str_; }
		long long int integer() const { return integer_; }
		const std::vector<reply>& elements() const { return elements_; }

		operator const std::string&() const { return str_; }
		operator const long long int() const { return integer_; }
		bool operator==(const std::string& rvalue) const
		{
			if (type_ == STRING_ || type_ == ERROR_ || type_ == STATUS_)
			{
				return str_ == rvalue;
			}

			return false;
		}

		bool operator==(const long long int rvalue) const
		{
			if (type_ == INTEGER_)
			{
				return integer_ == rvalue;
			}

			return false;
		}

	private:
		reply(redisReply* rep) : type_(ERROR_), integer_(0)
		{
			type_ = static_cast<type_t>(rep->type);

			switch (type_)
			{
			case ERROR_:
			case STRING_:
			case STATUS_:
				str_ = std::string(rep->str, rep->len);
				break;
			case INTEGER_:
				integer_ = rep->integer;
				break;
			case ARRAY_:
				for (std::size_t i = 0; i < rep->elements; ++i)
				{
					elements_.push_back(reply(rep->element[i]));
				}
				break;
			default:
				break;
			}
		}

		type_t type_;
		std::string str_;
		long long int integer_;
		std::vector<reply> elements_;

		friend class connection;

	};

	class command
	{
	public:
		command()
		{

		}

		command(std::string arg)
		{
			args.push_back(arg);
		}

		template<typename T>
		command& operator<<(const T arg)
		{
			args.push_back(boost::lexical_cast<std::string>(arg));
			return *this;
		}

		template<typename T>
		command& operator()(const T arg)
		{
			args.push_back(boost::lexical_cast<std::string>(arg));
			return *this;
		}

		operator const std::vector<std::string>&()
		{
			return args;
		}

	private:
		std::vector<std::string> args;

	};

	class connection
		: private boost::noncopyable
	{
	public:
		typedef boost::shared_ptr<connection> redis3m_ptr;

		~connection()
		{
			redisFree(c);
		}

		static redis3m_ptr connect(const std::string& host = "localhost", const unsigned int port = 6379, bool pooled_ = false)
		{
			return redis3m_ptr(new connection(host, port, pooled_));
		}

		bool is_valid() const
		{
			return c->err == REDIS_OK;
		}

		void append(const std::vector<std::string>& args)
		{
			std::vector<const char*> argv;
			argv.reserve(args.size());

			std::vector<std::size_t> argv_lengths;
			argv_lengths.reserve(args.size());

			std::vector<std::string>::const_iterator i;
			for (i = args.begin(); i != args.end(); ++i)
			{
				argv.push_back(i->c_str());
				argv_lengths.push_back(i->size());
			}

			int result = redisAppendCommandArgv(c, static_cast<int>(args.size()), argv.data(), argv_lengths.data());
			if (result != REDIS_OK)
			{
				throw std::runtime_error("redisAppendCommandArgv failed");
			}
		}

		reply get_reply()
		{
			redisReply* r;

			int result = redisGetReply(c, reinterpret_cast<void**>(&r));
			if (result != REDIS_OK)
			{
				throw std::runtime_error("redisGetReply failed");
			}

			reply rep(r);
			freeReplyObject(r);

			if (rep.type() == ERROR_ && (rep.str().find("READONLY") == 0))
			{
				throw std::runtime_error("read only");
			}

			return rep;
		}

		std::vector<reply> get_replies(unsigned int count)
		{
			std::vector<reply> replies;
			for (unsigned int i = 0; i < count; ++i)
			{
				replies.push_back(get_reply());
			}

			return replies;
		}

		reply run(const std::vector<std::string>& args)
		{
			append(args);
			return get_reply();
		}

		redisContext* c_ptr() { return c; }

		std::tm* last_released()
		{
			return &released;
		}

		void set_released(const std::tm& released_)
		{
			released = released_;
		}

		bool is_pooled()
		{
			return pooled;
		}

		void set_pooled(bool pooled_)
		{
			pooled = pooled_;
		}

	private:
		connection(const std::string& host, const unsigned int port, bool pooled_ = false)
		{
			c = redisConnect(host.c_str(), port);

			if (c->err != REDIS_OK)
			{
				throw std::runtime_error("redisConnect failed");
			}

			std::time_t time_ = std::time(0);
			set_released(*std::localtime(&time_));
			set_pooled(pooled_);
		}

		redisContext* c;

		std::tm released;
		bool pooled;

	};

} // namespace redis3m

#endif // _WIN32
#endif // REDIS3M_HPP
