#include "MJPai.h"

namespace shig{



	MJPai::MJPai() {
		//bf_size = 8;
		/*
		                                  0/1
		bf[0] => existence flag    non exist/exist
		bf[1] => visible flag      invisible/visible
		bf[2] => orientation flag  vertically/horizontally
		bf[3] => red fives flag    normal/red_fives
		bf[4] =>
		bf[5] =>
		bf[6] =>
		bf[7] =>
		*/
		this->bit_flag = std::bitset<8>("00000000");
		this->kind = 0;
		this->value = 0;
		this->id = -1;
		this->locate = MJLocation(-2, -1);
		this->test = nullptr;
		this->pai_name = "hazure";
	}

	MJPai::MJPai(const int& kind, const int& value, const int& id)
		:kind(kind), value(value), id(id)
	{
		this->bit_flag = std::bitset<8>("00000000");
		this->locate = MJLocation(0, this->id);
		this->test = nullptr;
		this->pai_name = MJPai::set_name();
	}

	bool MJPai::set_bf(int at, bool flag) {
		if (at < 0 || at > bf_size)return false;
		bit_flag.set(at, flag);
		return true;
	}

	std::bitset<8> MJPai::get_bf() {
		return bit_flag;
	}

	bool MJPai::set_state(const std::tuple<int, int, int>& state) {
		auto [a, b, c] = state;
		kind = a; value = b; id = c;
		return true;
	}

	bool MJPai::set_locate(MJLocation locate) {
		if (locate.consistency_check()) {
			this->locate = locate;
			return true;
		}
		else return false;
		
	}

	bool  MJPai::set_name() {
		if (!MJPai::consistency_check())return false;
		pai_name = pai_kind.at(kind) + std::to_string(value);
		return true;
	}

	bool MJPai::consistency_check() {
		if (id < 0 || id >= pai_num)return false; // pai_id = series of digits
		else if (kind < 0 || kind > 3) return false; // m p s d
		else if (kind == 3 && (value < 0 || value > 6)) return false; // d0 ~ d7
		else if ((kind >= 0 && kind < 3) && (value < 0 || value > 8)) return false; // m0 ~ m8
		else return true;
	}

	bool MJPai::operator < (const MJPai& ath) const {
		return std::tie(kind, value) < std::tie(ath.kind, ath.value);
	}

	MJPai& MJPai::operator = (const MJPai& ath) {
		this->bit_flag = ath.bit_flag;
		this->kind = ath.kind; this->value = ath.value; this->id = ath.id; this->locate = ath.locate;
		return *this;
	}


	MJPai::~MJPai() {
		
	}


	





}
