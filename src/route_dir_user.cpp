/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "router.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utility.hpp"

namespace waspp
{
	namespace dir_user
	{

		void signin_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_user_signin.html");
			router::get_file(res, full_path);
		}

		void auth(logger* log, config* cfg, database* db, request& req, response& res)
		{
			dbconn_ptr db_index = db->get("db_index");

			unsigned int userid = 1;
			dbconn_ptr db_shard = db->get_shard(userid);

			res.content = "OK";
			res.content_extension = "html";

			db->free("db_index", db_index);
			db->free_shard(userid, db_shard);
		}

		void signup_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_user_signup.html");
			router::get_file(res, full_path);
		}

		void post(logger* log, config* cfg, database* db, request& req, response& res)
		{
			res.content_extension = "html";

			unsigned int userid;

			int platformtype;
			std::string platformid;

			std::string username;
			std::string passwd;

			// param check
			{
				if (req.param("platformtype").empty())
				{
					res.content = "platformtype is required";
					return;
				}
				platformtype = boost::lexical_cast<int>(req.param("platformtype"));

				if (req.param("platformid").empty())
				{
					res.content = "platformid is required";
					return;
				}
				platformid = req.param("platformid");

				if (req.param("username").empty())
				{
					res.content = "username is required";
					return;
				}
				username = req.param("username");

				if (req.param("passwd").empty())
				{
					res.content = "passwd is required";
					return;
				}
				passwd = md5_digest(req.param("passwd"));
			}

			dbconn_ptr db_index = db->get("db_index");
			try
			{
				stmt_ptr select_username(db_index->prepare("SELECT userid FROM users_idx WHERE username = ?"));
				{
					select_username->param(username);
				}

				res_ptr r_username(select_username->query());
				if (r_username->num_rows() != 0)
				{
					throw std::runtime_error("username not available");
				}

				userid = 3;
				/*
				stmt_ptr get_keys(db_index->prepare("CALL USP_GET_UNIQUE_KEYS(?, ?)"));
				{
					get_keys->param("users_idx");
					get_keys->param(1);
				}

				res_ptr r_keys(get_keys->query());
				if (r_keys->fetch())
				{
					userid = r_keys->get<unsigned int>("last_key");
				}
				*/

				stmt_ptr insert_idx(db_index->prepare("INSERT INTO users_idx(userid, platformtype, platformid, username, inserttime, updatetime) VALUES(?, ?, ?, ?, NOW(), NOW())"));
				{
					insert_idx->param(userid);
					insert_idx->param(platformtype);
					insert_idx->param(platformid);
					insert_idx->param(username);
				}

				unsigned long long int affected_rows = insert_idx->execute();
				if (affected_rows == 0)
				{
					throw std::runtime_error("insert_index failed");
				}
			}
			catch (...)
			{
				db->free("db_index", db_index);
				throw;
			}
			db->free("db_index", db_index);

			dbconn_ptr db_shard = db->get_shard(userid);
			try
			{
				stmt_ptr insert_shard(db_shard->prepare("INSERT INTO users(userid, passwd, inserttime, updatetime) VALUES(?, ?, NOW(), NOW())"));
				{
					insert_shard->param(userid);
					insert_shard->param(passwd);
				}
				unsigned long long int affected_rows = insert_shard->execute();

				if (affected_rows == 0)
				{
					throw std::runtime_error("insert_shard failed");
				}
			}
			catch (...)
			{
				db->free_shard(userid, db_shard);
				throw;
			}
			db->free_shard(userid, db_shard);
			
			res.redirect_to("/dir/user/signin");
		}

		void signout(logger* log, config* cfg, database* db, request& req, response& res)
		{
			res.content = "OK";
			res.content_extension = "html";
		}

	} // namespace dir_user

} // namespace waspp
