#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <string>

#include "exerciser.h"

using namespace std;
using namespace pqxx;
string get_string(string file_name) {
  string line;
  string result_string;
  ifstream myfile(file_name);
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      result_string += line + "\n";
    }
    myfile.close();
  }
  else {
    cout << "cannot open tables file" << endl;
  }
  return result_string;
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
  string color_string = get_string("color.txt");
  insert_color_rows(C, color_string);
  string team_string = get_string("team.txt");
  insert_team_rows(C, team_string);
  string player_string = get_string("player.txt");
  insert_player_rows(C, player_string);
  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}
