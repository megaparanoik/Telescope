#ifndef RING_HPP_
#define RING_HPP_

#include <memory>
#include <cstring>

using namespace std;

template <class T>
class RingBuff
{
private:
    unique_ptr<T[]> buffer;
    int buf_size;
    int head;
    int tail;
    int count;
public:
    RingBuff(int size):buffer(unique_ptr<T[]>(new T[size])), buf_size(size)
    {
        Init();
    }

    ~RingBuff() {}

    int Init()
    {
        std::memset(&buffer[0], 0, buf_size);
        head = 0;
        tail = 0;
        count = 0;
        return 0;
    }

    int Push(T element)
    {
        if (count == buf_size) {
            return 0;
        }

        buffer[tail] = element;
        tail = (tail + 1) % buf_size;
        count++;

        return 1;
    }

	int Pop(T *element) {
		if (count == 0) {
			return 0;
		}
		*element = buffer[head];
		head = (head + 1) % buf_size;
		count--;

		return 1;
	}

    int Pop(T *element, int size) {
		if (count == 0) {
			return 0;
		}

		int i;
		for(i=0; i<size; i++) {
			element[i] = buffer[head];
			head = (head + 1) % buf_size;
			if ((--count) == 0) {
				i++;
				break;
			}
		}
		return i;
	}

    int GetCount()
    {
    	return count;
    }

    int Peek(T *element)
    {
		if (count == 0) {
			return 0;
		}

		*element = buffer[head];
		return 1;
    }

    int Peek(T *element, int size)
    {
		if (count == 0) {
			return 0;
		}

		int i;
		for(i=0; i<size; i++) {
			element[i] = buffer[head+i];
			if(i == count) {
				break;
			}
		}

		return i;
    }

};

#endif /* RING_HPP_ */
