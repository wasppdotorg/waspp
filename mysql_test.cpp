#include <iostream>
#include <vector>

#include "mysql.hpp"

int main()
{
	waspp::mysql_conn* conn = new waspp::mysql_conn();

	waspp::mysql_stmt* stmt;
	//stmt = conn->prepare_statement("DROP TABLE IF EXISTS test");
	//stmt = conn->prepare_statement("DROP TABLE test");
	//stmt->execute();

	//stmt = conn->prepare_statement("CREATE TABLE test(id INT)");
	//stmt->execute();

	std::vector<waspp::mysql_param> params;
	{
		params.push_back(waspp::mysql_param("1"));
	}

	stmt = conn->prepare_statement("INSERT INTO test(id) VALUES (?)", params);
	stmt->execute();
	
	std::cin.get();
	return 0;
}