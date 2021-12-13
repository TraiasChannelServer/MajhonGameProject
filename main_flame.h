#pragma once

#include "resource.h"

#include <dwmapi.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
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
#include <thread>
#include <functional>
#include <optional>


#define shig_for(i, a, b) for(int i = (a); i < (b); ++i)
#define shig_rep(i, n) shig_for(i, 0, n)
#define shig_forB(bit,a,b) for(int bit = (a); bit < (1<<(b)); ++bit)
#define shig_repB(bit,n) shig_forB(bit,0,n)
#define shig_all(v) (v).begin(), (v).end()