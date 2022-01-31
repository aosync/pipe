#pragma once

#include <stack>
#include <functional>
#include <queue>
#include <iostream>

template <typename C>
class Pipe {
	std::queue<C> buf;
	std::stack<std::queue<C>> mem;
	std::function<C()> fetchf;
public:
	Pipe(std::function<C()> fetch) : fetchf(fetch) {}
	virtual ~Pipe() = default;
	C get() {
		C next;
		if (!buf.empty()) {
			next = buf.front();
			buf.pop();
		} else
			next = fetch();
		
		if (!mem.empty())
			mem.top().push(std::move(next));

		return next;
	}
	C peek() {
		if (!buf.empty())
			return buf.front();
		
		buf.push(fetch());
		return buf.front();
	}

	virtual C fetch() {
		return fetchf();
	}
	
	void checkpoint() {
		std::queue<C> top;
		mem.push(top);
	}

	void flush() {
		mem.pop();
	}

	void backtrack() {
		while (!mem.top().empty()) {
			buf.push(mem.top().front());
			mem.top().pop();
		}
		flush();
	}
};
