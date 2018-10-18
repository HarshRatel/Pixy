#pragma once

#include "stdafx.h"
#include <vector>
#include <map>

template<typename K, typename V>
std::vector<K> GetKeys(std::map<K,V> map)
{
	std::vector<K> result(map.size);

	for (auto value : map)
		result.push_back(value.first);

	return result;
}

template<typename K, typename V>
std::vector<V> GetValues(const std::map<K, V> &map)
{
	std::vector<V> result;

	for (auto value : map)
		result.push_back(value.second);

	return result;
}