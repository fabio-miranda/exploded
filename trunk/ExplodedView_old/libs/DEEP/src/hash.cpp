/*************************************************************************\

  Copyright 2002 The University of North Carolina at Chapel Hill.
  All Rights Reserved.

  Permission to use, copy, modify OR distribute the MODIFIED portion of this
  software and its documentation for educational, research and non-profit 
  purposes, without fee, and without a written agreement is hereby granted, 
  provided that the above copyright notice and the following three paragraphs
  appear in all copies.
	
  IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL BE
  LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
  CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
  USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY
  OF NORTH CAROLINA HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGES.

  THE UNIVERSITY OF NORTH CAROLINA SPECIFICALLY DISCLAIM ANY
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  NORTH CAROLINA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

  The authors may be contacted via:

  US Mail:             Y. Kim, M. Lin
                       Department of Computer Science
                       Sitterson Hall, CB #3175
                       University of N. Carolina
                       Chapel Hill, NC 27599-3175

  Phone:               (919) 962-1749

  EMail:               geom@cs.unc.edu
                       youngkim@cs.unc.edu
					   lin@cs.unc.edu

\**************************************************************************/

// hash.cpp was originally written by Thomans Wang (wang@cup.hp.com)

/* 
 * Description:
 *   hash table implementation file
 *
 * Author: Thomas Wang
 *
 * History:
 * (ID)   (name)   (date)    (bug#)      (message)
 * 000TW  Thomas   07/01/97  0000-00000  Sample msg
 */




#include <SWIFT_hash.h>








/*
 * Description:
 *   single linked list template implementation file
 *
 * Author: Thomas Wang
 *
 * History:
 * (ID)   (name)   (date)    (bug#)      (message)
 * 000TW  Thomas   07/01/97  0000-00000  Sample msg
 */







/* purecov: begin inspected */


/* explain function
 * Description:
 *   Assignment operator
 *
 * Parameters:
 *   cpy - node to be copied from
 *
 * Return Value: reference to the copied object
 *
 * Assumes: none
 */
kvlinkSWIFT_hash& kvlinkSWIFT_hash::operator = (const kvlinkSWIFT_hash& cpy)
{
  this->data_item = cpy.data_item;
  this->the_next = cpy.the_next;
  return *this;
}


/* explain function
 * Description:
 *   Reverse() will update the single linked list to reverse
 *   its ordering.  The reversed single linked list will be returned.
 *
 * Parameters: none
 *
 * Return Value: Reversed uuSlink
 *
 * Assumes: none
 */
kvlinkSWIFT_hash* kvlinkSWIFT_hash::Reverse()
{
  kvlinkSWIFT_hash* result = NULL;
  kvlinkSWIFT_hash* cur_ptr = this;
  kvlinkSWIFT_hash* next_ptr;
  do
  {
    /* reverse one link */
    next_ptr = cur_ptr->the_next;
    cur_ptr->the_next = result;
    result = cur_ptr;
    cur_ptr = next_ptr;
  } while (cur_ptr != NULL);

  return result;
}

/* purecov: end inspected */





/* file kvarraySWIFT_hash.cc
 *
 * Description: expansion array template implementation file
 *
 * Author: Thomas Wang
 *
 */







/* purecov: begin inspected */


/* explain function
 * Description:
 *   Operator [] will obtain an object inside the array, given an index.
 *   The index starts from 0.
 *
 *   Warning: if retrieval is out of bound, program will
 *            segmentation fault!
 *
 * Parameters:
 *   the_index - position number of array
 *
 * Return Value: object at the specified index position
 *
 * Assumes:
 *   Variable 'the_index' is within the array bound.
 */
kvlinkSWIFT_hash*& kvarraySWIFT_hash::operator [] (size_t the_index) const
{
  register size_t buf_index = 0;
  register size_t cur_buflen = UUARRAY_INITIAL_SIZE;

  /* determine which slice buffer to use */

  /* while the index is not in the current buffer */
  while (the_index >= cur_buflen)
  {
    the_index -= cur_buflen;
    ++buf_index; // double buffer index
    cur_buflen += cur_buflen; // double buffer size
  }
  return *((this->the_desc[buf_index]) + the_index);
}


