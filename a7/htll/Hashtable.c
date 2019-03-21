#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "Hashtable.h"
#include "Hashtable_priv.h"
#include "LinkedList.h"
#include "Assert007.h"

//Homework submitter: Rongyi Chen
//Date: 3/21/2019

// a free function that does nothing
static void NullFree(void *freeme) { }

static void FreeKVP(void *freeme) {
  free(freeme); 
}

Hashtable CreateHashtable(int num_buckets) {
  if (num_buckets == 0)
    return NULL;

  Hashtable ht = (Hashtable)malloc(sizeof(struct hashtableInfo));

  if (ht == NULL) {
    return NULL;
  }

  ht->num_buckets = num_buckets;
  ht->num_elements = 0;
  ht->buckets =
      (LinkedList*)malloc(num_buckets * sizeof(LinkedList));

  if (ht->buckets == NULL) {
    free(ht);
    return NULL;
  }

  for (int i = 0; i < num_buckets; i++) {
    ht->buckets[i] = CreateLinkedList();
    if (ht->buckets[i] == NULL) {
      // Need to free everything and then return NULL
      for (int j = 0; j < i; j++) {
        DestroyLinkedList(ht->buckets[j], NULL);
      }
      free(ht);
      return NULL;
    }
  }
  return ht;
}


void DestroyHashtable(Hashtable ht, ValueFreeFnPtr valueFreeFunction) {
  // Go through each bucket, freeing each bucket
  for (int i = 0; i < ht->num_buckets; i++) {
    LinkedList bucketlist = ht->buckets[i];
    HTKeyValuePtr nextKV;
  
    // Free the values in the list; then free the list
    if (NumElementsInLinkedList(bucketlist) > 0) {
      LLIter list_iter = CreateLLIter(bucketlist);

      LLIterGetPayload(list_iter, (void**)&nextKV); 
      valueFreeFunction(nextKV->value);
 
      // Now loop through the rest
      while (LLIterHasNext(list_iter) == 1) {
        LLIterNext(list_iter);
        LLIterGetPayload(list_iter, (void**)&nextKV); 
        valueFreeFunction(nextKV->value);
      }
      DestroyLLIter(list_iter); 
    }
    DestroyLinkedList(bucketlist, FreeKVP);
  }

  // free the bucket array within the table record,
  // then free the table record itself.
  free(ht->buckets);
  free(ht);
}

// the helper funtion mentioned in STEP 1 comments
// return 0 if key is not found in the chain
// return 1 if key is found in the chain
int HelperFunction(LLIter iter, uint64_t key, HTKeyValue **kv) {
  Assert007(iter != NULL);
  while (1) {
    LLIterGetPayload(iter, (void **) kv);

    // key is found
    if ((*kv)->key == key)
      return 1;

    // out of while loop for iterate to the end of the list
    if (LLIterHasNext(iter) == 0)
      break;

    // go to the next node
    LLIterNext(iter);
  }

  // key is not found return 0
  return 0;
}

int PutInHashtable(Hashtable ht,
                   HTKeyValue kvp,
                   HTKeyValue *old_key_value) {
  Assert007(ht != NULL);
  
  int insert_bucket;
  LinkedList insert_chain;

  ResizeHashtable(ht);

  // calculate which bucket we're inserting into,
  // get the list
  insert_bucket = HashKeyToBucketNum(ht, kvp.key);
  insert_chain = ht->buckets[insert_bucket];

  // STEP 1: Finish the implementation of the put.
  // This is a fairly complex task, so you might decide you want
  // to define/implement a helper function hat helps you find
  // and optionally remove a key within a chain, rather than putting
  // all that logic inside here. You might also find that your helper(s)
  // can be reused in step 2 and 3.

  HTKeyValue *kv = (HTKeyValue *)malloc(sizeof(HTKeyValue));
  if (kv == NULL) {
    return 1;
  }
  kv->key = kvp.key;
  kv->value = kvp.value;
  // if chain has 0 elements, insert the key value pair into the hash table
  if (NumElementsInLinkedList(insert_chain) == 0) {
    // if insert fail, return 1
    if (InsertLinkedList(insert_chain, (void *) kv) != 0) {
      free(kv);
      return 1;
    }
    // if insert successfully, return 0
    ht->num_elements++;
    return 0;
  }
  // if the chain is not empty
  // create an iterator for the chain
  LLIter iter = CreateLLIter(insert_chain);
  // no more memory, return 1
  if (iter == NULL) {
    free(kv);
    return 1;
  }
  HTKeyValue *old_payload;
  // use helper function as the comments mentioned in step 1
  // if can find the key in the bucket
  if (HelperFunction(iter, kvp.key, &old_payload) == 1) {
    // insert key value pair to the bucket, if fail return 1
    if (InsertLinkedList(insert_chain, (void *) kv) != 0) {
      free(kv);
      DestroyLLIter(iter);
      return 1;
    }
    // put key and value to the returning key value pair
    old_key_value->key = old_payload->key;
    old_key_value->value = old_payload->value;
    free(old_payload);
    LLIterDelete(iter, NullFree);
    DestroyLLIter(iter);
    return 2;
  }
  DestroyLLIter(iter);

  if (InsertLinkedList(insert_chain, (void *) kv) != 0) {
    free(kv);
    return 1; 
  }
  ht->num_elements++;  
  return 0;
}

