
#ifndef READONLY_CLAU_PARSER_H
#define READONLY_CLAU_PARSER_H

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <string>
#include <list>

#include <fstream>

#include <algorithm>

#include <thread>

namespace wiz {
	template <typename T>
	inline T pos_1(const T x, const int base = 10)
	{
		if (x >= 0) { return x % base; }// x - ( x / 10 ) * 10; }
		else { return (x / base) * base - x; }
		// -( x - ( (x/10) * 10 ) )
	}


	template <typename T> /// T <- char, int, long, long long...
	std::string toStr(const T x) /// chk!!
	{
		const int base = 10;
		if (base < 2 || base > 16) { return "base is not valid"; }
		T i = x;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 NULL, 1 minus
		std::string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE + 1] = '\0'; //

		for (k = INT_SIZE; k >= 1; k--) {
			T val = pos_1<T>(i, base); /// 0 ~ base-1
									   /// number to ['0'~'9'] or ['A'~'F']
			if (val < 10) { temp[k] = val + '0'; }
			else { temp[k] = val - 10 + 'A'; }

			i /= base;

			if (0 == i) { // 
				k--;
				break;
			}
		}

		if (isMinus) {
			temp[k] = '-';
			tempString = std::string(temp + k);//
		}
		else {
			tempString = std::string(temp + k + 1); //
		}
		delete[] temp;

