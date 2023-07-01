#include <Arduino.h>
HardwareSerial& terminal = Serial1;

class Poll {
	String* candidates;
	String* users;
	int* votes;
	int* users_votes;

	unsigned int candidates_count;
	unsigned int users_count;

	bool is_active;

	int getCandidate(String candidate) {
		int candidate_idx = -1;
		for (unsigned int i = 0; i < candidates_count; i++) {
			if (candidates[i] == candidate) {
				candidate_idx = i;
				break;
			}
		}
		return candidate_idx;
	}

	int getUser(String user) {
		int user_idx = -1;
		for (unsigned int i = 0; i < users_count; i++) {
			if (users[i] == user) {
				user_idx = i;
				break;
			}
		}
		return user_idx;
	}

	public:
	Poll() {
		terminal.println("default constructor");
	};
	Poll(String candidates[], String users[], unsigned int users_count, unsigned int candidates_count)
	: candidates{candidates}, users{users},
		candidates_count{candidates_count}, users_count{users_count}, is_active{true}
	{
		votes = new int[candidates_count]{};
		users_votes = new int[users_count];
		for (unsigned int i = 0; i < users_count; i++) users_votes[i] = -1;		
	}

	~Poll() { delete votes; delete users_votes; }

	int assignVote(String user, unsigned int candidate_idx) {
		if (!is_active) return -1;

		int user_idx = getUser(user);
		if (user_idx < 0) return -2;

		if (candidate_idx < 0 || candidate_idx >= candidates_count) return -3;

		if (users_votes[user_idx] >= 0) {
			return -4;
		}

		votes[candidate_idx]++;
		users_votes[user_idx] = candidate_idx;

		return 0;
	}

	String getResults() {
		String out {""};
		for (unsigned int i = 0; i < candidates_count; i++) {
			out += candidates[i] + ": " + String(votes[i]) + ",";
		}
		return out;
	}

	void setActive(bool is_active = true) {
		this->is_active = is_active;
	}
	bool isActive() {
		return this->is_active;
	}
};
