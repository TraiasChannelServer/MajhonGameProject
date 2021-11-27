#pragma once

#include "MJTable.h"


namespace shig {

	class MJManager
	{
	private:
		int identifier;
		int preset_mode;



		shig::MJTable table;

		std::vector<int> player_id;

		std::shared_ptr<bool> test;

	public:

		MJManager();
		~MJManager();



	};




}