		return tempString;
	}


	class LoadDataOption
	{
	public:
		char LineComment = '#';	// # 
		char Left = '{', Right = '}';	// { }
		char Assignment = '=';	// = 
		char Removal = ' ';		// ',' 
	};

	inline bool isWhitespace(const char ch)
	{
		switch (ch)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\v':
		case '\f':
			return true;
			break;
		}
		return false;
	}


	inline int Equal(const long long x, const long long y)
	{
		if (x == y) {
			return 0;
		}
		return -1;
	}

	class Scanner
	{
	private:
		long long start;
		long long last;
	public:
		const wiz::LoadDataOption* option;
		long long* token_arr;
		long long* token_arr_len;
		long long num;
		int* err;
		long long* arr_count;
		long long count;
		long long arr_count_size;
		char* buffer;
	public:
		Scanner(long long start, long long last, const wiz::LoadDataOption* option,
			long long* token_arr, long long num, long long* token_arr_len, char* buffer, int* err, long long* arr_count, long long count, long long arr_count_size)
			: start(start), last(last), option(option), buffer(buffer), err(err), arr_count(arr_count), count(count), arr_count_size(arr_count_size)
		{
			this->token_arr = token_arr;
			this->num = num;
			this->token_arr_len = token_arr_len;
		}
		~Scanner() {
			//
		}
	public:
		void operator() () {
			//arr_count = nullptr;
			if (true) {
				long long token_arr_count = 0;
				long long start_idx = 0;
				long long last_idx = 0;
				long long token_first = start;
				long long token_last = start - 1; //?
				int state = 0;

				long long now_idx = 0;

				for (long long i = 0; start + i < last; ++i, ++now_idx) {
					const char x = buffer[start + i];
					const int _x = start + i;
					long long offset = 0;
					int idx;

					if (arr_count_size > 0 && count >= 0 && count < arr_count_size && _x == arr_count[count]) {
						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;
							{
								if (token_last - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}
						}

						start_idx = i;
						token_first = arr_count[count];
						token_last = arr_count[count + 1]; // count : even index;


						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;
						}

						token_last = token_last + 1;
						token_first = token_last;

						i += (arr_count[count + 1] - arr_count[count]);

						now_idx = i;
						start_idx = i + 1;
						last_idx = i + 1;

						count += 2;

						continue;
					}
					else if (0 == state && -1 != (idx = Equal(option->Removal, x)))
					{
						token_last = _x - 1;
						last_idx = now_idx - 1;

						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;

							{
								if (token_last - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}

							token_first = _x + 1;
							start_idx = now_idx + 1;
						}
						else {
							token_first = token_first + 1;
							start_idx = start_idx + 1;
						}
						continue;
					}
					else if (0 == state && -1 != (idx = Equal(option->Assignment, x))) {
						token_last = _x - 1;
						last_idx = now_idx - 1;

						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;

							{
								if (token_last - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}

							token_first = _x + 1;
							start_idx = now_idx + 1;

							token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 3;
							token_arr_count++;
						}
						else {
							token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 3;
							token_arr_count++;

							token_first = token_first + 1;
							start_idx = start_idx + 1;
						}
					}
					else if (0 == state && isWhitespace(x)) { // isspace ' ' \t \r \n , etc... ?
						token_last = _x - 1;
						last_idx = now_idx - 1;
						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;

							{
								if (token_last - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}

							token_first = _x + 1;

							start_idx = now_idx + 1;
						}
						else
						{
							token_first = token_first + 1;
							start_idx = start_idx + 1;
						}
					}
					else if (0 == state && -1 != (idx = Equal(option->Left, x))) {
						token_last = _x - 1;
						last_idx = now_idx - 1;
						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;

							{
								if (token_last - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}

							token_first = _x + 1;

							start_idx = now_idx + 1;

							token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 1;

							token_arr_count++;
						}
						else {
							token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 1;
							token_arr_count++;

							token_first = token_first + 1;
							start_idx = start_idx + 1;
						}
					}
					else if (0 == state && -1 != (idx = Equal(option->Right, x))) {
						token_last = _x - 1;
						last_idx = now_idx - 1;
						if (token_last - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
							token_arr_count++;
							{
								if (token_last - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}

							token_first = _x + 1;
							start_idx = now_idx + 1;

							token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 2;
							token_arr_count++;
						}
						else {
							token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 2;
							token_arr_count++;

							token_first = token_first + 1;
							start_idx = start_idx + 1;
						}
					}
					else {
						//
					}

					token_last = _x + offset;

					last_idx = now_idx + offset;

				}

				if (token_first < last)
				{
					if (last - 1 - token_first + 1 > 0) {
						token_arr[token_arr_count] = ((start_idx + num) << 32) + ((last - 1 - token_first + 1) << 2) + 0;
						token_arr_count++;

						{
							if (last - 1 - token_first + 1 == 1) {
								if (buffer[start + start_idx] == option->Left) {
									token_arr[token_arr_count - 1] += 1;
								}
								if (buffer[start + start_idx] == option->Right) {
									token_arr[token_arr_count - 1] += 2;
								}
								if (buffer[start + start_idx] == option->Assignment) {
									token_arr[token_arr_count - 1] += 3;
								}
							}
						}
					}
				}

				if (this->token_arr_len) {
					*(this->token_arr_len) = token_arr_count;
				}

				if (state != 0) {
					*err = -3;
					return; // throw "Scanning Error in qouted string";
				}

				*err = 0;
				return;
			}
		}
	};

	class InFileReserver
	{
	private:
		static void _func(const int dif, const char* text, const int length, long long* arr, long long* arr_count) {
			int _arr_count = 0;

			for (int i = 0; i < length; ++i) {
				switch (text[i]) {
				case '\"':
				case '\\':
					//case '=':
				case '\n':
				case '#': // line comment start char
					arr[_arr_count] = 1 + i + dif; // after called this function, arr[index] += -1?
					_arr_count += 1;
					break;
				}
			}

			*arr_count = _arr_count;
		}

		static void func(char* text, const int length, long long*& _arr_count, const int thr_num, long long& _arr_count_size) {
			long long* arr = (long long*)calloc(length + 1, sizeof(long long));// long[length];, null -> +1
			long long* arr_count = new long long[thr_num];
			long long* arr_start = new long long[thr_num];
			long count = -2;

			for (int i = 0; i < thr_num; ++i) {
				arr_start[i] = length / thr_num * i;
			}

			{
				std::thread* thr = new std::thread[thr_num];
				for (int i = 0; i < thr_num - 1; ++i) {
					thr[i] = std::thread(_func, length / thr_num * i, text + length / thr_num * i, length / thr_num, arr + length / thr_num * i, arr_count + i);
				}
				int last_length = length - length / thr_num * (thr_num - 1);
				thr[thr_num - 1] = std::thread(_func, length / thr_num * (thr_num - 1), text + length / thr_num * (thr_num - 1), last_length, arr + length / thr_num * (thr_num - 1), arr_count + thr_num - 1);
				for (int i = 0; i < thr_num; ++i) {
					thr[i].join();
				}

				delete[] thr;
			}
			{ // debug
				//for (int i = 0; i < length; ++i) {
				//	std::cout << arr[i] << " ";
				//}
				//std::cout << "\n";
			}

			{
				int _count = 0;
				for (int i = 0; i < thr_num; ++i) {
					for (int j = 0, k = 0; j < arr_count[i]; ++j, ++k) {
						// pass zero.. 
						if (0 == arr[arr_start[i] + k]) {
							--j;
							break;
						}
						arr[_count] = arr[arr_start[i] + k] - 1; // chk this.
						_count++;
					}
				}
				count = _count;
			}

			{ // debug
				//std::cout << "debug2\n";
				//for (int i = 0; i < count; ++i) {
				//	std::cout << arr[i] << " ";
				//}
				//std::cout << "\n";
			}

			{ // todo - add error check!
				int _count = 0;
				int state = 0;
				int line_comment_start = 0;

				arr[count] = length; // text[arr[count]] == '\0'
				for (long long i = 0; i <= count; ++i) {
					const char ch = text[arr[i]];

					if (0 == state) {
						if ('#' == ch) {
							state = 3;
							line_comment_start = arr[i];
						}
						else if ('\"' == ch) {
							state = 1;

							arr[_count] = arr[i];

							// for abc"def"ghi
							for (long long j = arr[i]; j >= 0; --j) {
								bool pass = false;
								switch (text[j]) {
								case ' ':
								case '\t':
								case '\r':
								case '\n':
								case '\v':
								case '\f':
								case '\0':
								case '{':
								case '}':
								case '=':
									pass = true;
									break;
								}
								if (pass) {
									arr[_count] = j + 1;
									break;
								}
								if (0 == j) {
									arr[_count] = 0;
								}
							}
							_count++;
						}
					}
					else if (1 == state) {
						if ('\\' == ch) {
							state = 2;
						}
						else if ('\"' == ch) {
							state = 0;

							arr[_count] = arr[i];

							for (long long j = arr[i]; j <= length; ++j) {
								bool pass = false;
								switch (text[j]) {
								case ' ':
								case '\t':
								case '\r':
								case '\n':
								case '\v':
								case '\f':
								case '\0':
								case '{':
								case '}':
								case '=':
									pass = true;
									break;
								}
								if (pass) {
									arr[_count] = j - 1;
									break;
								}
							}

							_count++;
						}
					}
					else if (2 == state) {
						state = 1;
					}
					else if (3 == state) {
						if ('\n' == ch || '\0' == ch) {
							for (int j = line_comment_start; j < arr[i]; ++j) {
								text[j] = ' ';
							}

							state = 0;
						}
					}
				}

				count = _count;

				// odd case. -> error, count must be even!
				if (count % 2 == 1) {
					std::cout << "[" << count << "] " << "valid \"`s num is not even.\n"; //
				}
			}

			{ //debug
				//for (int i = 0; i < count; ++i) {
				//	std::cout << arr[i] << " ";
				//}
				//std::cout << "\n";
			}

			{
				delete[] arr_start;
				delete[] arr_count;
				_arr_count = arr;
				_arr_count_size = count;
			}
		}


		static std::pair<bool, int> Scan(std::ifstream& inFile, const int num, bool* isFirst, const wiz::LoadDataOption& option, int thr_num,
			char*& _buffer, long long* _buffer_len, long long*& _token_arr, long long* _token_arr_len)
		{
			if (inFile.eof()) {
				return { false, 0 };
			}

			long long* arr_count = nullptr; //
			long long arr_count_size = 0;

			std::string temp;
			char* buffer = nullptr;
			std::vector<long long> start(thr_num + 1, 0);
			std::vector<long long> last(thr_num + 1, 0);
			std::vector<long long> token_arr_len(thr_num + 1, 0);
			long long file_length;

			if (thr_num > 1) {
				inFile.seekg(0, inFile.end);
				unsigned long long length = inFile.tellg();
				inFile.seekg(0, inFile.beg);

				file_length = length;
				buffer = new char[file_length + 1]; // 

				// read data as a block:
				inFile.read(buffer, file_length);

				buffer[file_length] = '\0';

				{
					//int a = clock();
					func(buffer, file_length, arr_count, thr_num, arr_count_size); // todo - rename
					//int b = clock();
				//	std::cout << b - a << "ms\n";
				}


				start[0] = 0;

				for (int i = 1; i < thr_num; ++i) {
					start[i] = file_length / thr_num * i;

					for (long long x = start[i]; x <= file_length; ++x) {
						if (isWhitespace(buffer[x]) || '\0' == buffer[x] ||
							option.Left == buffer[x] || option.Right == buffer[x] || option.Assignment == buffer[x]) {
							start[i] = x;
							break;
						}
					}
				}
				for (int i = 0; i < thr_num - 1; ++i) {
					last[i] = start[i + 1];
					for (long long x = last[i]; x <= file_length; ++x) {
						if (isWhitespace(buffer[x]) || '\0' == buffer[x] ||
							option.Left == buffer[x] || option.Right == buffer[x] || option.Assignment == buffer[x]) {
							last[i] = x;
							break;
						}
					}
				}
				last[thr_num - 1] = file_length;

			}
			else {
				inFile.seekg(0, inFile.end);
				unsigned long long length = inFile.tellg();
				inFile.seekg(0, inFile.beg);

				file_length = length;
				buffer = new char[file_length + 1]; // 

				// read data as a block:
				inFile.read(buffer, file_length);
				buffer[file_length] = '\0';

				start[0] = 0;
				last[0] = file_length;

				{
					//int a = clock();
					func(buffer, file_length, arr_count, thr_num, arr_count_size);
					//int b = clock();
				//	std::cout << b - a << "ms\n";
				}

			}

			long long* token_arr = nullptr;

			if (thr_num > 1) {
				std::vector<std::thread> thr(thr_num);

				token_arr = new long long[file_length];

				std::vector<long long> counter(thr_num, 0);
				std::vector<int> err(thr_num, 0);

				std::vector<long long> count(thr_num, -1); // start_count?

				{
					long long start_count = 0;

					for (int i = 0; i < thr_num; ++i) {
						if (i > 0) {
							start[i] = last[i - 1];
							if (last[i] < start[i]) {
								last[i] = start[i];
							}
						}
						bool pass = false;
						for (; start_count < arr_count_size; start_count += 2) {
							if (!pass && start[i] <= arr_count[start_count] && arr_count[start_count] < last[i]) {
								count[i] = start_count;

								if (last[i] <= arr_count[count[i] + 1]) {
									last[i] = arr_count[count[i] + 1] + 1;
								}
								pass = true;
							}
							else if (arr_count[start_count] < last[i]) {
								if (last[i] <= arr_count[start_count + 1]) {
									last[i] = arr_count[start_count + 1] + 1;
									break;
								}
							}
							else if (arr_count[start_count] >= last[i]) {
								break;
							}
						}
					}
				}
				{
					//	for (int i = 0; i < thr_num; ++i) {
					//		std::cout << start[i] << " " << last[i] << "\n";
					//	}
				}

				for (int i = 0; i < thr_num; ++i) {
					thr[i] = std::thread(Scanner(start[i], last[i], &option,
						token_arr + start[i], start[i], &counter[i], buffer, &err[i], arr_count, count[i], arr_count_size));
				}

				for (int i = 0; i < thr_num; ++i) {
					thr[i].join();
				}

				{
					free(arr_count);//free(arr);
				}

				for (int i = 0; i < thr_num; ++i) {
					switch (err[i]) {
					case 0:
						// no errors.
						break;
					case -1:
						std::cout << "\'\' has enter key\n";
						break;
					case -2:
						std::cout << "\"\" has enter key\n";
						break;
					case -3:
						std::cout << "Scanning Error in qouted string\n";
						break;
					default:
						std::cout << "unknown scnner error\n";
						break;
					}
				}

				long long sum = 0;

				for (int i = 1; i < counter.size(); ++i) {
					sum += counter[i - 1];

					memmove(token_arr + sum, token_arr + start[i], counter[i] * sizeof(long long) / sizeof(char));
				}

				*_token_arr_len = sum + counter.back();
			}
			else {
				token_arr = new long long[file_length];

				long long len;
				int err = 0;
				long long count = 0;

				Scanner scanner(start[0], last[0], &option, token_arr, start[0], &len, buffer, &err, arr_count, count, arr_count_size);

				scanner();

				{
					free(arr_count);//free(arr);
				}

				switch (err) {
				case 0:
					// no errors.
					break;
				case -1:
					std::cout << "\'\' has enter key\n";
					break;
				case -2:
					std::cout << "\"\" has enter key\n";
					break;
				case -3:
					std::cout << "Scanning Error in qouted string\n";
					break;
				default:
					std::cout << "unknown scnner error\n";
					break;

				}
				*_token_arr_len = len;
			}

			_buffer = buffer;
			_token_arr = token_arr;
			*_buffer_len = file_length;

			return{ true, 1 };
		}

	private:
		std::ifstream* pInFile;
		bool isFirst;
	public:
		int Num;
	public:
		explicit InFileReserver(std::ifstream& inFile)
		{
			pInFile = &inFile;
			Num = 1;
			isFirst = true;
		}
		bool end()const { return pInFile->eof(); } //
	public:
		bool operator() (const wiz::LoadDataOption& option, int thr_num, char*& buffer, long long* buffer_len, long long*& token_arr, long long* token_arr_len)
		{
			bool x = Scan(*pInFile, Num, &isFirst, option, thr_num, buffer, buffer_len, token_arr, token_arr_len).second > 0;
			return x;
		}
	};




	class Utility {
	public:
		static long long GetIdx(long long x) {
			return (x >> 32) & 0x00000000FFFFFFFF;
		}
		static long long GetLength(long long x) {
			return (x & 0x00000000FFFFFFFC) >> 2;
		}
		static long long GetType(long long x) {
			return x & 3;
		}
	};
	
	class Node;

	class MemoryPool
	{
		public:
			Node* arr = nullptr;
		//	Node static_else_list[10]; // todo - real depth in real data? < 10 ?
			std::list<Node*> else_list;
			long long count = 0;
			long long size = 0;
		public:
			MemoryPool() {
				//
			}
		public:
			Node* Get();
		public:
			virtual ~MemoryPool(); //

			void Clear();
	};
	

	class Node {
	public:
			
	public:
		long type = 2; // 1 itemtype, 2 usertype, -1 virtual node and usertype.
		long long name = 0;
		long long value = 0;
	public:
		Node* first = nullptr;
		Node* last = nullptr;
		Node* parent = nullptr;
		Node* child = nullptr;
		Node* next = nullptr;
	public:
		Node() {
			first = this;
			last = this;
		}
	public:
		Node* GetParent() {
			return this->first->parent;
		}

		// usertype
		void AddItem(long long var, long long val, MemoryPool& pool) {
			if (type != 2 && type != -1) {
				return;
			}


			if (nullptr == this->child) {
				this->child = MakeNode(pool);
				this->child->first = this->child;
				this->child->last = this->child;
				this->child->parent = this;
				this->child->type = 1;
				this->child->name = var;
				this->child->value = val;
			}
			else {
				Node* temp = MakeNode(pool);

				this->child->first->last->next = temp;
				this->child->first->last = temp;
				temp->first = this->child->first;
				temp->type = 1;
				temp->name = var;
				temp->value = val;
			}
		}

		Node* AddUserTypeItem(MemoryPool& pool, long long var = 0) {
			if (type != 2 && type != -1) {
				return nullptr;
			}

			if (nullptr == this->child) {
				this->child = MakeNode(pool);
				this->child->first = this->child;
				this->child->last = this->child;
				this->child->parent = this;
				this->child->type = 2;
				this->child->name = var;
				
				return this->child;
			}
			else {
				Node* temp = MakeNode(pool);

				this->child->first->last->next = temp;
				this->child->first->last = temp;
				temp->first = this->child->first;
				temp->type = 2;
				temp->name = var;

				return temp;
			}
		}

		Node* AddVirtualNode(MemoryPool& pool) {
			Node* temp = AddUserTypeItem(pool);
			if (temp) {
				temp->type = -1;
			}

			return temp;
		}

		// itemtype
		
	public:
	
		static Node* MakeNode(MemoryPool& pool) {	
			return pool.Get(); // using memory pool.
		}

		void Save(std::ostream& stream, char* buffer) const {
			Save(stream, buffer, this);
		}
	private:
		void Save(std::ostream& stream, char* buffer, const Node* node) const {
			do {
				if (!node) {
					return;
				}

				if (Utility::GetLength(node->name) != 0) {
					stream << std::string(Utility::GetIdx(node->name) + buffer, Utility::GetLength(node->name)) << " = ";
				}
				if (node->type == -1) {
					stream << "# = { \n";
				}
				else if (node->type == 2 && node != this) {
					stream << " { \n";
				}
				else if (node->type == 1) {
					stream << std::string(Utility::GetIdx(node->value) + buffer, Utility::GetLength(node->value)) << " ";
				}

				if (node->type == -1 || node->type == 2) {
					Save(stream, buffer, node->child);
					if (node != this) {
						stream << " } \n ";
					}
				}
				node = node->next;
			} while (node != nullptr);
		}

	public:
		void Link(Node* x) {
			// this - usertype,  x - node..
			if (this->child) {
				this->child->last->next = x;
				this->child->last = x->first->last;
				x->parent = nullptr;
			}
			else {
				this->child = x;
				this->child->first = x;
				x->parent = this;
				x->last = x->first->last;
			}

			for (Node* iter = x; iter != nullptr; iter = iter->next) {
				iter->first = this->child;
			}
		}
	};

	

	// LoadDat
	class LoadData
	{
	private:
		static long long check_syntax_error1(long long str) {
			long long len = Utility::GetLength(str);
			long long type = Utility::GetType(str);

			if (1 == len && (type == 1 || type == 2 ||
				type == 3)) {
				throw "check syntax error 1 : " + str;
			}
			return str;
		}
		static int Merge(Node* next, Node* ut, Node** ut_next)
		{
			//check!!
			while (ut->child
				&& ut->child->type == -1)
			{
				ut = ut->child;
			}

			while (true) {
				Node* _ut = ut;
				Node* _next = next;

				if (ut_next && _ut == *(ut_next)) { // check..
					*(ut_next) = next;
				}

				{
					Node* x = _ut->child;
					if (x) {
						if (x->type == -1) {
							Node* temp = x->next;
							Node* last = x->last;
							// delete x; //
							x = temp;

							if (x) {
								x->last = last;
							}
						}
						if (x) {
							x->first = x;
							_next->Link(x);
							_ut->child = nullptr;
						}
					}
				}

				ut = ut->GetParent();
				next = next->GetParent();


				if (next && ut) {
					//
				}
				else {
					// right_depth > left_depth
					if (!next && ut) {
						return -1;
					}
					else if (next && !ut) {
						return 1;
					}

					return 0;
				}
			}
		}

		
	private:
		static bool __LoadData(const char* buffer, const long long* token_arr, long long token_arr_len, Node* _global, const wiz::LoadDataOption* _option,
			int start_state, int last_state, Node** next, MemoryPool* _pool) // first, strVec.empty() must be true!!
		{
			{
				long long count_left = 0;
				long long count_right = 0;
				long long count_eq = 0;
				long long count_other = 0; // count_other

				for (long long x = 0; x < token_arr_len; ++x) {
					switch (Utility::GetType(token_arr[x]))
					{
					case 0:
						count_other++;
						break;
					case 1:
						count_left++;
						break;
					case 2:
						count_right++;
						break;
					case 3:
						count_eq++;
						break;
					}
				}

				// chk count_ - count_eq + count_left < 0 ? -
				// count_ = count_other..
				long long chkNum = count_other - count_eq + count_left;

				if (count_right > count_left) { // maybe has virtual node? - support some virutal node
					// cf) }}} {{{   right == left but, # of virtual node is 3.
					chkNum += count_right - count_left;
				}
				if (chkNum < 0) {
					_pool->arr = nullptr;
					_pool->count = 0;
					_pool->size = 0;
				}
				else {
					_pool->arr = new Node[1 + chkNum];
					_pool->count = 0;
					_pool->size = 1 + chkNum;
				}
			}

			MemoryPool& pool = *_pool;

			std::vector<long long> varVec;
			std::vector<long long> valVec;


			if (token_arr_len <= 0) {
				return false;
			}

			const wiz::LoadDataOption& option = *_option;

			int state = start_state;
			int braceNum = 0;
			std::vector< Node* > nestedUT(1);
			long long var = 0, val = 0;

			nestedUT.reserve(10);
			nestedUT[0] = _global;


			long long count = 0;
			const long long* x = token_arr;
			const long long* x_next = x;

			for (long long i = 0; i < token_arr_len; ++i) {
				x = x_next;
				{
					x_next = x + 1;
				}
				if (count > 0) {
					count--;
					continue;
				}
				long long len = Utility::GetLength(token_arr[i]);

				switch (state)
				{
				case 0:
				{
					// Left 1
					if (len == 1 && (-1 != Equal(1, Utility::GetType(token_arr[i])) || -1 != Equal(1, Utility::GetType(token_arr[i])))) {
						if (!varVec.empty()) {
							for (long long x = 0; x < varVec.size(); ++x) {
								nestedUT[braceNum]->AddItem(varVec[x], valVec[x], pool);
							}

							varVec.clear();
							valVec.clear();
						}

						Node* pTemp = nullptr;

						{
							pTemp = nestedUT[braceNum]->AddUserTypeItem(pool);
						}
						

						braceNum++;

						/// new nestedUT
						if (nestedUT.size() == braceNum) { /// changed 2014.01.23..
							nestedUT.push_back(nullptr);
						}

						/// initial new nestedUT.
						nestedUT[braceNum] = pTemp;
						///

						state = 0;
					}
					// Right 2
					else if (len == 1 && (-1 != Equal(2, Utility::GetType(token_arr[i])) || -1 != Equal(2, Utility::GetType(token_arr[i])))) {
						state = 0;

						if (!varVec.empty()) {
							for (long long x = 0; x < varVec.size(); ++x) {
								nestedUT[braceNum]->AddItem(varVec[x], valVec[x], pool);
							}

							varVec.clear();
							valVec.clear();
						}

						if (braceNum == 0) {
							Node ut;
							Node* temp = ut.AddVirtualNode(pool);

							temp->child = nestedUT[0]->child;
							temp->parent = nestedUT[0];
							nestedUT[0]->child->first->parent = temp;
							nestedUT[0]->child = temp;

							braceNum++;
						}

						{
							if (braceNum < nestedUT.size()) {
								nestedUT[braceNum] = nullptr;
							}
							braceNum--;
						}
					}
					else {
						if (x < token_arr + token_arr_len - 1) {
							long long _len = Utility::GetLength(token_arr[i + 1]);
							// EQ 3
							if (_len == 1 && -1 != Equal(3, Utility::GetType(token_arr[i + 1]))) {
								var = token_arr[i];

								state = 1;

								{
									count = 1;
								}
							}
							else {
								// var1
								if (x <= token_arr + token_arr_len - 1) {

									val = token_arr[i];

									varVec.push_back(check_syntax_error1(var));
									valVec.push_back(check_syntax_error1(val));

									val = 0;

									state = 0;

								}
							}
						}
						else
						{
							// var1
							if (x <= token_arr + token_arr_len - 1)
							{
								val = token_arr[i];
								varVec.push_back(check_syntax_error1(var));
								valVec.push_back(check_syntax_error1(val));
								val = 0;

								state = 0;
							}
						}
					}
				}
				break;
				case 1:
				{
					// LEFT 1
					if (len == 1 && (-1 != Equal(1, Utility::GetType(token_arr[i])) || -1 != Equal(1, Utility::GetType(token_arr[i])))) {
						for (long long x = 0; x < varVec.size(); ++x) {
							nestedUT[braceNum]->AddItem(varVec[x], valVec[x], pool);
						}


						varVec.clear();
						valVec.clear();

						///
						{
							Node* pTemp;

							pTemp = nestedUT[braceNum]->AddUserTypeItem(pool, var);
							


							var = 0;
							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) {
								nestedUT.push_back(nullptr);
							}

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
						}
						///
						state = 0;
					}
					else {
						if (x <= token_arr + token_arr_len - 1) {
							val = token_arr[i];

							varVec.push_back(check_syntax_error1(var));
							valVec.push_back(check_syntax_error1(val));
							var = 0; val = 0;

							state = 0;
						}
					}
				}
				break;
				default:
					// syntax err!!
					throw "syntax error ";
					break;
				}
			}

			if (varVec.empty() == false) {
				for (long long x = 0; x < varVec.size(); ++x) {
					nestedUT[braceNum]->AddItem(varVec[x], valVec[x], pool);
				}

				varVec.clear();
				valVec.clear();
			}

			if (next) {
				*next = nestedUT[braceNum];
			}

			if (state != last_state) {
				throw std::string("error final state is not last_state!  : ") + toStr(state);
			}
			if (x > token_arr + token_arr_len) {
				throw std::string("error x > buffer + buffer_len: ");
			}

			return true;
		}

		static long long FindDivisionPlace(const char* buffer, const long long* token_arr, long long start, long long last, const wiz::LoadDataOption& option)
		{
			for (long long a = last; a >= start; --a) {
				long long len = Utility::GetLength(token_arr[a]);
				long long val = Utility::GetType(token_arr[a]);


				if (len == 1 && (-1 != Equal(2, val) || -1 != Equal(2, val))) { // right
					return a;
				}

				bool pass = false;
				if (len == 1 && (-1 != Equal(1, val) || -1 != Equal(1, val))) { // left
					return a;
				}
				else if (len == 1 && -1 != Equal(3, val)) { // assignment
					//
					pass = true;
				}

				if (a < last && pass == false) {
					long long len = Utility::GetLength(token_arr[a + 1]);
					long long val = Utility::GetType(token_arr[a + 1]);

					if (!(len == 1 && -1 != Equal(3, val))) // assignment
					{ // NOT
						return a;
					}
				}
			}
			return -1;
		}

		static bool _LoadData(InFileReserver& reserver, Node* global, const wiz::LoadDataOption option, char** _buffer, std::vector<wiz::MemoryPool>* _pool, const int lex_thr_num, const int parse_num) // first, strVec.empty() must be true!!
		{
			const int pivot_num = parse_num - 1;
			char* buffer = nullptr;
			long long* token_arr = nullptr;
			long long buffer_total_len;
			long long token_arr_len = 0;

			{
				bool success = reserver(option, lex_thr_num, buffer, &buffer_total_len, token_arr, &token_arr_len);

				if (!success) {
					return false;
				}
				if (token_arr_len <= 0) {
					return true;
				}
			}


			Node* before_next = nullptr;
			Node _global;

			bool first = true;
			long long sum = 0;

			{
				std::set<long long> _pivots;
				std::vector<long long> pivots;
				const long long num = token_arr_len; //

				if (pivot_num > 0) {
					std::vector<long long> pivot;
					pivots.reserve(pivot_num);
					pivot.reserve(pivot_num);

					for (int i = 0; i < pivot_num; ++i) {
						pivot.push_back(FindDivisionPlace(buffer, token_arr, (num / (pivot_num + 1)) * (i), (num / (pivot_num + 1)) * (i + 1) - 1, option));
					}

					for (int i = 0; i < pivot.size(); ++i) {
						if (pivot[i] != -1) {
							_pivots.insert(pivot[i]);
						}
					}

					for (auto& x : _pivots) {
						pivots.push_back(x);
					}
				}

				std::vector<Node*> next(pivots.size() + 1, nullptr);

				{
					std::vector<MemoryPool> pool(pivots.size() + 1);

					std::vector<Node> __global(pivots.size() + 1);

					std::vector<std::thread> thr(pivots.size() + 1);

					{
						long long idx = pivots.empty() ? num - 1 : pivots[0];
						long long _token_arr_len = idx - 0 + 1;


						thr[0] = std::thread(__LoadData, buffer, token_arr, _token_arr_len, &__global[0], &option, 0, 0, &next[0], &pool[0]);
					}

					for (int i = 1; i < pivots.size(); ++i) {
						long long _token_arr_len = pivots[i] - (pivots[i - 1] + 1) + 1;

						thr[i] = std::thread(__LoadData, buffer, token_arr + pivots[i - 1] + 1, _token_arr_len, &__global[i], &option, 0, 0, &next[i], &pool[i]);

					}

					if (pivots.size() >= 1) {
						long long _token_arr_len = num - 1 - (pivots.back() + 1) + 1;

						thr[pivots.size()] = std::thread(__LoadData, buffer, token_arr + pivots.back() + 1, _token_arr_len, &__global[pivots.size()],
							&option, 0, 0, &next[pivots.size()], &pool[pivots.size()]);
					}

					// wait
					for (int i = 0; i < thr.size(); ++i) {
						thr[i].join();
					}

					// Merge
					try { // chk empty global?
						if (__global[0].child && __global[0].child->type == -1) {
							std::cout << "not valid file1\n";
							throw 1;
						}
						if (next.back()->GetParent() && next.back()->GetParent()->GetParent() != nullptr) {
							std::cout << "not valid file2\n";
							throw 2;
						}
						
						/*
						SaveWizDB(__global[0], buffer, "0.txt");
						SaveWizDB(__global[1], buffer, "1.txt");
						SaveWizDB(__global[2], buffer, "2.txt");
						SaveWizDB(__global[3], buffer, "3.txt");
						SaveWizDB(__global[4], buffer, "4.txt");
						SaveWizDB(__global[5], buffer, "5.txt");
						SaveWizDB(__global[6], buffer, "6.txt");
						SaveWizDB(__global[7], buffer, "7.txt");
						*/

						for (int i = 1; i < pivots.size() + 1; ++i) {
							// linearly merge and error check...
							int err = Merge(next[i - 1], &__global[i], &next[i]);
							if (-1 == err) {
								std::cout << "not valid file4\n";
								throw 4;
							}
							else if (i == pivots.size() && 1 == err) {
								std::cout << "not valid file5\n";
								throw 5;
							}
						}

						_global = __global[0];
						*_pool = pool;
					}
					catch (...) {
						delete[] token_arr;
						delete[] buffer;
						for (auto& x : pool) {
							x.Clear();
						}
						buffer = nullptr;
						throw "in Merge, error";
					}

					before_next = next.back();
				}
			}

			delete[] token_arr;

			*_buffer = buffer;
			*global = _global;

			return true;
		}
	public:
		static bool LoadDataFromFile(const std::string& fileName, Node* global, char** _buffer, std::vector<wiz::MemoryPool>* pool, int lex_thr_num = 0, int parse_num = 0) /// global should be empty
		{
			if (lex_thr_num <= 0) {
				lex_thr_num = std::thread::hardware_concurrency();
			}
			if (lex_thr_num <= 0) {
				lex_thr_num = 1;
			}

			if (parse_num <= 0) {
				parse_num = std::thread::hardware_concurrency();
			}
			if (parse_num <= 0) {
				parse_num = 1;
			}

			bool success = true;
			std::ifstream inFile;
			inFile.open(fileName, std::ios::binary);

			if (true == inFile.fail())
			{
				inFile.close(); return false;
			}

			Node globalTemp;

			try {

				InFileReserver ifReserver(inFile);
				wiz::LoadDataOption option;
				option.Assignment = ('=');
				option.Left = '{';
				option.Right = '}';
				option.LineComment = ('#');
				option.Removal = ' '; // ','

				ifReserver.Num = 1 << 19;
				//	strVec.reserve(ifReserver.Num);
				// cf) empty file..
				if (false == _LoadData(ifReserver, &globalTemp, option, _buffer, pool, lex_thr_num, parse_num))
				{
					inFile.close();
					return false; // return true?
				}

				inFile.close();
			}
			catch (const char* err) {
				std::cout << err << "\n"; inFile.close();
				for (auto& x : *pool) {
					x.Clear();
				}
				pool->clear();
				return false;
			}
			catch (const std::string & e) {
				std::cout << e << "\n"; inFile.close();
				for (auto& x : *pool) {
					x.Clear();
				}
				pool->clear();
				return false;
			}
			catch (std::exception e) {
				std::cout << e.what() << "\n"; inFile.close();
				for (auto& x : *pool) {
					x.Clear();
				}
				pool->clear();
				return false;
			}
			catch (...) {
				std::cout << "not expected error" << "\n"; inFile.close();
				for (auto& x : *pool) {
					x.Clear();
				}
				pool->clear();
				return false;
			}


			*global = globalTemp;
			return true;
		}
		static bool LoadWizDB(Node* global, const std::string& fileName, char** buffer, std::vector<wiz::MemoryPool>* pool, const int thr_num) {
			Node globalTemp;

			// Scan + Parse 
			if (false == LoadDataFromFile(fileName, &globalTemp, buffer, pool, thr_num, thr_num)) { return false; }
			//std::cout << "LoadData End" << "\n";

			*global = globalTemp;
			return true;
		}
		// SaveQuery
		static bool SaveWizDB(const Node& global, char* buffer, const std::string& fileName, const bool append = false) {
			std::ofstream outFile;
			if (fileName.empty()) { return false; }
			if (false == append) {
				outFile.open(fileName);
				if (outFile.fail()) { return false; }
			}
			else {
				outFile.open(fileName, std::ios::app);
				if (outFile.fail()) { return false; }

				outFile << "\n";
			}

			/// saveFile
			global.Save(outFile, buffer); // cf) friend

			outFile.close();

			return true;
		}
	};

}


#endif