/* explain function
 * Description:
 *   GrowBuffer() will extend the buffers to the length
 *   indicated by 'the_length'.  If out of memory happens, the
 *   grown buffers will be deleted.
 *
 *   Returns TRUE if out of memory.
 *
 * Parameters:
 *   the_length - length to grow to
 *
 * Return Value: TRUE if out of memory, FALSE if OK
 *
 * Assumes: none
 */
char kvarraySWIFT_hash::GrowBuffer(size_t the_length)
{
  register size_t buf_index = 0;
  register size_t cur_buflen = UUARRAY_INITIAL_SIZE;
  size_t old_num_buffer = this->num_buffer;

  while (the_length > 0)
  {
    if (buf_index >= this->num_buffer)
    {
      // beyond the current buffer limit
      value_type *new_buffer = new value_type[cur_buflen];
      if (new_buffer == NULL)
      {
        // out of memory, delete any newly allocated buffer
        while (this->num_buffer > old_num_buffer)
        {
          delete [] this->the_desc[--(this->num_buffer)];
        }
        return 1; // out of memory status
      }
      this->the_desc[(this->num_buffer)++] = new_buffer; // assign new buffer
    }
      
    if (the_length <= cur_buflen)
      break; // end of the loop
    else
    {
      the_length -= cur_buflen; // decrease length by current buffer length
      ++buf_index; // increase buffer index
      cur_buflen += cur_buflen; // double current buffer length
    }
  }
  return 0; // ok
}


/* explain function
 * Description:
 *   Append() will append an object to the end of an array.
 *   The resulting length of the array will be returned.
 * 
 *   If out of memory, the object will not be appended, and
 *   the length of the array will not be modified.
 *
 * Parameters:
 *   the_obj - object to be appended
 *
 * Return Value: resulting length in unsigned 32 bit integer
 *
 * Assumes: none
 */
size_t kvarraySWIFT_hash::Append(const value_type the_obj)
{
  size_t buf_index = 0;
  register size_t cur_buflen = UUARRAY_INITIAL_SIZE;
  register size_t the_index = this->total_elem;

  if (the_index + 1 != 0)
  {
    /* number did not wrap around past 2^32 - 1 back to 0 */

    /* determine which slice buffer to use */

    /* while the index is not covered by current buffer */
    while (the_index >= cur_buflen)
    {
      the_index -= cur_buflen; // decrease array index by current buffer len
      ++buf_index; // increase buffer index
      cur_buflen += cur_buflen; // double buffer size
    }

    if (buf_index < this->num_buffer)
    {
      // no expansion required

      *((this->the_desc[buf_index])+the_index) = the_obj; // assign the object
      ++(this->total_elem); // increase length by 1
    }
    else
    {
      // expansion required

      value_type* new_buffer = new value_type[cur_buflen];
      // allocate a new buffer

      if (new_buffer != NULL)
      {
        ++(this->num_buffer); // increase number of buffer by 1
        this->the_desc[buf_index] = new_buffer; // assign the new buffer
        *(this->the_desc[buf_index]) = the_obj; // assign the object
        ++(this->total_elem); // increase length by 1
      }
    }
  }
  return this->total_elem; // return resulting array length
}


/* explain function
 * Description:
 *   IncreaseLength() increases the length of the array.
 *   The newly created elements will be initialized with the
 *   value of 'init_value'.
 *
 *   The resulting length will be returned.
 *
 *   If out of memory occurred during array expansion, the
 *   array length will not be modified.
 *   If error occurred during object assignment, the array length
 *   will be increased up to the position where the error occurred.
 *
 * Parameters:
 *   amount     - amount of length to increase
 *   init_value - initialize value
 *
 * Return Value: resulting length in unsigned 32 bit integer
 *
 * Assumes: none
 */
