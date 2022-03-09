#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include "exerciser.h"

using namespace std;
using namespace pqxx;

void execute_sql(string sql_command, connection * C) {
  work W(*C);
  W.exec(sql_command);
  W.commit();
}

void drop_table(connection * C, vector<string> & table_list) {
  for (auto it = table_list.begin(); it != table_list.end(); ++it) {
    string sql_command = "DROP TABLE IF EXISTS " + *it + " CASCADE;";
    execute_sql(sql_command, C);
  }
}
string get_string(string file_name) {
  string line;
  string table_string;
  ifstream myfile(file_name);
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      table_string += line + "\n";
    }
    myfile.close();
  }
  else {
    cout << "cannot open tables file" << endl;
  }
  return table_string;
}
void create_table(connection * C, string table_string) {
  execute_sql(table_string, C);
}
void insert_state_rows(connection * C, string state_string) {
  stringstream ss;
  string name;
  int state_id;
  int pos;
  string delemitor = "\n";
  while ((pos = state_string.find(delemitor)) != string::npos) {
    ss << state_string;
    ss >> state_id >> name;
    //cout << state_id <<"," << name <<endl;
    add_state(C, name);
    state_string.erase(0, pos + delemitor.length());
  }
}

int main(int argc, char * argv[]) {
  //Allocate & initialize a Postgres connection object
  connection * C;

  try {
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    }
    else {
      cout << "Can't open database" << endl;
      return 1;
    }
  }
  catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  vector<string> table_list;
  table_list.push_back("PLAYER");
  table_list.push_back("TEAM");
  table_list.push_back("STATE");
  table_list.push_back("COLOR");
  drop_table(C, table_list);
  string table_string = get_string("tables.txt");
  create_table(C, table_string);
  string state_string = get_string("state.txt");
  insert_state_rows(C, state_string);
  //cout << table_string << '\n';
  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}
