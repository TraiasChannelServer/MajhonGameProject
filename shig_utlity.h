#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <numeric>
#include <queue>
#include <deque>
#include <random>
#include <bitset>
#include <tuple>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <memory>
#include <thread>
#include <functional>
#include <optional>


#define shig_for(i, a, b) for(int i = (a); i < (b); ++i)
#define shig_rep(i, n) shig_for(i, 0, n)
#define shig_forB(bit,a,b) for(int bit = (a); bit < (1<<(b)); ++bit)
#define shig_repB(bit,n) shig_forB(bit,0,n)
#define shig_all(v) (v).begin(), (v).end()
#define shig_IFOR(i, a, b) (i) < (a) || (i) > (b)

namespace shig {

	template <class T>
	bool rng_chk(T v, T l, T r);

}




