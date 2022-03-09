#include "query_funcs.h"

void add_player(connection * C,
                int team_id,
                int jersey_num,
                string first_name,
                string last_name,
                int mpg,
                int ppg,
                int rpg,
                int apg,
                double spg,
                double bpg) {
  work W(*C);
  stringstream sql_command;
  sql_command
      << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES ("
      << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", "
      << W.quote(last_name) << ", " << mpg << ", " << ppg << ", " << rpg << ", " << apg
      << ", " << spg << ", " << bpg << ");";
  W.exec(sql_command.str());
  W.commit();
}

void add_team(connection * C,
              string name,
              int state_id,
              int color_id,
              int wins,
              int losses) {
  work W(*C);
  stringstream sql_command;
  sql_command << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
              << W.quote(name) << ", " << state_id << ", " << color_id << ", " << wins
              << ", " << losses << ");";
  W.exec(sql_command.str());
  W.commit();
}

void add_state(connection * C, string name) {
  work W(*C);
  string sql_command = "INSERT INTO STATE (NAME) VALUES (" + W.quote(name) + ");";
  W.exec(sql_command);
  W.commit();
}

void add_color(connection * C, string name) {
  work W(*C);
  string sql_command = "INSERT INTO COLOR (NAME) VALUES (" + W.quote(name) + ");";
  W.exec(sql_command);
  W.commit();
}

void query1(connection * C,
            int use_mpg,
            int min_mpg,
            int max_mpg,
            int use_ppg,
            int min_ppg,
            int max_ppg,
            int use_rpg,
            int min_rpg,
            int max_rpg,
            int use_apg,
            int min_apg,
            int max_apg,
            int use_spg,
            double min_spg,
            double max_spg,
            int use_bpg,
            double min_bpg,
            double max_bpg) {
}

void query2(connection * C, string team_color) {
}

void query3(connection * C, string team_name) {
}

void query4(connection * C, string team_state, string team_color) {
}

void query5(connection * C, int num_wins) {
}

// my implementation

// void execute_sql(string sql_command, connection * C) {
//   work W(*C);
//   W.exec(sql_command);
//   W.commit();
// }

void drop_table(connection * C, vector<string> & table_list) {
  for (auto it = table_list.begin(); it != table_list.end(); ++it) {
    string sql_command = "DROP TABLE IF EXISTS " + *it + " CASCADE;";
    work W(*C);
    W.exec(sql_command);
    W.commit();
  }
}

void create_table(connection * C, string table_string) {
  //execute_sql(table_string, C);
  work W(*C);
  W.exec(table_string);
  W.commit();
}

void insert_state_rows(connection * C, string state_string) {
  stringstream ss;
  int pos, state_id;
  string name;
  string delemitor = "\n";
  while ((pos = state_string.find(delemitor)) != string::npos) {
    ss << state_string;
    ss >> state_id >> name;
    add_state(C, name);
    state_string.erase(0, pos + delemitor.length());
  }
}

void insert_color_rows(connection * C, string color_string) {
  stringstream ss;
  string name;
  int pos, color_id;
  string delemitor = "\n";
  while ((pos = color_string.find(delemitor)) != string::npos) {
    ss << color_string;
    ss >> color_id >> name;
    add_color(C, name);
    color_string.erase(0, pos + delemitor.length());
  }
}

void insert_team_rows(connection * C, string team_string) {
  stringstream ss;
  int pos, team_id, state_id, color_id, wins, losses;
  string name;
  string delemitor = "\n";
  while ((pos = team_string.find(delemitor)) != string::npos) {
    ss << team_string;
    ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
    add_team(C, name, state_id, color_id, wins, losses);
    team_string.erase(0, pos + delemitor.length());
  }
}
void insert_player_rows(connection * C, string player_string) {
  stringstream ss;
  int pos, player_id, team_id, jersey_num, mpg, ppg, rpg, apg;
  string first_name, last_name;
  double spg, bpg;
  string delemitor = "\n";
  while ((pos = player_string.find(delemitor)) != string::npos) {
    ss << player_string;
    ss >> player_id >> team_id >> jersey_num >> first_name >> last_name >> mpg >> ppg >>
        rpg >> apg >> spg >> bpg;
    add_player(
        C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
    player_string.erase(0, pos + delemitor.length());
  }
}