//
// Created by FyS on 25/03/18.
//

#ifndef FREESOULS_WORLD_MEMORYPOOL_HPP
#define FREESOULS_WORLD_MEMORYPOOL_HPP

#include <spdlog/spdlog.h>
#include <vector>
#include <variant>

namespace fys::mem {

    template <typename T>
    class MemoryPool {

        union PoolUnit {
            int nextFree;
            T poolElem;
        };

        public:
            MemoryPool(int poolSize) : _poolSize(poolSize), _firstFree(0), _vector(static_cast<unsigned>(poolSize)) {

                for (int i = 0; i < poolSize; ++i) {
                    _vector.at(i).poolElem.setPoolIndex(i);
                    _vector.at(i).nextFree = i + 1;
                }
                _vector.at(poolSize - 1).nextFree = -1;
            }

            T &create() {
                int firstFree = _firstFree;
                _firstFree = _vector.at(firstFree).nextFree;
                return _vector.at(firstFree).poolElem;
            }

            void clearFromPool(int indexInMemPool) {
                if (indexInMemPool < _poolSize) {
                    _vector.at(indexInMemPool).nextFree = _firstFree;
                    _firstFree = indexInMemPool;
                }
            }

        private:
            int _poolSize;
            int _firstFree;
            std::vector<PoolUnit> _vector;
    };

}

#endif //FREESOULS_WORLD_MEMORYPOOL_HPP