int HashKeyToBucketNum(Hashtable ht, uint64_t key) {
  return key % ht->num_buckets;
}

// -1 if not found; 0 if success
int LookupInHashtable(Hashtable ht, uint64_t key, HTKeyValue *result) {
  Assert007(ht != NULL);
  
  // STEP 2: Implement lookup
  int lookup_bucket;
  LinkedList lookup_chain;
  lookup_bucket = HashKeyToBucketNum(ht, key);
  lookup_chain = ht->buckets[lookup_bucket];

  //if bucket empty, key cannot be found, return -1
  if (NumElementsInLinkedList(lookup_chain) == 0) {
    return -1;
  }
  // create iterator for the chain
  LLIter iter = CreateLLIter(lookup_chain);
  // if memory error, return -1;
  if (iter == NULL) {
    return -1;
  }
  HTKeyValue *payload;
  // use helper function again 
  // if can find the key in bucket, copy the kv pair to the returning payload
  if (HelperFunction(iter, key, &payload) == 1) {
    result->key = payload->key;
    result->value = payload->value;
    DestroyLLIter(iter);
    // return 0 for successfully find
    return 0;
  }
  // if cannot find the key, free the iterator and return -1
  DestroyLLIter(iter);
  return -1;
}


int NumElemsInHashtable(Hashtable ht) {
  int res = 0; 
  for (int i=0; i<ht->num_buckets; i++) {
    res += NumElementsInLinkedList(ht->buckets[i]); 
  }
  return res; 
}


int RemoveFromHashtable(Hashtable ht, uint64_t key, HTKeyValue *junkKVP) {
  Assert007(ht != NULL);
  // STEP 3: Implement Remove
  int remove_bucket;
  LinkedList remove_chain;
  remove_bucket = HashKeyToBucketNum(ht, key);
  remove_chain = ht->buckets[remove_bucket];

  //if no elements in bucket, return -1
  if (NumElementsInLinkedList(remove_chain) == 0) {
    return -1;
  }
  // create iterator for the bucket
  LLIter iter = CreateLLIter(remove_chain);
  if (iter == NULL) {
    return -1;
  }
  HTKeyValue* payload; 
  // use helper function again
  // if can find the key
  if (HelperFunction(iter, key, &payload) == 1) {
    junkKVP->key = payload->key;
    junkKVP->value = payload->value;
    free(payload);
    LLIterDelete(iter, NullFree);
    DestroyLLIter(iter);
    ht->num_elements--;
    return 0;
  } 
  // if cannot find the key
  DestroyLLIter(iter);
  return -1;
}


uint64_t FNVHash64(unsigned char *buffer, unsigned int len) {
  // This code is adapted from code by Landon Curt Noll
  // and Bonelli Nicola:
  //
  // http://code.google.com/p/nicola-bonelli-repo/
  static const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
  static const uint64_t FNV_64_PRIME = 0x100000001b3ULL;
  unsigned char *bp = (unsigned char *) buffer;
  unsigned char *be = bp + len;
  uint64_t hval = FNV1_64_INIT;
  /*
   * FNV-1a hash each octet of the buffer
   */
  while (bp < be) {
    /* xor the bottom with the current octet */
    hval ^= (uint64_t) * bp++;
    /* multiply by the 64 bit FNV magic prime mod 2^64 */
    hval *= FNV_64_PRIME;
  }
  /* return our new hash value */
  return hval;
}


uint64_t FNVHashInt64(uint64_t makehash) {
  unsigned char buf[8];
  int i;
  for (i = 0; i < 8; i++) {
    buf[i] = (unsigned char) (makehash & 0x00000000000000FFULL);
    makehash >>= 8;
  }
  return FNVHash64(buf, 8);
}


