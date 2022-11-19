//          Copyright Malte Skarupke 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)

// this is pretty much original code, with prime hash policy removed, and faster log2
// this also relies on das:: to hold all necessary std:: definitions, so its stl agnostic
// and can use EASTL instead. namespace has been renamed to das_ska, to avoild collisions
// with other ska implementations in the projects

#pragma once

#ifdef _MSC_VER
#define SKA_NOINLINE(...) __declspec(noinline) __VA_ARGS__
#else
#define SKA_NOINLINE(...) __VA_ARGS__ __attribute__((noinline))
#endif

namespace das_ska
{
struct power_of_two_hash_policy;
struct fibonacci_hash_policy;

namespace detailv3
{
template<typename Result, typename Functor>
struct functor_storage : Functor
{
    functor_storage() = default;
    functor_storage(const Functor & functor)
        : Functor(functor)
    {
    }
    template<typename... Args>
    Result operator()(Args &&... args)
    {
        return (Result) static_cast<Functor &>(*this)(das::forward<Args>(args)...);
    }
    template<typename... Args>
    Result operator()(Args &&... args) const
    {
        return (Result) static_cast<const Functor &>(*this)(das::forward<Args>(args)...);
    }
};
template<typename Result, typename... Args>
struct functor_storage<Result, Result (*)(Args...)>
{
    typedef Result (*function_ptr)(Args...);
    function_ptr function;
    functor_storage(function_ptr function)
        : function(function)
    {
    }
    Result operator()(Args... args) const
    {
        return function(das::forward<Args>(args)...);
    }
    operator function_ptr &()
    {
        return function;
    }
    operator const function_ptr &()
    {
        return function;
    }
};
template<typename key_type, typename value_type, typename hasher>
struct KeyOrValueHasher : functor_storage<size_t, hasher>
{
    typedef functor_storage<size_t, hasher> hasher_storage;
    KeyOrValueHasher() = default;
    KeyOrValueHasher(const hasher & hash)
        : hasher_storage(hash)
    {
    }
    size_t operator()(const key_type & key)
    {
        return static_cast<hasher_storage &>(*this)(key);
    }
    size_t operator()(const key_type & key) const
    {
        return static_cast<const hasher_storage &>(*this)(key);
    }
    size_t operator()(const value_type & value)
    {
        return static_cast<hasher_storage &>(*this)(value.first);
    }
    size_t operator()(const value_type & value) const
    {
        return static_cast<const hasher_storage &>(*this)(value.first);
    }
    template<typename F, typename S>
    size_t operator()(const das::pair<F, S> & value)
    {
        return static_cast<hasher_storage &>(*this)(value.first);
    }
    template<typename F, typename S>
    size_t operator()(const das::pair<F, S> & value) const
    {
        return static_cast<const hasher_storage &>(*this)(value.first);
    }
};
template<typename key_type, typename value_type, typename key_equal>
struct KeyOrValueEquality : functor_storage<bool, key_equal>
{
    typedef functor_storage<bool, key_equal> equality_storage;
    KeyOrValueEquality() = default;
    KeyOrValueEquality(const key_equal & equality)
        : equality_storage(equality)
    {
    }
    bool operator()(const key_type & lhs, const key_type & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs, rhs);
    }
    bool operator()(const key_type & lhs, const value_type & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs, rhs.first);
    }
    bool operator()(const value_type & lhs, const key_type & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs.first, rhs);
    }
    bool operator()(const value_type & lhs, const value_type & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs.first, rhs.first);
    }
    template<typename F, typename S>
    bool operator()(const key_type & lhs, const das::pair<F, S> & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs, rhs.first);
    }
    template<typename F, typename S>
    bool operator()(const das::pair<F, S> & lhs, const key_type & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs.first, rhs);
    }
    template<typename F, typename S>
    bool operator()(const value_type & lhs, const das::pair<F, S> & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs.first, rhs.first);
    }
    template<typename F, typename S>
    bool operator()(const das::pair<F, S> & lhs, const value_type & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs.first, rhs.first);
    }
    template<typename FL, typename SL, typename FR, typename SR>
    bool operator()(const das::pair<FL, SL> & lhs, const das::pair<FR, SR> & rhs)
    {
        return static_cast<equality_storage &>(*this)(lhs.first, rhs.first);
    }
};
static constexpr int8_t min_lookups = 4;
template<typename T>
struct sherwood_v3_entry
{
    sherwood_v3_entry()
    {
    }
    sherwood_v3_entry(int8_t distance_from_desired)
        : distance_from_desired(distance_from_desired)
    {
    }
    ~sherwood_v3_entry()
    {
    }
    static sherwood_v3_entry * empty_default_table()
    {
        static sherwood_v3_entry result[min_lookups] = { {}, {}, {}, {special_end_value} };
        return result;
    }

