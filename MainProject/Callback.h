#pragma once

#include <vector>
#include <functional>
#include <string>

template<typename T>
class Callback
{
public:
	std::vector<std::function<T>> functions;
	std::vector<std::string> ids;

	void Add(std::function<T> function, std::string id);
	void Add(std::function<T> function, int id);

	void Add(Callback<T> callback);

	void Remove(std::string id);
	void Remove(int id);
	void Clear();
};

template<typename T>
inline void Callback<T>::Add(std::function<T> function, std::string id)
{
	functions.push_back(function);
	ids.push_back(id);
}

template<typename T>
inline void Callback<T>::Add(std::function<T> function, int id)
{
	Add(function, std::to_string(id));
}

template<typename T>
inline void Callback<T>::Add(Callback<T> callback)
{
	for (size_t i = 0; i < callback.ids.size(); i++)
	{
		Add(callback.functions[i], callback.ids[i]);
	}
}

template<typename T>
inline void Callback<T>::Remove(std::string id)
{
	auto it = ids.begin();
	while (it != ids.end())
	{
		it = find(ids.begin(), ids.end(), id);

		if (it != ids.end())
		{
			int index = it - ids.begin();
			ids.erase(ids.begin() + index);
			functions.erase(functions.begin() + index);
		}
	}

}
template<typename T>
inline void Callback<T>::Remove(int id)
{
	Remove(std::to_string(id));
}

template<typename T>
inline void Callback<T>::Clear()
{
	functions.clear();
	ids.clear();
}
