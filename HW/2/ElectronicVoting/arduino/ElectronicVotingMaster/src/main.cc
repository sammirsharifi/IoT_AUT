#include <Arduino.h>
#include "Poll.hh"

// HardwareSerial& terminal = Serial1;

const int MAX_USER_COUNT {100};
const int MAX_POLL_COUNT {10};

String candidates[] {
  "candidate1",
  "candidate2",
  "candidate3",
};
unsigned int candidates_count {3};

String users[MAX_USER_COUNT];
unsigned int users_count {0};

Poll polls[MAX_POLL_COUNT];
Poll current_poll;
int current_poll_idx {-1};

char welcome_message[] {"1) Start a new poll\r\n2) Add a new user\r\n3) End poll\r\n4) See polls history\r\n5) List users\r\n"};
bool poll_history_mode {false};

void handleInput(String);
void handleNodeMessage(String);

void polls_history_prompt();
void view_poll_history(int);

int get_user(String);
void list_users();
bool addUser(String);
void startNewPoll();
void handleEndPoll();

void setup() {
  Serial.begin(57600);//xbee
  terminal.begin(57600);
  terminal.print(welcome_message);
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\r');
    message.trim();
    handleNodeMessage(message);
 
  }
  if (terminal.available()) {
    String input = terminal.readStringUntil('\r');
    input.trim();
    if (poll_history_mode) {
      view_poll_history(input.toInt() - 1);
    } else handleInput(input);
  }
}

void handleInput(String input) {
  if (input.equals("1")) {
    startNewPoll();
    return;
  }
  if (input.startsWith("2")) {
    if (input.substring(1, 2) != " ") {
      terminal.println("invalid syntax, use: 2 [USER]");
      return;
    }
    input = input.substring(2);
    input.trim();
    addUser(input);
    return;
  }
  if (input.equals("3")) {
    handleEndPoll();
    return;
  }
  if (input.equals("4")) {
    polls_history_prompt();
    return;
  }
  if (input.equals("5")) {
    list_users();
    return;
  }
}

void handleNodeMessage(String input) {
  String vote_str = input;
  int colon_split_idx = vote_str.indexOf(':');
  if (colon_split_idx < 0) {
    Serial.print("Error: invalid voting syntax\r");
    return;
  }
  String user = vote_str.substring(0, colon_split_idx);
  int user_idx;
  if ((user_idx = get_user(user)) < 0) {
      Serial.print("Error: invalid user\r");
      return;
  }

  unsigned int vote = vote_str.substring(colon_split_idx + 1, vote_str.length()).toInt();
  if (vote >= candidates_count + 1 || vote == 0) {
    Serial.print("Error: invalid candidate\r");
    return;
  }
  vote -= 1; // conversion to index
  int vote_result;
  if ((vote_result = current_poll.assignVote(user, vote)) < 0) {
    switch (vote_result) {
    case -4:
      Serial.print("Error: duplicate user vote");
    }
    return;
  }
  Serial.print("Success");
  terminal.println("User " + user + " voted for " + candidates[vote]);
}

void polls_history_prompt() {
  poll_history_mode = true;
  terminal.write(0x0C);//clear screen
  terminal.println("Which poll's results do you wish to see? use 0 to return\r\n");
  for (int i = 0; i < current_poll_idx + 1; i++) {
    terminal.println("Poll[" + String(i + 1) + "]");
  }
}

void view_poll_history(int poll_idx) {
  if (poll_idx == -1) {
    poll_history_mode = false;
    terminal.write(0x0C);
    terminal.println(welcome_message);
    return;
  }
  if (poll_idx < 0 || poll_idx > current_poll_idx) {
    terminal.println("invalid poll");
    return;
  }
  terminal.println(polls[poll_idx].getResults());
}

int get_user(String user_to_check) {
  user_to_check.trim();
  for (unsigned int i = 0; i < users_count; i++)
    if (user_to_check.equals(users[i]))
      return i;
  return -1;
}

void list_users() {
  terminal.println("Users:");
  for (unsigned int i = 0; i < users_count; i++) {
    terminal.println(String(i + 1) + ": " + users[i]);
  }
}

bool addUser(String user) {
  if (current_poll.isActive()) {
    terminal.println("Can't add user during active poll");
    return false;
  }
  if (users_count == MAX_USER_COUNT) {
    terminal.println("Can't add more users");
    return false;
  }
  if (get_user(user) >= 0) {
    terminal.println("Username already exists");
    return false;
  }
  users[users_count++] = user;
  terminal.println(users[users_count]);
  terminal.println("user `" + user + "` created");
  return true;
}

void startNewPoll() {
  if (current_poll.isActive()) {
    terminal.println("Can't start a new poll when there's already an active poll");
    return;
  }
  if (users_count <= 0) {
    terminal.println("Insufficient users, cannot create poll");
    return;
  }
  polls[++current_poll_idx] = *(new Poll{candidates, users, users_count, candidates_count});
  current_poll = polls[current_poll_idx];
  current_poll.setActive();
  String message = "poll:" + String(current_poll_idx + 1) + ";candidates:";
  for (unsigned int i = 0; i < candidates_count; i++) {
    message += candidates[i];
    if (i < candidates_count - 1) {
      message += ',';
    }
  }

  Serial.println(message);
  terminal.println(message);
}

void handleEndPoll() {
  if (!current_poll.isActive()) {
    terminal.println("no currently active poll");
    return;
  }
  current_poll.setActive(false);
  String results = current_poll.getResults();
  String out {""};
  out += "Poll[" + String(current_poll_idx + 1) + "] results\n\r";
  for (unsigned int i = 0; results[i] != ';' && i < results.length(); i++) {
    bool should_break = false;
    int candidate_split_index = results.indexOf(',', i + 1);
    if (candidate_split_index < 0) {
      should_break = true;
    }
    String candidate_name = results.substring(i, should_break ? results.length() : candidate_split_index);
    out += "\r\n" + candidate_name;
    i = candidate_split_index;
    if (should_break) {
      break;
    }
  }
  terminal.println(out);
  Serial.print("res:poll:" + String(current_poll_idx + 1) + ";" + results + "\r");
}
