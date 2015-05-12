/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_STATUS_HPP
#define WASPP_STATUS_HPP

#include <string>

namespace waspp
{

	enum error_type
	{
		err_none = 0,
		err_unknown = 1,

		err_unauthorized = 1001,

		err_username_required = 1101,
		err_passwd_required = 1102,
		err_username_not_found = 1103,
		err_auth_failed = 1104,
		err_platformtype_required = 1105,
		err_platformid_required = 1106,
		err_passwds_not_identical = 1107,
		err_username_not_available = 1108,
		err_users_idx_insert_failed = 1109,
		err_users_insert_failed = 1110,

		err_title_required = 1201,
		err_content_required = 1202,
		err_forum_insert_failed = 1203,

		err_end
	};

} // namespace waspp

#endif // WASPP_STATUS_HPP