    bool has_value() const
    {
        return distance_from_desired >= 0;
    }
    bool is_empty() const
    {
        return distance_from_desired < 0;
    }
    bool is_at_desired_position() const
    {
        return distance_from_desired <= 0;
    }
    template<typename... Args>
    void emplace(int8_t distance, Args &&... args)
    {
        new (das::addressof(value)) T(das::forward<Args>(args)...);
        distance_from_desired = distance;
    }

    void destroy_value()
    {
        value.~T();
        distance_from_desired = -1;
    }

    int8_t distance_from_desired = -1;
    static constexpr int8_t special_end_value = 0;
    union { T value; };
};

inline int8_t log2(size_t value)
{
#ifdef _MSC_VER
    unsigned long res;
    _BitScanReverse(&res, (unsigned long)(value));
    return (int8_t)res;
#elif defined(__GNUC__) || defined(__clang__)
    return 31UL - __builtin_clz(value);
#else
#error "log2(uint32_t) not implemented"
#endif
}
template<typename T, bool>
struct AssignIfTrue
{
    void operator()(T & lhs, const T & rhs)
    {
        lhs = rhs;
    }
    void operator()(T & lhs, T && rhs)
    {
        lhs = das::move(rhs);
    }
};
template<typename T>
struct AssignIfTrue<T, false>
{
    void operator()(T &, const T &)
    {
    }
    void operator()(T &, T &&)
    {
    }
};

inline size_t next_power_of_two(size_t i)
{
    return size_t(1) << (detailv3::log2(i - 1) + 1);
}

template<typename...> using void_t = void;

template<typename T, typename = void>
struct HashPolicySelector
{
    typedef fibonacci_hash_policy type;
};
template<typename T>
struct HashPolicySelector<T, void_t<typename T::hash_policy>>
{
    typedef typename T::hash_policy type;
};

template<typename T, typename FindKey, typename ArgumentHash, typename Hasher, typename ArgumentEqual, typename Equal, typename ArgumentAlloc, typename EntryAlloc>
class sherwood_v3_table : private EntryAlloc, private Hasher, private Equal
{
    using Entry = detailv3::sherwood_v3_entry<T>;
    using AllocatorTraits = das::allocator_traits<EntryAlloc>;
    using EntryPointer = typename AllocatorTraits::pointer;
    struct convertible_to_iterator;

public:

    using value_type = T;
    using size_type = size_t;
    using difference_type = das::ptrdiff_t;
    using hasher = ArgumentHash;
    using key_equal = ArgumentEqual;
    using allocator_type = EntryAlloc;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    sherwood_v3_table()
    {
    }
    explicit sherwood_v3_table(size_type bucket_count, const ArgumentHash & hash = ArgumentHash(), const ArgumentEqual & equal = ArgumentEqual(), const ArgumentAlloc & alloc = ArgumentAlloc())
        : EntryAlloc(alloc), Hasher(hash), Equal(equal)
    {
        rehash(bucket_count);
    }
    sherwood_v3_table(size_type bucket_count, const ArgumentAlloc & alloc)
        : sherwood_v3_table(bucket_count, ArgumentHash(), ArgumentEqual(), alloc)
    {
    }
    sherwood_v3_table(size_type bucket_count, const ArgumentHash & hash, const ArgumentAlloc & alloc)
        : sherwood_v3_table(bucket_count, hash, ArgumentEqual(), alloc)
    {
    }
    explicit sherwood_v3_table(const ArgumentAlloc & alloc)
        : EntryAlloc(alloc)
    {
    }
    template<typename It>
    sherwood_v3_table(It first, It last, size_type bucket_count = 0, const ArgumentHash & hash = ArgumentHash(), const ArgumentEqual & equal = ArgumentEqual(), const ArgumentAlloc & alloc = ArgumentAlloc())
        : sherwood_v3_table(bucket_count, hash, equal, alloc)
    {
        insert(first, last);
    }
    template<typename It>
    sherwood_v3_table(It first, It last, size_type bucket_count, const ArgumentAlloc & alloc)
        : sherwood_v3_table(first, last, bucket_count, ArgumentHash(), ArgumentEqual(), alloc)
    {
    }
    template<typename It>
    sherwood_v3_table(It first, It last, size_type bucket_count, const ArgumentHash & hash, const ArgumentAlloc & alloc)
        : sherwood_v3_table(first, last, bucket_count, hash, ArgumentEqual(), alloc)
    {
    }
    sherwood_v3_table(std::initializer_list<T> il, size_type bucket_count = 0, const ArgumentHash & hash = ArgumentHash(), const ArgumentEqual & equal = ArgumentEqual(), const ArgumentAlloc & alloc = ArgumentAlloc())
        : sherwood_v3_table(bucket_count, hash, equal, alloc)
    {
        if (bucket_count == 0)
            rehash(il.size());
        insert(il.begin(), il.end());
    }
    sherwood_v3_table(std::initializer_list<T> il, size_type bucket_count, const ArgumentAlloc & alloc)
        : sherwood_v3_table(il, bucket_count, ArgumentHash(), ArgumentEqual(), alloc)
    {
    }
    sherwood_v3_table(std::initializer_list<T> il, size_type bucket_count, const ArgumentHash & hash, const ArgumentAlloc & alloc)
        : sherwood_v3_table(il, bucket_count, hash, ArgumentEqual(), alloc)
    {
    }
    sherwood_v3_table(const sherwood_v3_table & other)
        : sherwood_v3_table(other, AllocatorTraits::select_on_container_copy_construction(other.get_allocator()))
    {
    }
    sherwood_v3_table(const sherwood_v3_table & other, const ArgumentAlloc & alloc)
        : EntryAlloc(alloc), Hasher(other), Equal(other), _max_load_factor(other._max_load_factor)
    {
        rehash_for_other_container(other);
        insert(other.begin(), other.end());
    }
    sherwood_v3_table(sherwood_v3_table && other) noexcept
        : EntryAlloc(das::move(other)), Hasher(das::move(other)), Equal(das::move(other))
    {
        swap_pointers(other);
    }
    sherwood_v3_table(sherwood_v3_table && other, const ArgumentAlloc & alloc) noexcept
        : EntryAlloc(alloc), Hasher(das::move(other)), Equal(das::move(other))
    {
        swap_pointers(other);
    }
    sherwood_v3_table & operator=(const sherwood_v3_table & other)
    {
        if (this == das::addressof(other))
            return *this;

        clear();
        if (AllocatorTraits::propagate_on_container_copy_assignment::value)
        {
            if (static_cast<EntryAlloc &>(*this) != static_cast<const EntryAlloc &>(other))
            {
                reset_to_empty_state();
            }
            AssignIfTrue<EntryAlloc, AllocatorTraits::propagate_on_container_copy_assignment::value>()(*this, other);
        }
        _max_load_factor = other._max_load_factor;
        static_cast<Hasher &>(*this) = other;
        static_cast<Equal &>(*this) = other;
        rehash_for_other_container(other);
        insert(other.begin(), other.end());
        return *this;
    }
    sherwood_v3_table & operator=(sherwood_v3_table && other) noexcept
    {
        if (this == das::addressof(other))
            return *this;
        else if (AllocatorTraits::propagate_on_container_move_assignment::value)
        {
            clear();
            reset_to_empty_state();
            AssignIfTrue<EntryAlloc, AllocatorTraits::propagate_on_container_move_assignment::value>()(*this, das::move(other));
            swap_pointers(other);
        }
        else if (static_cast<EntryAlloc &>(*this) == static_cast<EntryAlloc &>(other))
        {
            swap_pointers(other);
        }
        else
        {
            clear();
            _max_load_factor = other._max_load_factor;
            rehash_for_other_container(other);
            for (T & elem : other)
                emplace(das::move(elem));
            other.clear();
        }
        static_cast<Hasher &>(*this) = das::move(other);
        static_cast<Equal &>(*this) = das::move(other);
        return *this;
    }
    ~sherwood_v3_table()
    {
        clear();
        deallocate_data(entries, num_slots_minus_one, max_lookups);
    }

