#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <numeric>

using namespace std;

class Becher {

public:

	Becher(int max, int actual) : max(max), actual(actual){}
	bool isEmpty() {
		return actual == 0;
	}

	bool isFull() {
		return actual == max;
	}



	int max;
	int actual;
};

const Becher NullBecher(0, 0);

typedef std::vector<Becher> WeinBecher;

bool operator==(const Becher &a, const Becher &b) {
	return a.max == b.max && a.actual == b.actual;
}

const bool operator<(const Becher &a, const Becher &b) {
	if(a.max == b.max) {
		return a.actual < b.actual;
	} else {
		return a.max < b.max;
	}
}

Becher operator+(const Becher &a, const Becher &b) {
	return Becher(a.max + b.max, a.actual + b.actual);
}

void exchange(Becher &a, Becher &b) {
	int diff = b.max - b.actual;
	if(diff > a.actual) {
		b.actual += a.actual;
		a.actual = 0;
	} else {
		b.actual = b.max;
		a.actual -= diff;
	}
}

bool isBalanced(const WeinBecher &becher, int first) {
	return (
		std::accumulate(becher.begin(), becher.begin()+first, NullBecher).actual
		== std::accumulate(becher.begin()+(first+1), becher.end(), NullBecher).actual);
}

void printBecher(const WeinBecher &becher) {
	for(auto a = becher.begin(); a != becher.end(); a++) {
		cout << (*a).actual << "(" << (*a).max << ") ";
	}
}

bool tryExchange(Becher &a, Becher &b, WeinBecher &becher, std::vector<WeinBecher> &states) {
	auto oldstate = becher;
	exchange(a, b);
	if(std::find(states.begin(), states.end(), becher) != states.end()) {
		becher = oldstate;
		return false;
	}
	states.push_back(becher);
	return true;
}

struct Tausch {
	int von;
	int zu;
};


class Abfolge {
	public:
	Abfolge(std::vector<Tausch> abfolge, bool valid) : abfolge(abfolge), valid(valid){}
	std::vector<Tausch> abfolge;
	bool valid;
};

bool operator<(const Abfolge &a, const Abfolge &b) {
	if(!b.valid)
		return true;
	if(!a.valid)
		return false;
	return a.abfolge.size() < b.abfolge.size();
}

Abfolge null = Abfolge(std::vector<Tausch>(), false);

void printExchanges(Abfolge abfolge) {
	for(int i = 0; i < abfolge.abfolge.size(); i++) {
		cout << abfolge.abfolge[i].von << " " << abfolge.abfolge[i].zu << endl;
	}
}

Abfolge umfullen(WeinBecher becher, int first, std::map<WeinBecher, int> &map, int depth) {
	auto iter = map.find(becher);
	//printBecher(becher);
	//cout << endl;
	if(iter != map.end()) {
		if((*iter).second <= depth)
		return null;
	}
	map[becher] = depth;
	if(isBalanced(becher, first))
		return Abfolge(std::vector<Tausch>(), true);
	std::vector<Tausch> tausche;
	for(int i = 0; i < becher.size(); i++) {
		for(int j = 0; j < becher.size(); j++) {
			if(i == j) {
				continue;
			}
			if(becher[i].isEmpty() || becher[j].isFull()) {
				continue;
			}
			Tausch tausch = {i, j};
			tausche.push_back(tausch);
		}
	}
	for(int i = 0; i < tausche.size(); i++) {
		//cout << tausche[i].von << " " << tausche[i].zu << endl;
	}
	Abfolge shortest = null;
	for(int i = 0; i < tausche.size(); i++) {
		WeinBecher b = becher;
		exchange(b[tausche[i].von], b[tausche[i].zu]);
		Abfolge solution = umfullen(b, first, map, depth+1);
		//cout << solution.valid << endl;
		//printExchanges(solution);
		//cout << endl;
		if(solution.valid)
			solution.abfolge.push_back(tausche[i]);
		if(solution < shortest)
			shortest = solution;
	}
	return shortest;
}

void test(WeinBecher becher, int first, bool expected) {
	auto tausche = umfullen(becher, first, std::map<WeinBecher, int>(), 0);
	printExchanges(tausche);
	cout << endl << endl;
	exit(0);
}

void main() {
	{
		WeinBecher becher;
		becher.push_back(Becher(8, 8));
		becher.push_back(Becher(5, 0));
		becher.push_back(Becher(3, 0));
		test(becher, 1, true);
	}
	{
		WeinBecher becher;
		becher.push_back(Becher(5, 0));
		becher.push_back(Becher(8, 8));
		becher.push_back(Becher(3, 0));
		becher.push_back(Becher(1, 0));
		test(becher, 1, true);
	}
	{
		WeinBecher becher;
		becher.push_back(Becher(5, 0));
		becher.push_back(Becher(10, 10));
		becher.push_back(Becher(3, 0));
		becher.push_back(Becher(1, 0));
		test(becher, 1, true);
	}
	{
		WeinBecher becher;
		becher.push_back(Becher(3, 0));
		becher.push_back(Becher(20, 20));
		becher.push_back(Becher(1, 1));
		becher.push_back(Becher(2, 0));
		test(becher, 1, false);
	}
	{
		WeinBecher becher;
		becher.push_back(Becher(4, 4));
		becher.push_back(Becher(4, 4));
		becher.push_back(Becher(2, 0));
		becher.push_back(Becher(2, 0));
		becher.push_back(Becher(2, 0));
		becher.push_back(Becher(2, 0));
		test(becher, 1, true);
	}
	{
		WeinBecher becher;
		becher.push_back(Becher(10, 10));
		becher.push_back(Becher(8, 4));
		becher.push_back(Becher(11, 4));
		becher.push_back(Becher(7, 6));
		test(becher, 1, false);
	}
	{
		WeinBecher becher;
		becher.push_back(Becher(6, 0));
		becher.push_back(Becher(26, 0));
		becher.push_back(Becher(13, 0));
		becher.push_back(Becher(50, 20));
		test(becher, 2, true);
	}
}