size_t kvarraySWIFT_hash::IncreaseLength(size_t amount, const value_type init_value)
{
  size_t buf_index = 0;
  size_t cur_buflen = UUARRAY_INITIAL_SIZE;
  size_t old_size = this->total_elem;
  size_t the_index = 0;

  if (old_size + amount > old_size)
  {
    /* number did not wrap around past 2^32 - 1 back to 0, and amount != 0 */

    amount += old_size;

    /* increase buffer length */
    if (this->GrowBuffer(amount))
    {
      /* out of memory */
      return this->total_elem;
    }

    while (the_index < amount)
    {
      if (the_index + cur_buflen <= old_size)
      {
        // this buffer does not need copy, part of existing array
        the_index += cur_buflen; // increase index by current buffer length
        ++buf_index; // increase buffer index
        cur_buflen += cur_buflen; // double the current buffer length
      }
      else
      {
        // this buffer need copying
        // find out the number of objects to copy,
        // and the starting offset

        // find out how many objects to copy, based on current buffer length
        // and the left over amount to completion
        size_t num_to_copy =
          (amount - the_index > cur_buflen) ? cur_buflen : amount - the_index;

        // copy_index starts greater than 0 when the current position is
        // less than the old array length
        register size_t copy_index =
          (old_size > the_index) ? old_size - the_index : 0;

        do
        {
          *(this->the_desc[buf_index] + copy_index) = init_value; // copy
          ++(this->total_elem); // increase array length
        } while (++copy_index < num_to_copy);
        the_index += num_to_copy; // increase index by amount copied
        ++buf_index; // increse buffer index
        cur_buflen += cur_buflen; // double current buffer length
      }
    }
  }
  return this->total_elem; // return resulting array length
}


/* explain function
 * Description:
 *   DecreaseLength() decreases the length of the array.
 *   The resulting length will be returned.
 * 
 *   If the amount to be decreased is larger than the length of
 *   the array, the length will be set to 0.
 *
 * Parameters:
 *   amount - amount of length to decrease
 *
 * Return Value: resulting length in unsigned 32 bit integer
 *
 * Assumes: none
 */
size_t kvarraySWIFT_hash::DecreaseLength(size_t amount)
{
  size_t buf_index = 0;
  register size_t cur_buflen = UUARRAY_INITIAL_SIZE;
  register size_t the_length;

  /* adjust length */
  if (amount > this->Length())
  {
    the_length = this->total_elem = 0;
  }
  else
  {
    the_length = (this->total_elem -= amount);
  }

  /* determine which slice buffer to use */

  // while the length is not covered by current buffer
  while (the_length >= cur_buflen)
  {
    the_length -= cur_buflen; // decrease length by current buffer length
    ++buf_index; // increase buffer index
    cur_buflen += cur_buflen; // double current buffer length
  }

  /* remove unused slices - somewhat conservative to avoid boucing
     around the boundary conditions:
     eg. increase at the end of a buffer followed by decrease at the
     start of a buffer */
  while (buf_index + 2 < this->num_buffer)
  {
    delete [] this->the_desc[--(this->num_buffer)];
  }

  return this->total_elem; // return resulting array length
}


/* explain function
 * Description:
 *   Assigns the 'cpy' array to this array.
 *
 *   If assignment fails due to out of memory, the array's length
 *   will be set to 0.
 *
 *   If error occurs during object assignment, the length of the
 *   array will be set to the location of the error.
 *
 * Parameters:
 *   cpy - array to copy from
 *
 * Return Value: this array
 *
 * Assumes: none
 */