    const allocator_type & get_allocator() const
    {
        return static_cast<const allocator_type &>(*this);
    }
    const ArgumentEqual & key_eq() const
    {
        return static_cast<const ArgumentEqual &>(*this);
    }
    const ArgumentHash & hash_function() const
    {
        return static_cast<const ArgumentHash &>(*this);
    }

    template<typename ValueType>
    struct templated_iterator
    {
        templated_iterator() = default;
        templated_iterator(EntryPointer current)
            : current(current)
        {
        }
        EntryPointer current = EntryPointer();

        using iterator_category = das::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = ptrdiff_t;
        using pointer = ValueType *;
        using reference = ValueType &;

        friend bool operator==(const templated_iterator & lhs, const templated_iterator & rhs)
        {
            return lhs.current == rhs.current;
        }
        friend bool operator!=(const templated_iterator & lhs, const templated_iterator & rhs)
        {
            return !(lhs == rhs);
        }

        templated_iterator & operator++()
        {
            do
            {
                ++current;
            }
            while(current->is_empty());
            return *this;
        }
        templated_iterator operator++(int)
        {
            templated_iterator copy(*this);
            ++*this;
            return copy;
        }

        ValueType & operator*() const
        {
            return current->value;
        }
        ValueType * operator->() const
        {
            return das::addressof(current->value);
        }

        operator templated_iterator<const value_type>() const
        {
            return { current };
        }
    };
    using iterator = templated_iterator<value_type>;
    using const_iterator = templated_iterator<const value_type>;

    iterator begin()
    {
        for (EntryPointer it = entries;; ++it)
        {
            if (it->has_value())
                return { it };
        }
    }
    const_iterator begin() const
    {
        for (EntryPointer it = entries;; ++it)
        {
            if (it->has_value())
                return { it };
        }
    }
    const_iterator cbegin() const
    {
        return begin();
    }
    iterator end()
    {
        return { entries + static_cast<ptrdiff_t>(num_slots_minus_one + max_lookups) };
    }
    const_iterator end() const
    {
        return { entries + static_cast<ptrdiff_t>(num_slots_minus_one + max_lookups) };
    }
    const_iterator cend() const
    {
        return end();
    }

    iterator find(const FindKey & key)
    {
        size_t index = hash_policy.index_for_hash(hash_object(key), num_slots_minus_one);
        EntryPointer it = entries + ptrdiff_t(index);
        for (int8_t distance = 0; it->distance_from_desired >= distance; ++distance, ++it)
        {
            if (compares_equal(key, it->value))
                return { it };
        }
        return end();
    }
    const_iterator find(const FindKey & key) const
    {
        return const_cast<sherwood_v3_table *>(this)->find(key);
    }
    size_t count(const FindKey & key) const
    {
        return find(key) == end() ? 0 : 1;
    }
    das::pair<iterator, iterator> equal_range(const FindKey & key)
    {
        iterator found = find(key);
        if (found == end())
            return { found, found };
        else
            return { found, das::next(found) };
    }
    das::pair<const_iterator, const_iterator> equal_range(const FindKey & key) const
    {
        const_iterator found = find(key);
        if (found == end())
            return { found, found };
        else
            return { found, das::next(found) };
    }

    template<typename Key, typename... Args>
    das::pair<iterator, bool> emplace(Key && key, Args &&... args)
    {
        size_t index = hash_policy.index_for_hash(hash_object(key), num_slots_minus_one);
        EntryPointer current_entry = entries + ptrdiff_t(index);
        int8_t distance_from_desired = 0;
        for (; current_entry->distance_from_desired >= distance_from_desired; ++current_entry, ++distance_from_desired)
        {
            if (compares_equal(key, current_entry->value))
                return { { current_entry }, false };
        }
        return emplace_new_key(distance_from_desired, current_entry, das::forward<Key>(key), das::forward<Args>(args)...);
    }

