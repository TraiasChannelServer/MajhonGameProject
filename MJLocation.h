#pragma once

#include "shig_utlity.h"



namespace shig {

	class MJLocation
	{
	private:
		const int pai_num = 136;
		int place;
		int id;
	public:
		MJLocation(int p, int i);
		MJLocation();
		~MJLocation();
		bool set_location(const int& place, const int& id);
		std::pair<int, int> get_location();
		bool consistency_check();

		MJLocation& operator = (const MJLocation& ath);




	};


	MJLocation make_MJL(const int& p, const int& i);


}



