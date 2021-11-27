#pragma once

#include "MJLocation.h"




namespace shig {

	class MJPai
	{
	private:
		const int bf_size = 8;
		const int pai_num = 136;
		const std::vector<char> pai_kind = {'m', 'p', 's', 'd'};

		std::bitset<8> bit_flag;
		int kind;
		int value;
		int id;
		MJLocation locate;
		char* test;
		std::string pai_name;

	public:
		MJPai();
		MJPai(const int& kind, const int& value, const int& id);
		~MJPai();
		bool set_bf(int at, bool flag);
		std::bitset<8> get_bf();
		bool set_state(const std::tuple<int, int, int>& state);
		bool set_locate(MJLocation locate);
		bool set_name();
		bool consistency_check();
		bool operator < (const MJPai& ath) const;
		MJPai& operator = (const MJPai& ath);

	protected:

	};


}