    das::pair<iterator, bool> insert(const value_type & value)
    {
        return emplace(value);
    }
    das::pair<iterator, bool> insert(value_type && value)
    {
        return emplace(das::move(value));
    }
    template<typename... Args>
    iterator emplace_hint(const_iterator, Args &&... args)
    {
        return emplace(das::forward<Args>(args)...).first;
    }
    iterator insert(const_iterator, const value_type & value)
    {
        return emplace(value).first;
    }
    iterator insert(const_iterator, value_type && value)
    {
        return emplace(das::move(value)).first;
    }

    template<typename It>
    void insert(It begin, It end)
    {
        for (; begin != end; ++begin)
        {
            emplace(*begin);
        }
    }
    void insert(std::initializer_list<value_type> il)
    {
        insert(il.begin(), il.end());
    }

    void rehash(size_t num_buckets)
    {
        num_buckets = das::max(num_buckets, static_cast<size_t>(das::ceil(num_elements / static_cast<double>(_max_load_factor))));
        if (num_buckets == 0)
        {
            reset_to_empty_state();
            return;
        }
        auto new_prime_index = hash_policy.next_size_over(num_buckets);
        if (num_buckets == bucket_count())
            return;
        int8_t new_max_lookups = compute_max_lookups(num_buckets);
        EntryPointer new_buckets(AllocatorTraits::allocate(*this, num_buckets + new_max_lookups));
        EntryPointer special_end_item = new_buckets + static_cast<ptrdiff_t>(num_buckets + new_max_lookups - 1);
        for (EntryPointer it = new_buckets; it != special_end_item; ++it)
            it->distance_from_desired = -1;
        special_end_item->distance_from_desired = Entry::special_end_value;
        das::swap(entries, new_buckets);
        das::swap(num_slots_minus_one, num_buckets);
        --num_slots_minus_one;
        hash_policy.commit(new_prime_index);
        int8_t old_max_lookups = max_lookups;
        max_lookups = new_max_lookups;
        num_elements = 0;
        for (EntryPointer it = new_buckets, end = it + static_cast<ptrdiff_t>(num_buckets + old_max_lookups); it != end; ++it)
        {
            if (it->has_value())
            {
                emplace(das::move(it->value));
                it->destroy_value();
            }
        }
        deallocate_data(new_buckets, num_buckets, old_max_lookups);
    }

    void reserve(size_t num_elements)
    {
        size_t required_buckets = num_buckets_for_reserve(num_elements);
        if (required_buckets > bucket_count())
            rehash(required_buckets);
    }

    // the return value is a type that can be converted to an iterator
    // the reason for doing this is that it's not free to find the
    // iterator pointing at the next element. if you care about the
    // next iterator, turn the return value into an iterator
    convertible_to_iterator erase(const_iterator to_erase)
    {
        EntryPointer current = to_erase.current;
        current->destroy_value();
        --num_elements;
        for (EntryPointer next = current + ptrdiff_t(1); !next->is_at_desired_position(); ++current, ++next)
        {
            current->emplace(next->distance_from_desired - 1, das::move(next->value));
            next->destroy_value();
        }
        return { to_erase.current };
    }

    iterator erase(const_iterator begin_it, const_iterator end_it)
    {
        if (begin_it == end_it)
            return { begin_it.current };
        for (EntryPointer it = begin_it.current, end = end_it.current; it != end; ++it)
        {
            if (it->has_value())
            {
                it->destroy_value();
                --num_elements;
            }
        }
        if (end_it == this->end())
            return this->end();
        ptrdiff_t num_to_move = das::min(static_cast<ptrdiff_t>(end_it.current->distance_from_desired), end_it.current - begin_it.current);
        EntryPointer to_return = end_it.current - num_to_move;
        for (EntryPointer it = end_it.current; !it->is_at_desired_position();)
        {
            EntryPointer target = it - num_to_move;
            target->emplace(it->distance_from_desired - num_to_move, das::move(it->value));
            it->destroy_value();
            ++it;
            num_to_move = das::min(static_cast<ptrdiff_t>(it->distance_from_desired), num_to_move);
        }
        return { to_return };
    }

    size_t erase(const FindKey & key)
    {
        auto found = find(key);
        if (found == end())
            return 0;
        else
        {
            erase(found);
            return 1;
        }
    }

    void clear()
    {
        for (EntryPointer it = entries, end = it + static_cast<ptrdiff_t>(num_slots_minus_one + max_lookups); it != end; ++it)
        {
            if (it->has_value())
                it->destroy_value();
        }
        num_elements = 0;
    }

