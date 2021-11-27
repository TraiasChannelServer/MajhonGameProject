#include "MJTable.h"

const int minus_one = -1;

namespace shig {

	MJTable::MJTable(const int& pm) 
		:preset_mode(pm)
	
	{
		// preset mode      -> select play style(gamemode)
		// 0 : test mode     how to use this?
		// 1 : tonpu4 mode   -> only 1 round(East), 4hand(kyoku)  sudden death(in to South round)
		// 2 : hantyan4 mode -> 2 round(East+South), 8hand(kyoku) sudden death(in to West round)
		// 3 : tonpu3 mode   -> 3 players on to table  other is same as preset num 1
		// 4 : hantyan3 mode -> 3 players on to table  other is same as preset num 2
		
		//MJ_seed_gen : no point initializing this
		proc_slot = MJPai();
		MJ_engine = std::mt19937(MJ_seed_gen());
		table_slot = std::vector<std::vector<MJPai>>(12, std::vector<MJPai>(0, MJPai())); // id = 0 ~ 11(detail -> header file)
		//shig::MJTable::slot_init(); // (old)reserve slot
		wall_slot = std::vector<MJPai>(0, MJPai()); // id = -1
		wall_slot.reserve(136);
		dead_wall_slot = std::vector<MJPai>(0, MJPai()); // id = ???
		dead_wall_slot.reserve(14);
		pick_slot = std::vector<MJPai>(4, MJPai()); // pick(Tumo) slot init // id = 12 ~ 15(4player)
		player_stick = std::vector<long long>(4, 0);
		deposit_stick = 0; // some point sticks held by the table temporarily
		table_stick = 0; // if some one be no more leader, add +300 points (when 4 player is on) 
		stage = { 0, 0, 0 }; // round, hand 
		player_id = std::vector<int>(4, -1); // koreiru?



	}
	
	bool MJTable::init() {
		


		return true;
	}

	bool MJTable::slot_init() {

		shig_for(i, 0, 4) { // the wall : Yama or Kabehai
			// this might become unused menberV (because wall_slot is the replacement of this menberV)
			// this menberV is prepared for visualizing "Kabepai"(=wall) to the real mahjong
			table_slot.at(i) = std::vector<MJPai>(34, MJPai());
		}

		shig_for(i, 4, 8) { // hand : "tepai"
			table_slot.at(i) = std::vector<MJPai>(18, MJPai());
		}

		shig_for(i, 8, 12) { // inside of the wall : "Kawa" or "sutepai"
			table_slot.at(i) = std::vector<MJPai>(136, MJPai());
		}

		return true;
	}

	bool MJTable::end(int end_flag) {
		
		/*
		if (end_flag == 1) {

		}
		else if (end_flag == 2) {

		}
		*/

		switch (end_flag)
		{
		case 1:
			break;

		case 2:
			break;

		case 3:
			break;

		default:
			break;
		}
		

		return true;
	}

	bool MJTable::reset(int continue_flag) {

		/*
		#define tuple_at(v, i) -> std::get<(i)>((v))  nazobunnpou
		tuple<int, int, int>stage => kaze, kyoku, honnba  
		kaze => East->South(->West->North are minor)
		kyoku => East(1->2->3->4) -> South(1->2->3->4) (case Hantyannsenn(ES) :)
		honnba => if host continues to his play, stay Kaze and Kyoku, and add honnba +1
		
		*/

		// set

		if (continue_flag == 1) {
			tuple_at(stage, 2)++; // n Honnba (host continuing( Renntyann ) -> +1)
		}
		else {
			tuple_at(stage, 2) = 0; // 
			tuple_at(stage, 1)++; // add "kyoku" +1
			if (tuple_at(stage, 1) == 4) { // "kyoku" : 1 to 4(0 to 3) if be 5 -> "kaze" add +1, reset kyoku to 0 (and honnba to 0)
				tuple_at(stage, 1) = 0;
				tuple_at(stage, 0)++; // kaze++
			}
		}
		
		shig::MJTable::set_table();
		
		
		return true;
	}

	bool MJTable::set_table() {

		std::vector<int> id_list(136, 0);
		std::iota(shig_all(id_list), 0);
		std::shuffle(shig_all(id_list), shig::MJTable::MJ_seed_gen);

		if (preset_mode == 1 || preset_mode == 2) {

			/*
			shig_rep(i, 13) { // 0 ~ 51 (13*4)
				table_slot.at(4 + 0).at(i * 4 + 0) = pai_s.at(id_list.at(i * 4 + 0));
				table_slot.at(4 + 0).at(i * 4 + 0).set_bf(0, true);

				table_slot.at(4 + 1).at(i * 4 + 1) = pai_s.at(id_list.at(i * 4 + 1));
				table_slot.at(4 + 1).at(i * 4 + 1).set_bf(0, true);

				table_slot.at(4 + 2).at(i * 4 + 2) = pai_s.at(id_list.at(i * 4 + 2));
				table_slot.at(4 + 2).at(i * 4 + 2).set_bf(0, true);

				table_slot.at(4 + 3).at(i * 4 + 3) = pai_s.at(id_list.at(i * 4 + 3));
				table_slot.at(4 + 3).at(i * 4 + 3).set_bf(0, true);
			}
			*/

			//reset
			wall_slot.clear();
			dead_wall_slot.clear();
			table_slot = std::vector<std::vector<MJPai>>(12, std::vector<MJPai>(0, MJPai()));
			pick_slot = std::vector<MJPai>(4, MJPai());

			shig_for(i, 0, 51) {
				MJPai temp = pai_s.at(id_list.at(i));
				temp.set_bf(0, true);
				int itr = 4 + (i % 4);
				table_slot.at(itr).push_back(temp);
			}


			shig_for(i, 52, 136) {
				
				wall_slot.push_back(pai_s.at(id_list.at(i)));
				if (dead_wall_slot.size() < 14) {
					dead_wall_slot.push_back(pai_s.at(id_list.at(i)));
				}
				//shig::MJLocation temp(-1, i); //wall_slot.at(i).set_locate(temp);
				wall_slot.back().set_locate(MJLocation(-1, i)); //what is the "Sahenti Snannshou" ?
			}


		}

		return true;
	}

