#include "MJLocation.h"

namespace shig {


	MJLocation::MJLocation() {
		this->place = -2;
		this->id = -1;
	}

	MJLocation::MJLocation(int p, int i)
		: place(p), id(i)
	{
		if (!MJLocation::consistency_check()) {
			place = -2;
			id = -1;
		}
	}

	bool MJLocation::set_location(const int& place, const int& id) {
		this->place = place;
		this->id = id;
		if (!MJLocation::consistency_check()) {
			this->place = -2;
			this->id = -1;
		}
		return true;
	}

	bool MJLocation::consistency_check() {
		if (place < -1 || place > 15) return false; // place -1~15
		else if ((place == -1) && (id < 0 || id >= 136))return false; // wall_slot
		else if ((place > 0 && place <= 4) && (id < 0 || id >= 34))return false; // wall_slot(for visual exec)
		else if ((place > 4 && place <= 8) && (id < 0 || id >= 18))return false; // player_slot
		else if ((place > 8 && place <= 12) && (id < 0 || id >= 136))return false; // kawa(inside of wall)
		else if ((place > 12 && place <= 16) && (id < 0 || id >= 1))return false; // player's pick_slot
		else return true;
	}

	std::pair<int, int> MJLocation::get_location() {
		return std::make_pair(this->place, this->id);
	}

	MJLocation& MJLocation::operator = (const MJLocation& ath) {
		this->place = ath.place;
		this->id = ath.place;
		return *this;
	}



	MJLocation::~MJLocation() {

	}

	MJLocation make_MJL(const int& p, const int& i) {
		return MJLocation(p, i);
	}


}