    void shrink_to_fit()
    {
        rehash_for_other_container(*this);
    }

    void swap(sherwood_v3_table & other)
    {
        using das::swap;
        swap_pointers(other);
        swap(static_cast<ArgumentHash &>(*this), static_cast<ArgumentHash &>(other));
        swap(static_cast<ArgumentEqual &>(*this), static_cast<ArgumentEqual &>(other));
        if (AllocatorTraits::propagate_on_container_swap::value)
            swap(static_cast<EntryAlloc &>(*this), static_cast<EntryAlloc &>(other));
    }

    size_t size() const
    {
        return num_elements;
    }
    size_t max_size() const
    {
        return (AllocatorTraits::max_size(*this)) / sizeof(Entry);
    }
    size_t bucket_count() const
    {
        return num_slots_minus_one ? num_slots_minus_one + 1 : 0;
    }
    size_type max_bucket_count() const
    {
        return (AllocatorTraits::max_size(*this) - min_lookups) / sizeof(Entry);
    }
    size_t bucket(const FindKey & key) const
    {
        return hash_policy.index_for_hash(hash_object(key), num_slots_minus_one);
    }
    float load_factor() const
    {
        size_t buckets = bucket_count();
        if (buckets)
            return static_cast<float>(num_elements) / bucket_count();
        else
            return 0;
    }
    void max_load_factor(float value)
    {
        _max_load_factor = value;
    }
    float max_load_factor() const
    {
        return _max_load_factor;
    }

    bool empty() const
    {
        return num_elements == 0;
    }

private:
    EntryPointer entries = Entry::empty_default_table();
    size_t num_slots_minus_one = 0;
    typename HashPolicySelector<ArgumentHash>::type hash_policy;
    int8_t max_lookups = detailv3::min_lookups - 1;
    float _max_load_factor = 0.5f;
    size_t num_elements = 0;

    static int8_t compute_max_lookups(size_t num_buckets)
    {
        int8_t desired = detailv3::log2(num_buckets);
        return das::max(detailv3::min_lookups, desired);
    }

    size_t num_buckets_for_reserve(size_t num_elements_) const
    {
        return static_cast<size_t>(das::ceil(num_elements_ / das::min(0.5, static_cast<double>(_max_load_factor))));
    }
    void rehash_for_other_container(const sherwood_v3_table & other)
    {
        rehash(das::min(num_buckets_for_reserve(other.size()), other.bucket_count()));
    }

    void swap_pointers(sherwood_v3_table & other)
    {
        using das::swap;
        swap(hash_policy, other.hash_policy);
        swap(entries, other.entries);
        swap(num_slots_minus_one, other.num_slots_minus_one);
        swap(num_elements, other.num_elements);
        swap(max_lookups, other.max_lookups);
        swap(_max_load_factor, other._max_load_factor);
    }

    template<typename Key, typename... Args>
    SKA_NOINLINE(das::pair<iterator, bool>) emplace_new_key(int8_t distance_from_desired, EntryPointer current_entry, Key && key, Args &&... args)
    {
        using das::swap;
        if (num_slots_minus_one == 0 || distance_from_desired == max_lookups || num_elements + 1 > (num_slots_minus_one + 1) * static_cast<double>(_max_load_factor))
        {
            grow();
            return emplace(das::forward<Key>(key), das::forward<Args>(args)...);
        }
        else if (current_entry->is_empty())
        {
            current_entry->emplace(distance_from_desired, das::forward<Key>(key), das::forward<Args>(args)...);
            ++num_elements;
            return { { current_entry }, true };
        }
        value_type to_insert(das::forward<Key>(key), das::forward<Args>(args)...);
        swap(distance_from_desired, current_entry->distance_from_desired);
        swap(to_insert, current_entry->value);
        iterator result = { current_entry };
        for (++distance_from_desired, ++current_entry;; ++current_entry)
        {
            if (current_entry->is_empty())
            {
                current_entry->emplace(distance_from_desired, das::move(to_insert));
                ++num_elements;
                return { result, true };
            }
            else if (current_entry->distance_from_desired < distance_from_desired)
            {
                swap(distance_from_desired, current_entry->distance_from_desired);
                swap(to_insert, current_entry->value);
                ++distance_from_desired;
            }
            else
            {
                ++distance_from_desired;
                if (distance_from_desired == max_lookups)
                {
                    swap(to_insert, result.current->value);
                    grow();
                    return emplace(das::move(to_insert));
                }
            }
        }
    }

