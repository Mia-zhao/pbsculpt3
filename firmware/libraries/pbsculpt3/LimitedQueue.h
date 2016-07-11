/*
 * LimitedQueue.h
 *
 *  Created on: Jun 20, 2016
 *      Author: dkadish
 */

#ifndef LIMITEDQUEUE_H_
#define LIMITEDQUEUE_H_

#include "LinkedList.h"
#include "tools.h"

#define MAXLENGTH_DEFAULT 10

/** Limits the size of a queue, evicting the oldest elements when necessary
 *
 */
template<typename T> class LimitedQueue: public LinkedList<T> {
public:
	LimitedQueue(int length);
	LimitedQueue();

	int getMaxLength();
	void setMaxLength(int length);

	/** Push an item on to the back of the queue
	 *
	 * @param _t
	 * @return
	 */
	bool push(T _t);
	bool add(int index, T _t);
	bool add(T _t);

protected:
	int _maxLength;
	void _shrinkQueue();
};

template<typename T> LimitedQueue<T>::LimitedQueue(int length):
	LinkedList<T>(), _maxLength(length) {

}

template<typename T> LimitedQueue<T>::LimitedQueue():
	LinkedList<T>(), _maxLength(MAXLENGTH_DEFAULT) {

}


template<typename T> int LimitedQueue<T>::getMaxLength(){
	return _maxLength;
}

/**
 *
 * @todo IMPLEMENTED INCORRECTLY. Should use push and shift for queue.
 *
 * @param _t
 * @return
 */
template<typename T> bool LimitedQueue<T>::push(T _t){
	bool result = this->add(_t);

	return result;
}

template<typename T> bool LimitedQueue<T>::add(T _t){
	bool result = LinkedList<T>::add(_t);

	_shrinkQueue();

	return result;
}

template<typename T> bool LimitedQueue<T>::add(int index, T _t){
	bool result = LinkedList<T>::add(index, _t);

	_shrinkQueue();

	return result;
}

template<typename T> void LimitedQueue<T>::_shrinkQueue(){
	//DBGF("LimitedQueue", "Queue size: %d", this->size() );
	while(this->size() > _maxLength ){
		this->pop();
	}
}

#endif /* LIMITEDQUEUE_H_ */