kvarraySWIFT_hash& kvarraySWIFT_hash::operator = (const kvarraySWIFT_hash& cpy)
{
  size_t num_to_copy = cpy.total_elem;
  register size_t buf_index = 0;
  size_t cur_buflen = UUARRAY_INITIAL_SIZE;

  this->total_elem = 0;
  if (this->GrowBuffer(cpy.Length()) == 0) // array length increase OK
  {
    while (num_to_copy > 0) // while has data to copy
    {
      // find out how many items to copy
      size_t copy_len = (num_to_copy < cur_buflen) ? num_to_copy : cur_buflen;

      for (register size_t copy_index= 0; copy_index < copy_len; ++copy_index)
      {
        // copy item
        *(this->the_desc[buf_index] + copy_index) =
          *(cpy.the_desc[buf_index] + copy_index);
        ++(this->total_elem); // increase number of elements in array
      }
      num_to_copy -= copy_len; // decrease no. of items to copy
      cur_buflen += cur_buflen; // double current buffer length
      ++buf_index; // increase buffer index
    }
  }
  else
  {
    // out of memory, delete array buffers
    while (this->num_buffer > 0)
      delete [] this->the_desc[--(this->num_buffer)];
  }

  return *this;
}


/* explain function
 * Description:
 *   Destructor of uuArray.
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
kvarraySWIFT_hash::~kvarraySWIFT_hash()
{
  while (this->num_buffer > 0)
  {
    delete [] this->the_desc[--(this->num_buffer)];
  }
}

/* purecov: end inspected */





/* explain function
 * Description:
 *   This function will split a bucket in the hash table.
 *   Do nothing if length of hash table < 2.
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
void SWIFT_hash::SplitBucket()
{
  link_type *old_chain = NULL;
  link_type *new_chain = NULL;
  link_type *cur_ptr;
  link_type *next_ptr;

  size_t old_place = this->split_ptr;
  size_t new_place = old_place + this->max_split;

  if (old_place + 1 >= this->max_split)
  {
    if (this->key_count < 2)
      return; // does not need to split when number of keys < 2

    this->max_split += this->max_split; // double max split
    this->split_ptr = 0; // wrap around the split pointer
  }
  else
  {
    this->split_ptr = old_place + 1;
  }

  /* split the original chain into old_chain, and new_chain */

  cur_ptr = this->slots[old_place];

  while (cur_ptr != NULL)
  {
    next_ptr = cur_ptr->Next();

    if (BucketOffset(this->max_split, this->split_ptr,
                     this->ident.hash(cur_ptr->Item().a)
                    ) == old_place
       )
    {
      cur_ptr->Next() = old_chain;
      old_chain = cur_ptr;
    }
    else
    {
      cur_ptr->Next() = new_chain;
      new_chain = cur_ptr;
    }
    cur_ptr = next_ptr;
  }

  /* the split operation makes the list in reversed order, need to
     re-order them. */

  if (old_chain != NULL)
    this->slots[old_place] = old_chain->Reverse();
  else
    this->slots[old_place] = NULL;

  if (new_chain != NULL)
    this->slots[new_place] = new_chain->Reverse();
  else
    this->slots[new_place] = NULL;
}


/* explain function
 * Description:
 *   This function will merge 2 buckets in the hash table.
 *   Do nothing if length of hash table < 2.
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
void SWIFT_hash::MergeBucket()
{
  if (this->split_ptr == 0)
  {
    if (this->key_count < 1)
      return; // does not need to merge when number of keys < 1

    this->max_split = this->max_split >> 1; // half the max split
    this->split_ptr = this->max_split - 1; // wrap around the split pointer
  }
  else
    --(this->split_ptr);

  size_t new_place = this->split_ptr;
  size_t old_place = new_place + this->max_split;
  char use_old_chain;

  link_type *old_chain = this->slots[old_place];
  link_type *new_chain = this->slots[new_place];
  link_type *merge_list = NULL;

  while ((old_chain != NULL) || (new_chain != NULL))
  {
    /* while a chain is not empty */

    if (old_chain == NULL)
    {
      /* old_chain is empty, use new_chain instead */
      use_old_chain = 0;
    }
    else if (new_chain == NULL)
    {
      /* new_chain is empty, use old_chain instead */
      use_old_chain = 1;
    }
    else
    {
      /* both chains are not empty */
      /* if old chain is less, use old chain */
      use_old_chain = this->ident.less(old_chain->Item().a,
                                       new_chain->Item().a);
    }

    if (use_old_chain)
    {
      link_type *old_chain_next = old_chain->Next();
      old_chain->Next() = merge_list;
      merge_list = old_chain;
      old_chain = old_chain_next;
    }
    else
    {
      link_type *new_chain_next = new_chain->Next();
      new_chain->Next() = merge_list;
      merge_list = new_chain;
      new_chain = new_chain_next;
    }
  }

  /* the merging operation makes the list in reversed order, need to
     re-order them. */

  if (merge_list != NULL)
    this->slots[new_place] = merge_list->Reverse();
  else
    this->slots[new_place] = NULL;

  /* the old bucket does not have items left */
  this->slots[old_place] = NULL;
}


