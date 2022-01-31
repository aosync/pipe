#pragma once 
#include <vector>
#include <functional>
#include <queue>
#include <stack>
#include <iostream>

template <typename C>
class Pipe;

template <typename C>
class PipeChk {
	Pipe<C> &m_pipe;
	bool m_backtrack;
public:
	PipeChk(Pipe<C> &pipe) : m_pipe(pipe), m_backtrack(true) {
		pipe.checkpoint();
	}
	~PipeChk() {
		if (m_backtrack)
			m_pipe.backtrack();
		else
			m_pipe.flush();
	}
	void backtrack() {
		m_backtrack = true;
	}
	void flush() {
		m_backtrack = false;
	}
};

template <typename C>
class Pipe {
	size_t cursor;
	std::vector<C> buf;
	std::vector<size_t> mem;
	//std::queue<C> buf;
	//std::vector<std::vector<C>> mem;
	std::function<C()> fetchf;
public:
	Pipe(std::function<C()> fetch) : cursor(0), fetchf(fetch) {}
	virtual ~Pipe() = default;
	C get() {
		/*C next;
		if (!buf.empty()) {
			next = buf.front();
			buf.pop();
		} else
			next = fetch();

		if (!mem.empty())
			mem.back().push_back(C(next));

		return next; */

		if (cursor >= buf.size())
			buf.push_back(fetch());

		return buf[cursor++];
	}
	C peek() {

		/* if (buf.empty())
			buf.push(fetch());

		return buf.front();*/

		if (cursor >= buf.size())
			buf.push_back(fetch());

		return buf[cursor];
	}

	virtual C fetch() {
		return fetchf();
	}

	PipeChk<C> chk() {
		return PipeChk<C>(*this);
	}
	void checkpoint() {
		//mem.push_back(std::vector<C>());
		mem.push_back(cursor);
	}

	void flush() {
		/* if (mem.size() <= 1) {
			mem.pop_back();
			return;
		}
		auto pen = &mem[mem.size() - 2];
		for (auto it = mem.back().begin(); it != mem.back().end(); it++)
			pen->push_back(C(*it));
		mem.pop_back(); */
		mem.pop_back();
	}

	void backtrack() {
		/* 
		for (auto it = mem.back().begin(); it != mem.back().end(); it++)
			buf.push(C(*it));
		mem.pop_back(); */
		cursor = mem.back();
		mem.pop_back();
	}


	friend PipeChk<C>;
};