void ResizeHashtable(Hashtable ht) {
  Assert007(ht != NULL);
  
  // Resize if the load factor is > 3.
  if (ht->num_elements < 3 * ht->num_buckets)
    return;
  
  // This is the resize case.  Allocate a new hashtable,
  // iterate over the old hashtable, do the surgery on
  // the old hashtable record and free up the new hashtable
  // record.
  Hashtable newht = CreateHashtable(ht->num_buckets * 9);
  // Give up if out of memory.
  if (newht == NULL)
    return;
  
  // Loop through the old ht with an iterator,
  // inserting into the new HT.
  HTIter it = CreateHashtableIterator(ht);
  if (it == NULL) {
    // Give up if out of memory.
    DestroyHashtable(newht, &NullFree);
    return;
  }

  HTKeyValue item;
  HTIteratorGet(it, &item);
  HTKeyValue old_kv; 
  
  if (PutInHashtable(newht, item, &old_kv) == 1) {
    // failure, free up everything, return.
    DestroyHashtableIterator(it);
    DestroyHashtable(newht, &NullFree);
    return;
  }
  
  while (HTIteratorHasMore(it) != 0) {
    HTIteratorNext(it);
    
    HTKeyValue item; 
    HTIteratorGet(it, &item);
    HTKeyValue old_kv; 
    
    if (PutInHashtable(newht, item, &old_kv) == 1) {
      // failure, free up everything, return.
      DestroyHashtableIterator(it);
      DestroyHashtable(newht, &NullFree);
      return;
    }
  }
  // Worked!  Free the iterator.
  DestroyHashtableIterator(it);
  // Sneaky: swap the structures, then free the new table,
  // and we're done.
  {
    struct hashtableInfo tmp;
    tmp = *ht;
    *ht = *newht;
    *newht = tmp;
    DestroyHashtable(newht, &NullFree);
  }
  return;
}


// ==========================
// Hashtable Iterator
// ==========================

// Returns NULL on failure, non-NULL on success.
HTIter CreateHashtableIterator(Hashtable table) {
  if (NumElemsInHashtable(table) == 0) {
    return NULL; 
  }
  HTIter iter = (HTIter)malloc(sizeof(HTIterRecord));
  if (iter == NULL) {
    return NULL;  // Couldn't malloc
  }
  iter->ht = table;
  iter->which_bucket = 0;
  iter->bucket_iter = CreateLLIter(iter->ht->buckets[iter->which_bucket]);

  return iter;
}


void DestroyHashtableIterator(HTIter iter) {
  // Destroy the list iter
  free(iter->bucket_iter);
  iter->ht = NULL;
  free(iter);
}

// Moves to the next element; does not return. 
int HTIteratorNext(HTIter iter) {
  // Step 4: Implement HTIteratorNext
  
  if (iter->ht->num_elements != 0 ||
      iter->bucket_iter != NULL) {
    // If there is next element in current chain
    if (LLIterNext(iter->bucket_iter)) {
      return 1;
    } else {
      // To find next chain for next element in HashTable
      int chainNum = iter->ht->num_buckets + 1;
      for (int i = iter->which_bucket + 1; i < iter->ht->num_buckets; i++) {
        if (NumElementsInLinkedList(iter->ht->buckets[i]) > 0) {
          chainNum = i;
          iter->which_bucket = i;
          break;
        }
      }
      // If cannot find next non-empty chain in HashTable, return 0
      if (chainNum >= iter->ht->num_buckets) {
        return 0;
      } else {
        // If found next non-empty chain in HashTable
        free(iter->bucket_iter);
        iter->bucket_iter = CreateLLIter(iter->ht->buckets[chainNum]);
        
        if (iter->bucket_iter == NULL) {
          DestroyHashtableIterator(iter);
          return 0;
        }
        return 1;
      }
    }
  } else {
    return 0;
  }
}

int HTIteratorGet(HTIter iter, HTKeyValuePtr dest) {
  Assert007(iter != NULL); 
  // Step 6 -- implement HTIteratorGet.
  return 0;
}

//  0 if there are no more elements.
int HTIteratorHasMore(HTIter iter) {
  if (iter->bucket_iter == NULL) {
    return 0;
  }

  if (LLIterHasNext(iter->bucket_iter) == 1)
    return 1;

  // No more in this iter; are there more buckets?
  if (iter->which_bucket < (iter->ht->num_buckets -1) )
    return 1;
  else return 0; 
}

