#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class domino {
	/* constants and types */
	private:
		static constexpr unsigned alphabet = 'z' - 'a' + 1; //the length of the alphabet
		static constexpr unsigned tail = alphabet; //word end symbol
		static constexpr unsigned symbol_size = tail + 1;

		/* [from_char][to_char][at position] => word index */
		using roots_node = std::unordered_map<unsigned, std::unordered_set<unsigned>>;
		using roots_tree = roots_node [symbol_size][symbol_size];
		//using roots_tree = std::unordered_map<unsigned, std::unordered_set<unsigned>> [symbol_size][symbol_size];
		
		/* [word index] => [overlapping] => {other word index} (one that goes *before* the word) */
		using database = std::unordered_map<unsigned, std::unordered_map<unsigned, std::vector<unsigned>>>;

	/* data members */
	private:
		roots_tree roots;
		database db;

	/* public interface */
	public:
		void process(char const **words) {
			build_tree(words);	
			build_database(words);
		}

		void print(std::ostream &os) const {
			for (unsigned j = 0; j < symbol_size; ++j) {
				for (unsigned i = 0; i < symbol_size; ++i) {
					bool first = true;
					for (auto &e: roots[i][j]) {
						if (first) {
							char current_char = (char)(i + 'a');
							char next_char = (j == tail) ? '$' : (char)(j + 'a');

							os << current_char << " => " << next_char << ": " << std::endl;
							first = false;
						}

						os << "\t[" << e.first << "]: ";

						for (auto &s: e.second) {
							os << s << " ";
						}

						os << std::endl;
					}
				}
			}

			for (auto &i: db) {
				std::cout << i.first << ": " << std::endl;

				for (auto &j: i.second) {
					std::cout << "\t[" << j.first << "]: ";

					for (auto &k: j.second) {
						std::cout << k << " ";
					}

					std::cout << std::endl;
				}
			}
		}

	/* private methods */
	private:
		void build_tree(char const **words) {
			for (unsigned word = 0; words[word] != nullptr; ++word) {
				for (unsigned letter = 0; words[word][letter] != 0; ++letter) {
					char current = words[word][letter], next = words[word][letter + 1];
					unsigned const current_index = (unsigned)(current - 'a');
					unsigned const next_index = (unsigned)((next == 0) ? (char)tail : next - 'a');

					for (unsigned position = 0; position <= letter; ++position) {

						//std::cout << "Adding element to [" << current_index << "][" << next_index << "][" << position << "]: " << word << std::endl;
						
						roots[current_index][next_index][position].insert(word);
					}
				}
			}
		}

		void build_database(char const **words) {
			for (unsigned word = 0; words[word] != nullptr; ++word) {

				for (unsigned letter = 0; words[word][letter] != 0; ++letter) {
					char current = words[word][letter], next = words[word][letter + 1];
					unsigned const current_index = (unsigned)(current - 'a');
					unsigned const next_index = (unsigned)((next == 0) ? tail : next - 'a');

					std::cout << "Looking for " << current << " => " << next << std::endl;
					
					if (letter == 0) {
						if (roots[current_index][tail][0].size() > 0) {
							for (auto &e: roots[current_index][tail][0]) {
								db[word][1].push_back(e);
								std::cout << "Found a one-letter overlap" << std::endl;
							}
						}
					}

					if (next_index > 200) {
						continue;
					}
				}
			}
		}
};

std::ostream &operator << (std::ostream &os, domino const &d) {
	d.print(os);
	return os;
}

int main() {
	char const *words[] = {"galamb", "alma", "manga", "hang", nullptr};

	domino d;

	d.process(words);

	std::cout << d << std::endl;

	return 0;
}

