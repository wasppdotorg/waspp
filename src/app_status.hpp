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
		status_okay = 2000,
		status_unauthorized = 4001,

		status_username_required = 6101,
		status_passwd_required = 6102,
		status_username_not_found = 6103,
		status_auth_failed = 6104,
		status_platformtype_required = 6105,
		status_platformid_required = 6106,
		status_passwd_not_identical = 6107,
		status_username_not_available = 6108,
		status_users_idx_insert_failed = 6109,
		status_users_insert_failed = 6110,

		status_title_required = 6201,
		status_content_required = 6202,
		status_forum_insert_failed = 6203,

		status_error = 9999
	};

} // namespace waspp

#endif // WASPP_APP_STATUS_HPP
