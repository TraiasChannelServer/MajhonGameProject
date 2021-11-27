#pragma once

#include "MJPai.h"
//#include "MJLocation.h"

#define tuple_at(v, i) std::get<(i)>((v))


namespace shig {

	class MJTable
	{
	public:
		const std::vector<shig::MJPai> pai_s = {
			/*
			 MJPai(kind, value, id) 
			 kind => 0:Characters(m)  1:Circles(p)  2:Banboos(s)  3:Honours(d)
			 value 0~9  !exception kind:3 -> 0~7(East, South, West, North, White-dragon, Green-dragon, Red-dragon)
			 "id" means absolutely identifier, distinguishing same name but other pai
			*/
			MJPai(0,1,0),  MJPai(0,1,1),  MJPai(0,1,2),  MJPai(0,1,3),
			MJPai(0,2,4),  MJPai(0,2,5),  MJPai(0,2,6),  MJPai(0,2,7),
			MJPai(0,3,8),  MJPai(0,3,9),  MJPai(0,3,10), MJPai(0,3,11),
			MJPai(0,4,12), MJPai(0,4,13), MJPai(0,4,14), MJPai(0,4,15),
			MJPai(0,5,16), MJPai(0,5,17), MJPai(0,5,18), MJPai(0,5,19),
			MJPai(0,6,20), MJPai(0,6,21), MJPai(0,6,22), MJPai(0,6,23),
			MJPai(0,7,24), MJPai(0,7,25), MJPai(0,7,26), MJPai(0,7,27),
			MJPai(0,8,28), MJPai(0,8,29), MJPai(0,8,30), MJPai(0,8,31),
			MJPai(0,9,32), MJPai(0,9,33), MJPai(0,9,34), MJPai(0,9,35),

			MJPai(1,1,36), MJPai(1,1,37), MJPai(1,1,38), MJPai(1,1,39),
			MJPai(1,2,40), MJPai(1,2,41), MJPai(1,2,42), MJPai(1,2,43),
			MJPai(1,3,44), MJPai(1,3,45), MJPai(1,3,46), MJPai(1,3,47),
			MJPai(1,4,48), MJPai(1,4,49), MJPai(1,4,50), MJPai(1,4,51),
			MJPai(1,5,52), MJPai(1,5,53), MJPai(1,5,54), MJPai(1,5,55),
			MJPai(1,6,56), MJPai(1,6,57), MJPai(1,6,58), MJPai(1,6,59),
			MJPai(1,7,60), MJPai(1,7,61), MJPai(1,7,62), MJPai(1,7,63),
			MJPai(1,8,64), MJPai(1,8,65), MJPai(1,8,66), MJPai(1,8,67),
			MJPai(1,8,68), MJPai(1,8,69), MJPai(1,8,70), MJPai(1,8,71),

			MJPai(2,1,72),  MJPai(2,1,73),  MJPai(2,1,74),  MJPai(2,1,75),
			MJPai(2,2,76),  MJPai(2,2,77),  MJPai(2,2,78),  MJPai(2,2,79),
			MJPai(2,3,80),  MJPai(2,3,81),  MJPai(2,3,82),  MJPai(2,3,83),
			MJPai(2,4,84),  MJPai(2,4,85),  MJPai(2,4,86),  MJPai(2,4,87),
			MJPai(2,5,88),  MJPai(2,5,89),  MJPai(2,5,90),  MJPai(2,5,91),
			MJPai(2,6,92),  MJPai(2,6,93),  MJPai(2,6,94),  MJPai(2,6,95),
			MJPai(2,6,96),  MJPai(2,6,97),  MJPai(2,6,98),  MJPai(2,6,99),
			MJPai(2,8,100), MJPai(2,8,101), MJPai(2,8,102), MJPai(2,8,103),
			MJPai(2,9,104), MJPai(2,9,105), MJPai(2,9,106), MJPai(2,9,107),

			MJPai(3,1,108), MJPai(3,1,109), MJPai(3,1,110), MJPai(3,1,111),
			MJPai(3,2,112), MJPai(3,2,113), MJPai(3,2,114), MJPai(3,2,115),
			MJPai(3,3,116), MJPai(3,3,117), MJPai(3,3,118), MJPai(3,3,119),
			MJPai(3,4,120), MJPai(3,4,121), MJPai(3,4,122), MJPai(3,4,123),
			MJPai(3,5,124), MJPai(3,5,125), MJPai(3,5,126), MJPai(3,5,127),
			MJPai(3,6,128), MJPai(3,6,129), MJPai(3,6,130), MJPai(3,6,131),
			MJPai(3,7,132), MJPai(3,7,133), MJPai(3,7,134), MJPai(3,7,135),

		};

	private:
		int preset_mode;
		std::random_device MJ_seed_gen;
		std::mt19937 MJ_engine;
		
		shig::MJPai proc_slot;
		std::vector<std::vector<shig::MJPai>> table_slot;
		std::vector<shig::MJPai> wall_slot;
		std::vector<shig::MJPai> dead_wall_slot;
		std::vector<shig::MJPai> pick_slot;
		std::vector<long long> player_stick;
		long long deposit_stick;
		long long table_stick;
		std::tuple<int, int, int> stage;
		std::vector<int> player_id;


	public:
		MJTable(const int& pm);
		~MJTable();


		
		bool init();
		bool slot_init();
		bool end(int end_flag);
		bool reset(int continue_flag);
		bool set_table();
		bool move(shig::MJLocation& from, shig::MJLocation& to);
		bool move_pick(int player);
		bool move_discard(int player, int itr);
		bool move_discard(int player);
		bool move_calling(int player);
		std::vector<shig::MJPai> get_table_slot(const int& slot_id);
		std::vector<shig::MJPai> get_wall_slot();
		std::vector<shig::MJPai> get_pick_slot();
		bool set_player_id();
		std::vector<int> get_player_id();
		bool set_stage(auto&& stage);
		std::tuple<int, int, int> get_stage();
		shig::MJPai get_pai(shig::MJLocation& locate);
		




	};




}