/* explain function
 * Description:
 *   ClearAll() will remove all entries from the hash table.
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
void SWIFT_hash::ClearAll()
{
  link_type *cur_ptr;
  link_type *next_ptr;

  this->split_ptr = this->key_count = 0;
  this->max_split = 1;
  while (this->slots.Length() > 0)
  {
    cur_ptr = this->slots[this->slots.Length() - 1];
    while (cur_ptr != NULL)
    {
      next_ptr = cur_ptr->Next();
      delete cur_ptr;
      cur_ptr = next_ptr;
    }

    this->slots.DecreaseLength(1);
  }
}


/* explain function
 * Description:
 *   Exists(the_key) will return TRUE if the_key is in
 *   the hash table.  FALSE will be returned if the_key is not
 *   in the hash table.
 *
 * Parameters:
 *   the_key - key value to be searched in the hash table
 *
 * Return Value: TRUE if found, FALSE if not found
 *
 * Assumes: none
 */
char SWIFT_hash::Exists(const key_type& the_key) const
{
  if (this->key_count == 0)
    return 0; // zero length hash table, return not found

  size_t hash_val = this->ident.hash(the_key);
  size_t bucket_num =
    BucketOffset(this->max_split, this->split_ptr, hash_val);

  link_type *cur_link = this->slots[bucket_num];

  while (cur_link != NULL)
  {
    if (this->ident.equal(the_key, cur_link->Item().a))
    {
      return 1; // found
    }
    cur_link = cur_link->Next(); // travel down the chain
  }

  return 0; // not found
}


/* explain function
 * Description:
 *   Operator [] will return the value given the key.
 *
 *   If the key is not found in the hash, a default dummy
 *   value is returned.
 *
 * Parameters:
 *   the_key - key value to be searched in the hash table
 *
 * Return Value: the associated value
 *
 * Assumes: none
 */
SWIFT_hash::val_type&
  SWIFT_hash::operator [] (const key_type& the_key) const
{
  static val_type dummy;

  if (this->key_count == 0)
    return dummy; // zero length hash table

  size_t hash_val = this->ident.hash(the_key);
  size_t bucket_num =
    BucketOffset(this->max_split, this->split_ptr, hash_val);

  link_type *cur_link = this->slots[bucket_num];

  while (cur_link != NULL)
  {
    if (this->ident.equal(the_key, cur_link->Item().a))
    {
      return cur_link->Item().b; // found
    }
    cur_link = cur_link->Next(); // travel down the chain
  }

  return dummy; // not found
}


/* explain function
 * Description:
 *   GetAnyKey() would return a key in the hash table.  If there
 *   is not any insertion or deletion to the hash
 *   table, GetAnyKey() would always return the the same key.
 *
 *   If the hash table is length 0, a default key will be returned.
 *
 * Parameters:
 *   offset - this number will influence the key picked
 *
 * Return Value: a key in hash table, default key if length is 0
 *
 * Assumes: none
 */
