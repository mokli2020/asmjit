// This file is part of AsmJit project <https://asmjit.com>
//
// See <asmjit/core.h> or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#ifndef ASMJIT_CORE_SUPPORT_P_H_INCLUDED
#define ASMJIT_CORE_SUPPORT_P_H_INCLUDED

#include "../core/support.h"
#include "../core/zonevector.h"

ASMJIT_BEGIN_NAMESPACE

//! \cond INTERNAL
//! \addtogroup asmjit_utilities
//! \{

namespace Support {

template<typename IndexType>
class BitWordMutator {
public:
  BitWord _bitWord;

  ASMJIT_INLINE BitWordMutator(BitWord* data, [[maybe_unused]] size_t size) noexcept {
    ASMJIT_ASSERT(size > 0u);
    ASMJIT_ASSERT(size <= bitSizeOf<BitWord>());

    _bitWord = data[0];
  }

  ASMJIT_INLINE BitWordMutator(ZoneBitVector& bw) noexcept
    : BitWordMutator(bw.data(), bw.size()) {}

  [[nodiscard]]
  ASMJIT_INLINE bool bitAt(IndexType index) const noexcept {
    ASMJIT_ASSERT(size_t(index) < bitSizeOf<BitWord>());
    return (_bitWord & (BitWord(1) << size_t(index))) != 0u;
  }

  ASMJIT_INLINE void setBit(IndexType index, bool value) noexcept {
    ASMJIT_ASSERT(size_t(index) < bitSizeOf<BitWord>());

    BitWord clearMask = BitWord(1u) << size_t(index);
    BitWord bitMask = BitWord(value) << size_t(index);

    _bitWord = (_bitWord & ~clearMask) | bitMask;
  }

  ASMJIT_INLINE void addBit(IndexType index, bool value) noexcept {
    ASMJIT_ASSERT(size_t(index) < bitSizeOf<BitWord>());

    BitWord bitMask = BitWord(value) << size_t(index);
    _bitWord |= bitMask;
  }

  ASMJIT_INLINE void clearBit(IndexType index) noexcept {
    ASMJIT_ASSERT(size_t(index) < bitSizeOf<BitWord>());

    BitWord bitMask = BitWord(1) << size_t(index);
    _bitWord &= ~bitMask;
  }

  ASMJIT_INLINE void flipBit(IndexType index) noexcept {
    ASMJIT_ASSERT(size_t(index) < bitSizeOf<BitWord>());

    BitWord bitMask = BitWord(1) << size_t(index);
    _bitWord ^= bitMask;
  }

  ASMJIT_INLINE void commit(BitWord* data, size_t size) const noexcept {
    data[0] = _bitWord;
    DebugUtils::unused(size);
  }

  ASMJIT_INLINE void commit(ZoneBitVector& bw) const noexcept {
    commit(bw.data(), bw.size());
  }
};

template<typename IndexType>
class BitVectorMutator {
public:
  BitWord* _data;
  size_t _size;

  ASMJIT_INLINE BitVectorMutator(BitWord* data, size_t size) noexcept
    : _data(data),
      _size(size) {}

  ASMJIT_INLINE BitVectorMutator(ZoneBitVector& bw) noexcept
    : BitVectorMutator(bw.data(), bw.size()) {}

  [[nodiscard]]
  ASMJIT_INLINE bool bitAt(IndexType index) const noexcept {
    ASMJIT_ASSERT(size_t(index) < _size);
    return bitVectorGetBit(_data, size_t(index));
  }

  ASMJIT_INLINE void setBit(IndexType index, bool value) noexcept {
    ASMJIT_ASSERT(size_t(index) < _size);
    bitVectorSetBit(_data, size_t(index), value);
  }

  ASMJIT_INLINE void addBit(IndexType index, bool value) noexcept {
    ASMJIT_ASSERT(size_t(index) < _size);
    bitVectorOrBit(_data, size_t(index), value);
  }

  ASMJIT_INLINE void clearBit(IndexType index) noexcept {
    ASMJIT_ASSERT(size_t(index) < _size);
    bitVectorSetBit(_data, size_t(index), false);
  }

  ASMJIT_INLINE void flipBit(IndexType index) noexcept {
    ASMJIT_ASSERT(size_t(index) < _size);
    bitVectorFlipBit(_data, size_t(index));
  }

  ASMJIT_INLINE void commit(BitWord* data, size_t size) const noexcept {
    // Does nothing - each operation is written to memory.
    DebugUtils::unused(data, size);
  }

  ASMJIT_INLINE void commit(ZoneBitVector& bw) const noexcept {
    commit(bw.data(), bw.size());
  }
};

} // {Support}

//! \}
//! \endcond

ASMJIT_END_NAMESPACE

#endif // ASMJIT_CORE_SUPPORT_P_H_INCLUDED