    void grow()
    {
        rehash(das::max(size_t(4), 2 * bucket_count()));
    }

    void deallocate_data(EntryPointer begin, size_t num_slots_minus_one_, int8_t max_lookups_)
    {
        if (begin != Entry::empty_default_table())
        {
            AllocatorTraits::deallocate(*this, begin, num_slots_minus_one_ + max_lookups_ + 1);
        }
    }

    void reset_to_empty_state()
    {
        deallocate_data(entries, num_slots_minus_one, max_lookups);
        entries = Entry::empty_default_table();
        num_slots_minus_one = 0;
        hash_policy.reset();
        max_lookups = detailv3::min_lookups - 1;
    }

    template<typename U>
    size_t hash_object(const U & key)
    {
        return static_cast<Hasher &>(*this)(key);
    }
    template<typename U>
    size_t hash_object(const U & key) const
    {
        return static_cast<const Hasher &>(*this)(key);
    }
    template<typename L, typename R>
    bool compares_equal(const L & lhs, const R & rhs)
    {
        return static_cast<Equal &>(*this)(lhs, rhs);
    }

    struct convertible_to_iterator
    {
        EntryPointer it;

        operator iterator()
        {
            if (it->has_value())
                return { it };
            else
                return ++iterator{it};
        }
        operator const_iterator()
        {
            if (it->has_value())
                return { it };
            else
                return ++const_iterator{it};
        }
    };

};
}

struct power_of_two_hash_policy
{
    size_t index_for_hash(size_t hash, size_t num_slots_minus_one) const
    {
        return hash & num_slots_minus_one;
    }
    size_t keep_in_range(size_t index, size_t num_slots_minus_one) const
    {
        return index_for_hash(index, num_slots_minus_one);
    }
    int8_t next_size_over(size_t & size) const
    {
        size = detailv3::next_power_of_two(size);
        return 0;
    }
    void commit(int8_t)
    {
    }
    void reset()
    {
    }

};

struct fibonacci_hash_policy
{
    size_t index_for_hash(size_t hash, size_t /*num_slots_minus_one*/) const
    {
        return size_t((11400714819323198485ull * hash) >> shift);
    }
    size_t keep_in_range(size_t index, size_t num_slots_minus_one) const
    {
        return index & num_slots_minus_one;
    }

    int8_t next_size_over(size_t & size) const
    {
        size = das::max(size_t(2), detailv3::next_power_of_two(size));
        return 64 - detailv3::log2(size);
    }
    void commit(int8_t shift8)
    {
        this->shift = shift8;
    }
    void reset()
    {
        shift = 63;
    }

private:
    int8_t shift = 63;
};