SWIFT_hash::key_type SWIFT_hash::GetAnyKey(const size_t offset) const
{
  if (this->key_count == 0)
  {
    return key_type(); // return default key
  }

  // pick a starting bucket based on a hashing computation
  // 1640531551 is close to (1.0 - golden_ratio) of 2^32

  register size_t bucket_num =
    (this->key_count * 1640531551 + offset) % this->key_count;

  register link_type *cur_link = this->slots[bucket_num];

  while (cur_link == NULL)
  {
    if (++bucket_num == this->key_count)
      bucket_num = 0; // wrap around the table
    cur_link = this->slots[bucket_num];
  }
  return cur_link->Item().a; // return first key in this chain
}


/* explain function
 * Description:
 *   GetNextKey() would return a key considered to be immediately
 *   after the input key in the hash table.  This function can be
 *   used to iterate over the hash table if no elements are inserted
 *   or deleted.  If elements are inserted or deleted, the iteration
 *   may skip over some elements, while encountering other elements
 *   multiple times.
 *
 *   If there is not any insertion or deletion to the hash
 *   table, GetNextKey() would always return the the same key.
 *
 *   If the hash table is length 0, or if the input key is not in
 *   the hash table, a default key will be returned.
 *
 * Parameters:
 *   the_key - the input key
 *
 * Return Value: a key immediately after te input key
 *
 * Assumes: none
 */
SWIFT_hash::key_type
  SWIFT_hash::GetNextKey(const key_type& the_key) const
{
  static key_type dummy;

  if (this->key_count == 0)
    return dummy; // zero length hash table

  size_t hash_val = this->ident.hash(the_key);
  size_t bucket_num =
    BucketOffset(this->max_split, this->split_ptr, hash_val);

  link_type *cur_link = this->slots[bucket_num];

  while (cur_link != NULL)
  {
    if (this->ident.equal(the_key, cur_link->Item().a))
    {
      // found the input key
      cur_link = cur_link->Next(); // travel down the chain
      if (cur_link == NULL)
      {
        do
        {
          // go to next bucket
          if ((++bucket_num) == this->key_count)
            bucket_num = 0;
          cur_link = this->slots[bucket_num];
        } while (cur_link == NULL);
      }
      return cur_link->Item().a; // found
    }
    cur_link = cur_link->Next(); // travel down the chain
  }
  return dummy; // not found
}


/* explain function
 * Description:
 *   RemoveWith() will remove a mapping of key value pair in the
 *   hash table, given the key.
 *
 *   The resulting hash table length will be returned.
 *
 * Parameters:
 *   the_key  - the key
 *
 * Return Value: resulting hash table's length
 *
 * Assumes: none
 */
size_t SWIFT_hash::RemoveWith(const key_type& the_key)
{
  if (this->Length() == 0)
  {
    return 0; // removing items from length 0 hash table
  }

  size_t hash_val = this->ident.hash(the_key);
  size_t bucket_num =
    BucketOffset(this->max_split, this->split_ptr, hash_val);

  link_type *cur_ptr = this->slots[bucket_num];

  if (cur_ptr != NULL)
  {
    if (this->ident.equal(the_key, cur_ptr->Item().a))
    {
      // found the entry in the head of the list
      this->slots[bucket_num] = cur_ptr->Next(); // unlink the entry
      --(this->key_count); // decrease key count
      this->MergeBucket(); // merge two buckets
      this->slots.DecreaseLength(1); // decrease bucket table length
      delete cur_ptr; // free memory
      return this->key_count; 
    }
    link_type *prev_ptr = cur_ptr;
    cur_ptr = cur_ptr->Next();
    while (cur_ptr != NULL)
    {
      if (this->ident.equal(the_key, cur_ptr->Item().a))
      {
        // found the entry
        prev_ptr->Next() = cur_ptr->Next(); // unlink the entry
        --(this->key_count); // decrease key count
        this->MergeBucket(); // merge two buckets
        this->slots.DecreaseLength(1); // decrease bucket table length
        delete cur_ptr; // free memory
        return this->key_count; 
      }
      prev_ptr = cur_ptr;
      cur_ptr = cur_ptr->Next(); // go to next entry
    }
  }

  // not found
  return this->key_count;
}


