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
#include "session.hpp"

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
			unsigned int userid;
			std::string username;
			std::string passwd;

			md5 md5_;

			// param check
			{
				if (req.param("username").empty())
				{
					router::err_msg(res, "username is required", false);
					return;
				}
				username = req.param("username");

				if (req.param("passwd").empty())
				{
					router::err_msg(res, "passwd is required", false);
					return;
				}
				passwd = md5_.digest(req.param("passwd"));
			}

			dbconn_ptr db_index = db->get("db_index");
			{
				stmt_ptr stmt(db_index->prepare("SELECT userid FROM users_idx WHERE username = ?"));
				{
					stmt->param(username);
				}

				res_ptr r(stmt->query());
				if (r->num_rows() == 0)
				{
					router::err_msg(res, "auth failed(1)", db, "db_index", db_index);
					return;
				}

				if (r->fetch())
				{
					userid = r->get<unsigned int>("userid");
				}
			}
			db->free("db_index", db_index);

			dbconn_ptr db_shard = db->get_shard(userid);
			{
				stmt_ptr stmt(db_shard->prepare("SELECT userid FROM users WHERE userid = ? AND passwd = ?"));
				{
					stmt->param(userid);
					stmt->param(passwd);
				}

				res_ptr r(stmt->query());
				if (r->num_rows() == 0)
				{
					router::err_msg(res, "auth failed(2)", db, "db_shard", db_shard);
					return;
				}

				waspp::session sess(cfg, &req, &res);
				sess.set_sess("userid", boost::lexical_cast<std::string>(userid));
				sess.set_sess("username", username);
			}
			db->free_shard(userid, db_shard);

			res.redirect_to("/dir/board/index");
		}

		void signup_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_user_signup.html");
			router::get_file(res, full_path);
		}

		void post(logger* log, config* cfg, database* db, request& req, response& res)
		{
			unsigned int userid;

			int platformtype;
			std::string platformid;

			std::string username;
			std::string passwd;

			md5 md5_;

			// param check
			{
				if (req.param("platformtype").empty())
				{
					router::err_msg(res, "platformtype is required", false);
					return;
				}
				platformtype = boost::lexical_cast<int>(req.param("platformtype"));

				if (req.param("platformid").empty())
				{
					router::err_msg(res, "platformid is required", false);
					return;
				}
				platformid = req.param("platformid");

				if (req.param("username").empty())
				{
					router::err_msg(res, "username is required", false);
					return;
				}
				username = req.param("username");

				if (req.param("passwd").empty())
				{
					router::err_msg(res, "passwd is required", false);
					return;
				}
				passwd = md5_.digest(req.param("passwd"));
			}

			dbconn_ptr db_index = db->get("db_index");
			{
				stmt_ptr stmt(db_index->prepare("SELECT userid FROM users_idx WHERE username = ?"));
				{
					stmt->param(username);
				}

				res_ptr r(stmt->query());
				if (r->num_rows() != 0)
				{
					router::err_msg(res, "username not available", db, "db_index", db_index);
					return;
				}

				stmt.reset(db_index->prepare("CALL USP_GET_UNIQUE_KEYS('users_idx', ?)"));
				{
					stmt->param(1);
				}

				r.reset(stmt->query());
				if (r->fetch_proc_result())
				{
					userid = r->get<unsigned int>("last_key");
				}
				platformid = boost::lexical_cast<std::string>(userid);

				stmt.reset(db_index->prepare("INSERT INTO users_idx(userid, platformtype, platformid, username, inserttime, updatetime) VALUES(?, ?, ?, ?, NOW(), NOW())"));
				{
					stmt->param(userid);
					stmt->param(platformtype);
					stmt->param(platformid);
					stmt->param(username);
				}

				unsigned long long int affected_rows = stmt->execute();
				if (affected_rows == 0)
				{
					router::err_msg(res, "insert_index failed", db, "db_index", db_index);
				}
			}
			db->free("db_index", db_index);

			dbconn_ptr db_shard = db->get_shard(userid);
			{
				stmt_ptr stmt(db_shard->prepare("INSERT INTO users(userid, passwd, inserttime, updatetime) VALUES(?, ?, NOW(), NOW())"));
				{
					stmt->param(userid);
					stmt->param(passwd);
				}

				unsigned long long int affected_rows = stmt->execute();
				if (affected_rows == 0)
				{
					router::err_msg(res, "insert_shard failed", db, "db_shard", db_shard);
					return;
				}
			}
			db->free_shard(userid, db_shard);

			res.redirect_to("/dir/user/signin");
		}

		void signout(logger* log, config* cfg, database* db, request& req, response& res)
		{
			res.delete_cookie(cfg->sess_cookie);
			res.redirect_to("/dir/user/signin");
		}

	} // namespace dir_user

} // namespace waspp