	bool MJTable::move(shig::MJLocation& from, shig::MJLocation& to) {

		if (!from.consistency_check() || !to.consistency_check())return false;

		auto [p, i] = to.get_location();

		if (p == -1) {
			wall_slot.at(i) = MJTable::get_pai(from);
		}
		else if (p >= 0 && p < 12) {
			table_slot.at(p).at(i) = MJTable::get_pai(from);
		}
		else if (p >= 12 && p < 16) {
			int itr = p % 4; // 12=>0, 13=>1
			pick_slot.at(itr) = MJTable::get_pai(from);
		}

		auto [pp, ii] = from.get_location();
		MJPai mp = MJTable::get_pai(from);
		//mp.set_bf(1, true); // visible -> invisible or available -> unavailable

		if (pp == -1) {
			wall_slot.at(i) = mp;
			//wall_slot.at(i).set_bf(1, 1);
		}
		else if (p >= 0 && p < 12) {
			table_slot.at(p).at(i) = mp;
		}
		else if (p >= 12 && p < 16) {
			int itr = p % 4; // 12=>0, 13=>1
			pick_slot.at(itr) = mp;
		}



		return true;
	}

	bool MJTable::move_pick(int player) {

		if (wall_slot.size() <= 14)return false;

		pick_slot.at(player) = wall_slot.back();
		wall_slot.pop_back();
		return true;
	}

	bool MJTable::move_discard(int player, int itr) {

		int h_itr = 4 + player; // slot of hand 4~7
		int k_itr = 8 + player; // slot of kawa 8~11

		if (itr == -1) {
			proc_slot = pick_slot.at(player);
			pick_slot.at(player).set_bf(0, false);
		}
		else {
			if (itr < 0 || itr >= (int)table_slot.at(h_itr).size())return false;
			else {
				proc_slot = table_slot.at(h_itr).at(itr);
				//table_slot.at(k_itr).push_back(table_slot.at(h_itr).at(itr));
				table_slot.at(h_itr).at(itr) = pick_slot.at(player);
				pick_slot.at(player).set_bf(0, false);
			}
		}

		return true;
	}

	bool MJTable::move_discard(int player) {

		//if (shig::rng_chk<int>(player, 0, 3))return false; !unsolved external reference!
		if (player < 0 || player < 3)return false;
		int k_itr = 8 + player; // slot of kawa 8~11
		table_slot.at(k_itr).push_back(proc_slot);

		return true;
	}

	bool MJTable::move_calling(int player) {

		//if (shig::rng_chk<int>(player, 0, 3))return false; !unsolved external reference!
		if (player < 0 || player < 3)return false;
		pick_slot.at(player) = proc_slot;
		proc_slot = MJPai();

		return true;
	}

	std::vector<shig::MJPai> MJTable::get_table_slot(const int& slot_id) {
		if (slot_id < -1 || slot_id >= 12) {
			return this->table_slot.at(0);
		}
		else {
			return this->table_slot.at(slot_id);
		}
		
	}

	std::vector<shig::MJPai> MJTable::get_wall_slot() {
		return this->wall_slot;
	}


	std::vector<shig::MJPai> MJTable::get_pick_slot() {
		return this->pick_slot;
	}

	bool MJTable::set_player_id() {
		return true;
	}

	std::vector<int> MJTable::get_player_id() {
		return this->player_id;
	}

	bool MJTable::set_stage(auto&& stage) {
		this->stage = stage;
		return true;
	}

	std::tuple<int, int, int> MJTable::get_stage() {
		return this->stage;
	}
	
	shig::MJPai MJTable::get_pai(shig::MJLocation& locate) {

		if (!locate.consistency_check())return MJPai();

		auto [p, i] = locate.get_location();

		if (p == -1) {
			return wall_slot.at(i);
		}
		else if (p >= 0 && p < 12) {
			return table_slot.at(p).at(i);
		}
		else if (p >= 12 && p < 16) {
			int itr = p % 4; // 12=>0, 13=>1
			return pick_slot.at(itr);
		}

	}
	
	







	MJTable::~MJTable() {

	}


}