/* explain function
 * Description:
 *   Insert() will insert into the hash table a new mapping of
 *   key and value pair.  If there is already an identical key in the
 *   hash table, the existing mapping will not be modified.
 *
 *   The resulting length of the hash table will be returned.
 *
 *   If out of memory occurred, the length of the hash table will
 *   not be modified.
 *
 * Parameters:
 *   the_key - the key
 *   the_value - the value
 *
 * Return Value: resulting hash table length
 *
 * Assumes:
 */
size_t
  SWIFT_hash::Insert(const key_type& the_key, const val_type& the_val)
{
	
  this->slots.Append(NULL); // increase bucket table length
  if (this->slots.Length() <= this->key_count)
  {
    // out of memory for bucket expansion
    return this->key_count;
  }

  size_t hash_val = this->ident.hash(the_key);
  size_t bucket_num =
    BucketOffset(this->max_split, this->split_ptr, hash_val);

  link_type *cur_ptr = this->slots[bucket_num];
  link_type *new_ptr;

  if ((cur_ptr == NULL) || this->ident.less(the_key, cur_ptr->Item().a))
  {
    // insert in head of list
    new_ptr = 
      new link_type(kvpairSWIFT_hash(the_key,the_val),cur_ptr);
    if (new_ptr != NULL)
    {
      // not out of memory for link node creation
      this->slots[bucket_num] = new_ptr;
      ++(this->key_count); // increase key count
      this->SplitBucket(); // split a bucket
    }
    else
    {
      // out of memory for link node creation
      this->slots.DecreaseLength(1);
    }
  }
  else if (this->ident.equal(the_key, cur_ptr->Item().a))
  {
    // key in head of list, not inserted
    this->slots.DecreaseLength(1);
  }
  else
  {
    link_type *prev_ptr;

    for(;;)
    {
      prev_ptr = cur_ptr;
      cur_ptr = cur_ptr->Next();
      if ((cur_ptr == NULL) || this->ident.less(the_key, cur_ptr->Item().a))
      {
        // insert in middle or end of list
        new_ptr = 
          new link_type(kvpairSWIFT_hash(the_key,the_val),cur_ptr);
        if (new_ptr != NULL)
        {
          // not out of memory for link node creation
          prev_ptr->Next() = new_ptr;
          ++(this->key_count); // increase key count
          this->SplitBucket(); // split a bucket
          break;
        }
        else
        {
          // out of memory for link node creation
          this->slots.DecreaseLength(1);
          break;
        }
      }
      else if (this->ident.equal(the_key, cur_ptr->Item().a))
      {
        // key in list, not inserted
        this->slots.DecreaseLength(1);
        break;
      }
    }
  }
  return this->key_count; // return resulting hash table length
}


/* explain function
 * Description:
 *   Assignment operator
 *
 *   If out of memory during copying, the target hash table will
 *   be set to length of 0.
 *
 * Parameters:
 *   cpy - hash table to copy from
 *
 * Return Value: this hash table
 *
 * Assumes: none
 */
SWIFT_hash& SWIFT_hash::operator =
  (const SWIFT_hash& cpy)
{
  this->ClearAll();
  for (size_t bucket_num = 0; bucket_num < cpy.Length(); ++bucket_num)
  {
    link_type *cur_ptr = cpy.slots[bucket_num];
    while (cur_ptr != NULL)
    {
      this->Insert(cur_ptr->Item().a, cur_ptr->Item().b);
      cur_ptr = cur_ptr->Next(); // go to next entry
    }
  }

  if (this->Length() != cpy.Length())
  {
    // ran out of memory during copying
    this->ClearAll();
  }
  return (*this);
}


/* explain function
 * Description:
 *   destructor
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
SWIFT_hash::~SWIFT_hash()
{
	for (int i=0; i< Gauss_Array.Length(); i++) 
		delete[] Gauss_Array[i]->Gauss_Map;
	
	Gauss_Array.Destroy();
	this->ClearAll(); // delete all elements
}


