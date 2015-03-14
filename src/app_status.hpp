/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_APP_STATUS_HPP
#define WASPP_APP_STATUS_HPP

#include <string>

namespace waspp
{

	enum app_status_type
	{
		status_okay = 0,
		status_unauthorized = 1001,

		status_username_required = 1101,
		status_passwd_required = 1102,
		status_username_not_found = 1103,
		status_auth_failed = 1104,
		status_platformtype_required = 1105,
		status_platformid_required = 1106,
		status_passwd_not_identical = 1107,
		status_username_not_available = 1108,
		status_users_idx_insert_failed = 1109,
		status_users_insert_failed = 1110,

		status_title_required = 1201,
		status_content_required = 1202,
		status_forum_insert_failed = 1203,

		status_error = 9999
	};

} // namespace waspp

#endif // WASPP_APP_STATUS_HPP
