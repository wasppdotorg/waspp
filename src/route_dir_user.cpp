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
#include "error.hpp"
#include "performance_checker.hpp"

namespace waspp
{
	namespace dir_user
	{

		void signin_html(config* cfg, request_ptr req, response_ptr res)
		{
			router::get_file(cfg, res, "dir_include_header.html");
			router::get_file(cfg, res, "dir_user_signin.html");
			router::get_file(cfg, res, "dir_include_footer.html");
		}

		void auth(config* cfg, request_ptr req, response_ptr res)
		{
			performance_checker c(50, __FILE__, __LINE__);

			unsigned int userid = 0;
			std::string passwd;

			if (req->param("username").empty())
			{
				router::err_msg(cfg, res, err_username_required);
				return;
			}

			if (req->param("passwd").empty())
			{
				router::err_msg(cfg, res, err_passwd_required);
				return;
			}
			passwd = md5_digest(req->param("passwd"));

			scoped_db db_idx("db_idx");

			stmt_ptr stmt(db_idx.prepare("SELECT userid FROM users_idx WHERE username = ?"));
			{
				stmt->param(req->param("username"));
			}

			rs_ptr rs(stmt->query());
			if (rs->num_rows() == 0)
			{
				router::err_msg(cfg, res, err_username_not_found);
				return;
			}

			if (rs->fetch())
			{
				userid = rs->get<unsigned int>("userid");
			}

			scoped_db db_shard(userid);

			stmt.reset(db_shard.prepare("SELECT userid FROM users WHERE userid = ? AND passwd = ?"));
			{
				stmt->param(userid);
				stmt->param(passwd);
			}

			rs.reset(stmt->query());
			if (rs->num_rows() == 0)
			{
				router::err_msg(cfg, res, err_auth_failed);
				return;
			}

			waspp::session sess(cfg, req, res);
			sess.put("userid", boost::lexical_cast<std::string>(userid));
			sess.put("username", req->param("username"));

			res->redirect_to("/dir/forum/index");
		}

		void signup_html(config* cfg, request_ptr req, response_ptr res)
		{
			router::get_file(cfg, res, "dir_include_header.html");
			router::get_file(cfg, res, "dir_user_signup.html");
			router::get_file(cfg, res, "dir_include_footer.html");
		}

		void post(config* cfg, request_ptr req, response_ptr res)
		{
			performance_checker c(50, __FILE__, __LINE__);

			unsigned int userid;
			int platformtype;
			std::string platformid;
			std::string passwd;

			if (req->param("platformtype").empty())
			{
				router::err_msg(cfg, res, err_platformtype_required);
				return;
			}
			platformtype = boost::lexical_cast<int>(req->param("platformtype"));

			if (req->param("platformid").empty())
			{
				router::err_msg(cfg, res, err_platformid_required);
				return;
			}

			if (req->param("username").empty())
			{
				router::err_msg(cfg, res, err_username_required);
				return;
			}

			if (req->param("passwd").empty())
			{
				router::err_msg(cfg, res, err_passwd_required);
				return;
			}

			if (req->param("passwd") != req->param("retypepasswd"))
			{
				router::err_msg(cfg, res, err_passwds_not_identical);
				return;
			}
			passwd = md5_digest(req->param("passwd"));

			scoped_db db_idx("db_idx");

			stmt_ptr stmt(db_idx.prepare("SELECT userid FROM users_idx WHERE username = ?"));
			{
				stmt->param(req->param("username"));
			}

			rs_ptr rs(stmt->query());
			if (rs->num_rows() != 0)
			{
				router::err_msg(cfg, res, err_username_not_available);
				return;
			}

			stmt.reset(db_idx.prepare("CALL USP_GET_UNIQUE_KEYS('users_idx', ?)"));
			{
				stmt->param(1);
			}

			rs.reset(stmt->query());
			if (rs->fetch(true))
			{
				userid = rs->get<unsigned int>("last_key");
			}
			platformid = boost::lexical_cast<std::string>(userid);

			stmt.reset(db_idx.prepare("INSERT INTO users_idx(userid, platformtype, platformid, username, inserttime, updatetime) VALUES(?, ?, ?, ?, NOW(), NOW())"));
			{
				stmt->param(userid);
				stmt->param(platformtype);
				stmt->param(platformid);
				stmt->param(req->param("username"));
			}

			unsigned long long int affected_rows = stmt->execute();
			if (affected_rows == 0)
			{
				router::err_msg(cfg, res, err_users_idx_insert_failed);
				return;
			}

			scoped_db db_shard(userid);

			stmt.reset(db_shard.prepare("INSERT INTO users(userid, passwd, inserttime, updatetime) VALUES(?, ?, NOW(), NOW())"));
			{
				stmt->param(userid);
				stmt->param(passwd);
			}

			affected_rows = stmt->execute();
			if (affected_rows == 0)
			{
				router::err_msg(cfg, res, err_users_insert_failed);
				return;
			}

			res->redirect_to("/dir/user/signin");
		}

		void signout(config* cfg, request_ptr req, response_ptr res)
		{
			res->delete_cookie(cfg->sess_cookie());
			res->redirect_to("/dir/user/signin");
		}

	} // namespace dir_user

} // namespace waspp
