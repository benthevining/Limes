/*
 * ======================================================================================
 *  __    ____  __  __  ____  ___
 * (  )  (_  _)(  \/  )( ___)/ __)
 *  )(__  _)(_  )    (  )__) \__ \
 * (____)(____)(_/\/\_)(____)(___/
 *
 *  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include "../limes_namespace.h"
#include <limes_export.h>
#include "../misc/preprocessor.h"
#include <vector>
#include "./array_pointer.h"

/** @file
	This file defines the md_array_pointer class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

/** This class is an RAII version of a multidimensional C-style array.
	Internally, this class holds a vector of array_pointer objects.

	@see array_pointer
	@ingroup memory
 */
template <typename Type, bool UseExceptions = false>
class LIMES_EXPORT md_array_pointer final
{
public:
	/** You can access the type of the owned object using this typdef. */
	using OwnedType = Type;

	using Channel = array_pointer<Type, UseExceptions>;

	/** @name Constructors */
	///@{
	/** Creates a pointer that owns no memory. */
	constexpr md_array_pointer() = default;

	/** Allocates the specified number of channels.
		@throws std::bad_alloc An exception will be thrown if the UseExceptions template parameter is true and allocation fails.
	 */
	explicit md_array_pointer (std::size_t numChannels, std::size_t channelSize) noexcept (! UseExceptions)
	{
		if (! reallocate (numChannels, channelSize))
			if constexpr (UseExceptions)
				throw std::bad_alloc();	 // cppcheck-suppress throwInNoexceptFunction
	}

	/** Creates a non-owning array pointer that refers to the passed memory. */
	explicit md_array_pointer (Type** memoryToReference, std::size_t numChannels, std::size_t channelSize) noexcept (! UseExceptions)
	{
		allocate_channel_pointers (numChannels);

		for (auto c = 0UL; c < numChannels; ++c)
			channels[c]->referenceOtherMemory (memoryToReference[c], channelSize);
	}
	///@}

	/** Destructor. */
	~md_array_pointer()
	{
		free();
	}

	/** @name Copying */
	///@{
	/** Creates a non-owning array pointer that references the same memory as the passed pointer. */
	md_array_pointer (const md_array_pointer& other) noexcept
		: channels (other.channels)
	{
	}

	/** Deallocates this pointer's memory, and assigns it to refer to the memory owned by the other pointer.
		This object will not take ownership of the underlying memory.
		@attention The array pointer passed as the right hand side of this operation must outlive the one on the left hand side!
	 */
	md_array_pointer& operator= (const md_array_pointer& other) noexcept (! UseExceptions)	// cppcheck-suppress operatorEqVarError
	{
		free();

		channels = other.channels;
	}
	///@}

	LIMES_DEFAULT_MOVABLE (md_array_pointer)

	/** Deallocates this pointer's memory, and assigns it to refer to the passed pointer.
		This object will not take ownership of the underlying memory.
	 */
	md_array_pointer& referenceOtherMemory (Type** memoryToReference, std::size_t numChannels, std::size_t channelSize) noexcept (! UseExceptions)	// cppcheck-suppress operatorEqRetRefThis
	{
		free();

		allocate_channel_pointers (numChannels);

		for (auto c = 0UL; c < numChannels; ++c)
			channels[c]->referenceOtherMemory (memoryToReference[c], channelSize);

		return *this;
	}

	/** Returns a pointer to one of the channels in the array.
		If the channel index is out of range, a \c nullptr is returned.
	 */
	Channel* getChannel (std::size_t channel) const noexcept
	{
		if (channel >= channels.size())
			return nullptr;

		return &channels[channel];
	}

	/** @name Indexing */
	///@{
	/** Indexes into the array. */
	decltype (auto) operator[] (int i) { return channels[static_cast<std::size_t> (i)]; }
	decltype (auto) operator[] (std::size_t i) { return channels[i]; }
	///@}

	/** Returns the number of channels in the array. */
	std::size_t getNumChannels() const noexcept { return channels.size(); }

	/** Returns the number of elements in each channel of the array. */
	std::size_t getChannelSize() const noexcept
	{
		if (channels.size() == 0)
			return 0UL;

		return channels[0].getSize();
	}

	/** Frees the allocated memory.
		If this is a non-owning array pointer that simply references some other memory, then calling this function does nothing.
	 */
	void free() noexcept (! UseExceptions);

	/** Reallocates the array's memory.
		@throws std::bad_alloc An exception will be thrown if the UseExceptions template parameter is true and allocation fails.
	 */
	bool reallocate (std::size_t numChannels, std::size_t channelSize) noexcept (! UseExceptions);

	/** @name Begin accessors */
	///@{
	/** Returns an iterator to the beginning of the array. */
	Channel*	   begin() noexcept { return &channels[0]; }
	const Channel* begin() const noexcept { return &channels[0]; }
	///@}

	/** @name End accessors */
	///@{
	Channel*	   end() noexcept { return nullptr; }
	const Channel* end() const noexcept { return nullptr; }
	///@}

private:
	void allocate_channel_pointers (std::size_t numChannels) noexcept (! UseExceptions);

	std::vector<Channel> channels;
};

/*------------------------------------------------------------------------------------------------------------------*/

template <typename Type, bool UseExceptions>
void md_array_pointer<Type, UseExceptions>::free() noexcept (! UseExceptions)
{
	channels.clear();
}

template <typename Type, bool UseExceptions>
bool md_array_pointer<Type, UseExceptions>::reallocate (std::size_t numChannels, std::size_t channelSize) noexcept (! UseExceptions)
{
	if (numChannels == channels.size() && channelSize == getChannelSize())
		return true;

	if (numChannels == 0 || channelSize == 0)
	{
		free();
		return true;
	}

	auto allocate = [this, numChannels, channelSize]() -> bool
	{
		allocate_channel_pointers (numChannels);

		for (auto c = 0UL; c < numChannels; ++c)
			if (! channels[c].reallocate (channelSize))
				return false;

		return true;
	};

	if constexpr (UseExceptions)
	{
		if (allocate())
			return true;

		throw std::bad_alloc();	 // cppcheck-suppress throwInNoexceptFunction
	}
	else
	{
		try
		{
			return allocate();
		}
		catch (...)
		{
			return false;
		}
	}
}

template <typename Type, bool UseExceptions>
void md_array_pointer<Type, UseExceptions>::allocate_channel_pointers (std::size_t numChannels) noexcept (! UseExceptions)
{
	if (channels.size() == numChannels)
		return;

	if (numChannels == 0)
	{
		free();
		return;
	}

	for (auto i = channels.size(); i < numChannels; ++i)
		channels.emplace_back();

	for (auto i = channels.size(); i > numChannels; --i)
		channels.pop_back();
}

}  // namespace memory

LIMES_END_NAMESPACE