template<typename K, typename V, typename H = das::hash<K>, typename E = das::equal_to<K>, typename A = das::allocator<das::pair<K, V> > >
class flat_hash_map
        : public detailv3::sherwood_v3_table
        <
    das::pair<K, V>,
            K,
            H,
            detailv3::KeyOrValueHasher<K, das::pair<K, V>, H>,
            E,
            detailv3::KeyOrValueEquality<K, das::pair<K, V>, E>,
            A,
            typename das::allocator_traits<A>::template rebind_alloc<detailv3::sherwood_v3_entry<das::pair<K, V>>>
        >
{
    using Table = detailv3::sherwood_v3_table
    <
        das::pair<K, V>,
        K,
        H,
        detailv3::KeyOrValueHasher<K, das::pair<K, V>, H>,
        E,
        detailv3::KeyOrValueEquality<K, das::pair<K, V>, E>,
        A,
        typename das::allocator_traits<A>::template rebind_alloc<detailv3::sherwood_v3_entry<das::pair<K, V>>>
    >;
public:

    using key_type = K;
    using mapped_type = V;

    using Table::Table;
    flat_hash_map()
    {
    }

    inline V & operator[](const K & key)
    {
        return emplace(key, convertible_to_value()).first->second;
    }
    inline V & operator[](K && key)
    {
        return emplace(das::move(key), convertible_to_value()).first->second;
    }
    V & at(const K & key)
    {
        auto found = this->find(key);
        assert( found != this->end() && "Argument passed to at() was not in the map.");
        return found->second;
    }
    const V & at(const K & key) const
    {
        auto found = this->find(key);
        assert (found != this->end() && "Argument passed to at() was not in the map.");
        return found->second;
    }

    using Table::emplace;
    das::pair<typename Table::iterator, bool> emplace()
    {
        return emplace(key_type(), convertible_to_value());
    }
    template<typename M>
    das::pair<typename Table::iterator, bool> insert_or_assign(const key_type & key, M && m)
    {
        auto emplace_result = emplace(key, das::forward<M>(m));
        if (!emplace_result.second)
            emplace_result.first->second = das::forward<M>(m);
        return emplace_result;
    }
    template<typename M>
    das::pair<typename Table::iterator, bool> insert_or_assign(key_type && key, M && m)
    {
        auto emplace_result = emplace(das::move(key), das::forward<M>(m));
        if (!emplace_result.second)
            emplace_result.first->second = das::forward<M>(m);
        return emplace_result;
    }
    template<typename M>
    typename Table::iterator insert_or_assign(typename Table::const_iterator, const key_type & key, M && m)
    {
        return insert_or_assign(key, das::forward<M>(m)).first;
    }
    template<typename M>
    typename Table::iterator insert_or_assign(typename Table::const_iterator, key_type && key, M && m)
    {
        return insert_or_assign(das::move(key), das::forward<M>(m)).first;
    }

    friend bool operator==(const flat_hash_map & lhs, const flat_hash_map & rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        for (const typename Table::value_type & value : lhs)
        {
            auto found = rhs.find(value.first);
            if (found == rhs.end())
                return false;
            else if (value.second != found->second)
                return false;
        }
        return true;
    }
    friend bool operator!=(const flat_hash_map & lhs, const flat_hash_map & rhs)
    {
        return !(lhs == rhs);
    }

private:
    struct convertible_to_value
    {
        operator V() const
        {
            return V();
        }
    };
};

template<typename T, typename H = das::hash<T>, typename E = das::equal_to<T>, typename A = das::allocator<T> >
class flat_hash_set
        : public detailv3::sherwood_v3_table
        <
            T,
            T,
            H,
            detailv3::functor_storage<size_t, H>,
            E,
            detailv3::functor_storage<bool, E>,
            A,
            typename das::allocator_traits<A>::template rebind_alloc<detailv3::sherwood_v3_entry<T>>
        >
{
    using Table = detailv3::sherwood_v3_table
    <
        T,
        T,
        H,
        detailv3::functor_storage<size_t, H>,
        E,
        detailv3::functor_storage<bool, E>,
        A,
        typename das::allocator_traits<A>::template rebind_alloc<detailv3::sherwood_v3_entry<T>>
    >;
public:

    using key_type = T;

    using Table::Table;
    flat_hash_set()
    {
    }

    template<typename... Args>
    das::pair<typename Table::iterator, bool> emplace(Args &&... args)
    {
        return Table::emplace(T(das::forward<Args>(args)...));
    }
    das::pair<typename Table::iterator, bool> emplace(const key_type & arg)
    {
        return Table::emplace(arg);
    }
    das::pair<typename Table::iterator, bool> emplace(key_type & arg)
    {
        return Table::emplace(arg);
    }
    das::pair<typename Table::iterator, bool> emplace(const key_type && arg)
    {
        return Table::emplace(das::move(arg));
    }
    das::pair<typename Table::iterator, bool> emplace(key_type && arg)
    {
        return Table::emplace(das::move(arg));
    }

    friend bool operator==(const flat_hash_set & lhs, const flat_hash_set & rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        for (const T & value : lhs)
        {
            if (rhs.find(value) == rhs.end())
                return false;
        }
        return true;
    }
    friend bool operator!=(const flat_hash_set & lhs, const flat_hash_set & rhs)
    {
        return !(lhs == rhs);
    }
};


template<typename T>
struct power_of_two_std_hash : das::hash<T>
{
    typedef das_ska::power_of_two_hash_policy hash_policy;
};

} // end namespace ska
