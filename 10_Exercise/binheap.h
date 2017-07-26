/*

 *  binheap.h

 *

 *  Created on: 24.06.2017

 *

 *  Author: Daniel Rehfeldt

 *
 
 */

template <typename Key, typename Value>
struct HeapEntry
{
   Key key;
   Value val;
};


template <typename Key, typename Value>
class BinHeap
{
   using Entry = HeapEntry<Key, Value>;

   Entry* entries = nullptr;
   const Key max = std::numeric_limits<Key>::max(); // sentinel value
   const Key min = std::numeric_limits<Key>::min(); // sentinel value
   int size = 0;
   int cap = -1;
   
   public:
      BinHeap(int capacity)
      {
         assert(capacity >= 1);
         cap = capacity;
         entries = new Entry[cap + 2];
         entries[0].key = min;
         entries[cap + 1].key = max;
      }
      
      void deleteMin(Key& key, Value& value);
      void insert(Key key, Value value);
   
      int getSize() const
      {
         return size;
      }


      ~BinHeap()
      {
         delete[] entries; 
      }
};

template <typename Key, typename Value>
inline void BinHeap<Key, Value>::deleteMin(
    Key& key,
    Value& value
)
{
   assert(size > 0);
   assert(entries);

   key = entries[1].key;
   value = entries[1].val;
   
   int hole = 1; 
   int child = 2;
   const int lastentry = size--;

   // move down along min-path
   while( child < lastentry ) 
   {
      const Key key1 = entries[child].key;
      const Key key2 = entries[child + 1].key;
     
      // second child with smaller key?
      if( key1 > key2 ) 
      {
         entries[hole].key = key2;
         entries[hole].val = entries[++child].val;
      } 
      else 
      {
         entries[hole].key = key1;
         entries[hole].val = entries[child].val;
      }
      hole = child;
      child *= 2;
   }

   const Key fill = entries[lastentry].key;
   int parent = hole / 2;
   
   while( entries[parent].key > fill )
   {
      entries[hole] = entries[parent];
      hole = parent;
      parent /= 2;
   }

   // finally, fill the hole
   entries[hole].key = fill;
   entries[hole].val = entries[lastentry].val;

   // set sentinel
   entries[lastentry].key = max;
}


template <typename Key, typename Value>
inline void BinHeap<Key, Value>::insert(
    Key key,
    Value value
)
{
   assert(size < cap);

   int hole = ++size; 
   int parent = hole / 2;
   
   Key parentKey = entries[parent].key;
   
   // move hole up
   while( parentKey > key ) 
   {
      entries[hole].key = parentKey;
      entries[hole].val = entries[parent].val;
      hole = parent;
      parent /= 2;
      parentKey = entries[parent].key;
   }

   // fill the hole
   entries[hole].key = key;
   entries[hole].val = value;
}
