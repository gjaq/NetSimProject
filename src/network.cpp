#include "network.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "random.h"


    void Network::resize(const size_t& n) {
		values.clear();
		for (size_t i(0); i < n; ++i) {
			values.push_back(RNG.normal(0, 1));
		}
	}

    bool Network::add_link(const size_t& a, const size_t& b) {
		size_t n(values.size());
		if ((a > (n-1)) or (b > (n-1)) or (a == b)) {
			return false;
		} else {
			bool link_exist(false);
			std::pair <std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> ret;
			ret = links.equal_range(a);
			for (std::multimap<size_t, size_t>::iterator it=ret.first; it!=ret.second; ++it) {
				if (it->second == b) {
					link_exist = true;
				}
			}
		
			if (link_exist) {
				return false;
			} else {
				links.insert({a, b});
				links.insert({b, a});
				return true;
			}
		}
	}
		

    size_t Network::random_connect(const double& n) {
		links.clear();
		size_t size(values.size());
		size_t total_degree(0);
		size_t deg;
		
		for (size_t i(0); i < size; ++i) {
			do {
				deg = RNG.poisson(n);
			} while (deg > size);
			
			if (degree(i) < deg) {
				int n_links(deg - degree(i));
				do {
					std::vector<int> link(n_links);
					RNG.uniform_int(link, 0, size-1);
				
					for (auto n : link) {
						if (add_link(i, n)) {
							--n_links;
						}
					}
				} while (n_links > 0);
				total_degree += deg;
			} else {
				total_degree += degree(i);
			}
		}		
		return total_degree;
	}

    size_t Network::set_values(const std::vector<double>& newvec) {
		values.clear();
		values = newvec;
		return values.size();
	}

    size_t Network::size() const {
		return values.size();
	}

    size_t Network::degree(const size_t &_n) const {
		if (_n > values.size()) {
			throw std::invalid_argument("Argument higher than multimap's size !");
		} else {
			return links.count(_n);
		}
	}

    double Network::value(const size_t &_n) const {
		if (_n > values.size()) {
			throw std::invalid_argument("Argument higher than vector's size !");
		} else {
			return values[_n];
		}
	}

    std::vector<double> Network::sorted_values() const {
		std::vector<double> sortedValues(values);
		
		std::sort(sortedValues.begin(), sortedValues.end(), std::greater<double>());
		return sortedValues;
	}

    std::vector<size_t> Network::neighbors(const size_t& _n) const {
		std::vector<size_t> neighbour;
		for (auto it : links) {
			if (it.first == _n) {
				neighbour.push_back(it.second);
			}
		}
		return neighbour;
	}



