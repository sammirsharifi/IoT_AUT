#include <Arduino.h>

HardwareSerial& terminal = Serial3;

String description;
bool poll_active;
bool allow_clear_terminal { false };

void setup() {
  Serial.begin(57600);
  terminal.begin(57600);
}

void parseMessage(String);
void handleNewPoll(String);
void handlePollResult(String);

void send_message(String message) {
  Serial.print(message + '\r');
}

void reset_terminal() {
  terminal.write(0x0C);
  terminal.println(description);
}

void loop() {
  if (poll_active) {
    if (terminal.available()) {
      String input = terminal.readStringUntil('\r');
      send_message(input);
    }
  }
  if (Serial.available()) {
    String message = Serial.readStringUntil('\r');
    parseMessage(message);
  }
  if (allow_clear_terminal) {
    delay(200);
    reset_terminal();
    allow_clear_terminal = false;
  }
}

void parseMessage(String message) {
  message.trim();
  if (message.startsWith("poll:")) {
    handleNewPoll(message);
    reset_terminal();
  } else if (message.equals("Success")) {
    terminal.println(message);
    allow_clear_terminal = true;
  } else if (message.startsWith("Error")) {
    terminal.println(message);
    allow_clear_terminal = true;
  } else if (message.startsWith("res:poll")) {
    handlePollResult(message);
    allow_clear_terminal = true;
  }
}

void handleNewPoll(String poll) {
  String message = poll;
  message = message.substring(message.indexOf("poll:") + String("poll:").length());
  char poll_number = message[0];
  message = message.substring(1);
  String out = "";
  out += "Poll[" + String(poll_number) + "]\n\r";
  message = message.substring(String(";candidates:").length());
  unsigned int candidate_idx = 0;
  for (unsigned int i = 0; message[i] != ';' && i < message.length(); i++) {
    bool should_break = false;
    int candidate_split_index = message.indexOf(',', i + 1);
    if (candidate_split_index < 0) {
      should_break = true;
    }
    candidate_idx += 1;
    String candidate_name = message.substring(i, should_break ? message.length() : candidate_split_index);
    out += String(candidate_idx) + ")" + candidate_name + "\r\n";
    i = candidate_split_index;
    if (should_break) {
      break;
    }
  }
  description = out;
  poll_active = true;
}

void handlePollResult(String result) {
  String message = result;
  message = message.substring(message.indexOf("res:poll:") + String("res:poll:").length());
  char poll_number = message[0];
  message = message.substring(2);
  String out {""};
  out += "Poll[" + String(poll_number) + "] results\n\r";
  for (unsigned int i = 0; message[i] != ';' && i < message.length(); i++) {
    bool should_break = false;
    int candidate_split_index = message.indexOf(',', i + 1);
    if (candidate_split_index < 0) {
      should_break = true;
    }
    String candidate_name = message.substring(i, should_break ? message.length() : candidate_split_index);
    out += "\r\n" + candidate_name;
    i = candidate_split_index;
    if (should_break) {
      break;
    }
  }
  terminal.println(out);
  description = out;
  poll_active = false;
}
