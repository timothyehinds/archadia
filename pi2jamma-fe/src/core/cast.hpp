#pragma once

template<typename T, typename R>
T downCast(R r) {
	#ifdef DEBUG
		return dynamic_cast<T>(r);
	#else
		return static_cast<T>(r);
	#endif
}

template<typename T >
T safeDeRef(T* ppT) {
	if(nullptr == ppT) {
		return  nullptr;
	}

	return (*ppT);
}