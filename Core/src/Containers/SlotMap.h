#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <utility>

namespace NGN
{
	template<class T>
	class SlotMap
	{
	public:
		using value_type = T;

		SlotMap() = default;
		explicit SlotMap(size_t capacity) { reserve(capacity); }

		void reserve(size_t capacity) {
			m_values.reserve(capacity);
			m_denseToSlot.reserve(capacity);
			m_denseToId.reserve(capacity);
			m_idToSlot.reserve(capacity);
		}

		size_t size() const { return m_values.size(); }
		bool empty() const { return m_values.empty(); }

		void clear() {
			m_values.clear();
			m_denseToSlot.clear();
			m_denseToId.clear();
			m_idToSlot.clear();
			m_slotToDense.assign(m_slotToDense.size(), kInvalid);
		}

		template<class... Args>
		bool emplace_with_id(uint64_t id, Args&&... args) {
			if (m_idToSlot.find(id) != m_idToSlot.end()) {
				return false; // ID already exists
			}
			const uint32_t slot = alloc_slot();
			const uint32_t denseIndex = (uint32_t)m_values.size();

			m_values.emplace_back(std::forward<Args>(args)...);
			m_denseToSlot.push_back(slot);
			m_slotToDense[slot] = denseIndex;

			m_denseToId.push_back(id);
			m_idToSlot.emplace(id, slot);
			return true;
		}

		T* get(uint64_t id) {
			auto it = m_idToSlot.find(id);
			if (it == m_idToSlot.end()) return nullptr;
			const uint32_t slot = it->second;
			if (slot >= m_slotToDense.size()) return nullptr;
			const uint32_t denseIndex = m_slotToDense[slot];
			if (denseIndex == kInvalid || denseIndex >= m_values.size()) return nullptr;
			return &m_values[denseIndex];
		}

		const T* get(uint64_t id) const { return const_cast<SlotMap*>(this)->get(id); }

		bool contains(uint64_t id) const { return get(id) != nullptr; }

		bool erase(uint64_t id) {
			auto it = m_idToSlot.find(id);
			if (it == m_idToSlot.end()) return false;

			const uint32_t slot = it->second;
			if (slot >= m_slotToDense.size()) { 
				m_idToSlot.erase(it);
				return false;
			}

			const uint32_t denseIndex = m_slotToDense[slot];
			if (denseIndex == kInvalid || denseIndex >= m_values.size()) {
				m_idToSlot.erase(it);
				return false;
			}

			const uint32_t back = (uint32_t)m_values.size() - 1;

			if (denseIndex != back) {
				std::swap(m_values[denseIndex], m_values[back]);

				const uint32_t movedSlot = m_denseToSlot[back];
				m_denseToSlot[denseIndex] = movedSlot;
				m_slotToDense[movedSlot] = denseIndex;

				std::swap(m_denseToId[denseIndex], m_denseToId[back]);
				const uint64_t movedId = m_denseToId[denseIndex];
				m_idToSlot[movedId] = movedSlot;
			}

			m_values.pop_back();
			m_denseToSlot.pop_back();

			const uint64_t backId = m_denseToId.back();
			m_denseToId.pop_back();
			m_idToSlot.erase(backId);

			m_slotToDense[slot] = kInvalid;
			m_freeSlots.push_back(slot);
			return true;
		}

		auto begin() { return m_values.begin(); }
		auto end() { return m_values.end(); }
		auto begin() const { return m_values.begin(); }
		auto end() const { return m_values.end(); }

		T& operator[](size_t denseIndex) { return m_values[denseIndex]; }
		const T& operator[](size_t denseIndex) const { return m_values[denseIndex]; }

		T* data() { return m_values.data(); }
		const T* data() const { return m_values.data(); }

		uint64_t id_at(size_t denseIndex) const { return m_denseToId[denseIndex]; }

		uint32_t dense_index_of(uint64_t id) const {
			auto it = m_idToSlot.find(id);
			if (it == m_idToSlot.end()) return kInvalid;
			const uint32_t slot = it->second;
			if (slot >= m_slotToDense.size()) return kInvalid;
			return m_slotToDense[slot];
		}

		const std::vector<uint64_t>& ids() const { return m_denseToId; }

	private:
		static constexpr uint32_t kInvalid = 0xFFFFFFFFu;

		std::vector<T> m_values;
		std::vector<uint32_t> m_denseToSlot;
		std::vector<uint32_t> m_slotToDense;
		std::vector<uint32_t> m_freeSlots;
		std::vector<uint64_t> m_denseToId;
		std::unordered_map<uint64_t, uint32_t> m_idToSlot;

		uint32_t alloc_slot() {
			if (!m_freeSlots.empty()) {
				const uint32_t slot = m_freeSlots.back();
				m_freeSlots.pop_back();
				return slot;
			}
			const uint32_t slot = (uint32_t)m_slotToDense.size();
			m_slotToDense.push_back(kInvalid);
			return slot;
		}
	};
}