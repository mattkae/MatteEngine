#pragma once
#include <functional>
#include <vector>

template <typename T>
struct ObjectPoolItem {
	bool isTaken;
	T value;
};

template <class T>
class ObjectPool { 
public:
	std::function<void(ObjectPoolItem<T>&)> onGet = [](ObjectPoolItem<T>& t) {};
	std::function<void(ObjectPoolItem<T>&)> onRet = [](ObjectPoolItem<T>& t) {};
	ObjectPool() {
		this->values.resize(ObjectPool::DEFAULT_SIZE);
	}

	~ObjectPool() {
		values.clear();
	}

	ObjectPoolItem<T> &get() {
		int foundIndex = -1;
		for (int vIndex = 0; vIndex < values.size(); vIndex++) {
			if (values[vIndex].isTaken == false) {
				foundIndex = vIndex;
				break;
			}
		}

		if (foundIndex == -1) {
			int prevSize = values.size();
			values.resize(prevSize * 2);
			foundIndex = prevSize;
		}

		ObjectPoolItem<T>& item = values[foundIndex];
		item.isTaken = true;
		onGet(item);
		return item;
	};

	void ret(ObjectPoolItem<T> &value) {
		onRet(value);

		for (auto& v : values) {
			if (&v == &value) {
				v.isTaken = false;
				break;
			}
		}
	};

	std::vector<ObjectPoolItem<T>>& getValues() {
		return values;
	}

	const std::vector<ObjectPoolItem<T>>& getConstValues() const {
		return values;
	}
private:
	static const int DEFAULT_SIZE = 5;
    std::vector<ObjectPoolItem<T>> values;
};