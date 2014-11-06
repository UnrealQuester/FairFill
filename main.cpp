#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <numeric>
#include <sstream> 

using namespace std;

class Bucket {

public:

    Bucket(int max, int actual) : max(max), actual(actual){}
    bool isEmpty() {
        return actual == 0;
    }

    bool isFull() {
        return actual == max;
    }

    int max;
    int actual;
};

const Bucket NullBucket(0, 0);

typedef std::vector<Bucket> WineBucket;

bool operator==(const Bucket &a, const Bucket &b) {
    return a.max == b.max && a.actual == b.actual;
}

const bool operator<(const Bucket &a, const Bucket &b) {
    if(a.max == b.max) {
        return a.actual < b.actual;
    } else {
        return a.max < b.max;
    }
}

Bucket operator+(const Bucket &a, const Bucket &b) {
    return Bucket(a.max + b.max, a.actual + b.actual);
}

struct Exchange {
    int von;
    int zu;
};


class Steps {
    public:
    Steps(std::vector<Exchange> steps, bool valid) : steps(steps), valid(valid){}
    std::vector<Exchange> steps;
    bool valid;
};

bool operator<(const Steps &a, const Steps &b) {
    if(!b.valid)
        return true;
    if(!a.valid)
        return false;
    return a.steps.size() < b.steps.size();
}

Steps null = Steps(std::vector<Exchange>(), false);

void exchange(Bucket &a, Bucket &b) {
    int diff = b.max - b.actual;
    if(diff > a.actual) {
        b.actual += a.actual;
        a.actual = 0;
    } else {
        b.actual = b.max;
        a.actual -= diff;
    }
}

bool isBalanced(const WineBucket &bucket, int first) {
    return (
        std::accumulate(bucket.begin(), bucket.begin()+first, NullBucket).actual
        == std::accumulate(bucket.begin()+first, bucket.end(), NullBucket).actual);
}

void printBucket(const WineBucket &bucket) {
    for(auto a = bucket.begin(); a != bucket.end(); a++) {
        cout << (*a).actual << "(" << (*a).max << ") ";
    }
	cout << endl;
}

void printExchanges(Steps steps, WineBucket bucket) {
    for(int i = steps.steps.size()-1; i >= 0; i--) {
		printBucket(bucket);
        cout << "Put the contents of bucket number "
			 << (steps.steps[i].von+1)
			 << " into bucket number "
			 << (steps.steps[i].zu+1)
			 <<  endl;
		exchange(bucket[steps.steps[i].von], bucket[steps.steps[i].zu]);
    }
	printBucket(bucket);
}

Steps solve(WineBucket bucket, int first, std::map<WineBucket, int> &map, int depth) {
    auto iter = map.find(bucket);
    if(iter != map.end()) {
        if((*iter).second <= depth)
        return null;
    }
    map[bucket] = depth;
    if(isBalanced(bucket, first))
        return Steps(std::vector<Exchange>(), true);
    std::vector<Exchange> exchanges;
    for(int i = 0; i < bucket.size(); i++) {
        for(int j = 0; j < bucket.size(); j++) {
            if(i == j) {
                continue;
            }
            if(bucket[i].isEmpty() || bucket[j].isFull()) {
                continue;
            }
            Exchange ex = {i, j};
            exchanges.push_back(ex);
        }
    }
    Steps shortest = null;
    for(int i = 0; i < exchanges.size(); i++) {
        WineBucket b = bucket;
        exchange(b[exchanges[i].von], b[exchanges[i].zu]);
        Steps solution = solve(b, first, map, depth+1);
        if(solution.valid)
            solution.steps.push_back(exchanges[i]);
        if(solution < shortest)
            shortest = solution;
    }
    return shortest;
}

void test(WineBucket bucket, int first) {
	cout << "Please wait ..." << endl;
    std::map<WineBucket, int> map;
    auto exchange = solve(bucket, first, map, 0);
	if(!exchange.valid) {
		cout << "No solution!" << endl;
		return;
	}
	if(exchange.steps.size() == 0) {
		cout << "Already evenly distributed!" << endl;
		return;
	}
	cout << exchange.steps.size() << " Schritte" << endl;
    printExchanges(exchange, bucket);
    cout << endl << endl;
}

int main() {
	int first;
	int n;
	vector<int> capacity;
	vector<int> content;
	WineBucket bucket;
	string line;
	cout << "Please enter the number of buckets the first guy has" << endl;
	cin >> first;
	cout << "Please enter the maximum capacity of the buckets" << endl;
	getline(cin,line);
	getline(cin,line);
	stringstream ss = stringstream(line);
	while(ss >> n) {
		if(n < 1) {
			cout << "The capacity must be at least 1" << endl;
			exit(1);
		}
		capacity.push_back(n);
	}
	cout << "Please enter the content of the buckets" << endl;
	getline(cin,line);
	ss = stringstream(line);
	while(ss >> n) {
		if(n < 0) {
			cout << "You may only enter natural numbers" << endl;
			exit(1);
		}
		content.push_back(n);
	}
	for(int i = 0; i < content.size() && i < capacity.size(); i++) {
		if(capacity[i] < content[i]) {
			cout << "A Bucket must be able to hold its contents" << endl;
			exit(1);
		}
		bucket.push_back(Bucket(capacity[i], content[i]));
	}
	if(first < 1 || first >= bucket.size()) {
		cout << "The first guy has to have between 1 and " << bucket.size() -1 << " buckets" << endl;
		exit(1);
	}
	test(bucket, first);
	return 0;
}
