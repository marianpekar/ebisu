#pragma once

#include <vector>

template <typename T>
class Heap
{
private:
    std::vector<T> items;
    int current_item_count;

public:
    Heap(int maxHeapSize)
    {
        items.resize(maxHeapSize);
        current_item_count = 0;
    }

    void Add(T item)
    {
        item->heap_index = current_item_count;
        items[current_item_count] = item;
        SortUp(item);
        current_item_count++;
    }

    T RemoveFirst()
    {
        T first_item = items[0];
        current_item_count--;
        items[0] = items[current_item_count];
        items[0]->heap_index = 0;
        SortDown(items[0]);
        return first_item;
    }

    void UpdateItem(T item)
    {
        SortUp(item);
    }

    int Count() const
    {
        return current_item_count;
    }

    bool Contains(T item)
    {
        return items[item->heap_index] == item;
    }

private:
    void SortDown(T item)
    {
        while (true)
        {
            int child_index_left = item->heap_index * 2 + 1;
            int child_index_right = item->heap_index * 2 + 2;
            int swap_index = 0;

            if (child_index_left < current_item_count)
            {
                swap_index = child_index_left;

                if (child_index_right < current_item_count)
                {
                    if (items[child_index_left]->CompareTo(*items[child_index_right]) < 0)
                    {
                        swap_index = child_index_right;
                    }
                }

                if (item->CompareTo(*items[swap_index]) < 0)
                {
                    Swap(item, items[swap_index]);
                }
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }

    void SortUp(T item)
    {
        int parent_index = (item->heap_index - 1) / 2;

        while (true)
        {
            T parent_item = items[parent_index];
            if (item->CompareTo(*parent_item) > 0)
            {
                Swap(item, parent_item);
            }
            else
            {
                break;
            }

            parent_index = (item->heap_index - 1) / 2;
        }
    }

    void Swap(T item_a, T item_b)
    {
        items[item_a->heap_index] = item_b;
        items[item_b->heap_index] = item_a;

        int temp = item_a->heap_index;
        item_a->heap_index = item_b->heap_index;
        item_b->heap_index = temp;
    }
};
